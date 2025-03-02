#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/resource.h>

// oldP is not passed as const pointer because it is going to be freed.
static int *realloc_array(int *oldP, size_t oldSize, size_t newSize)
{
  int *newArrP = (int *)malloc(sizeof(int) * newSize);
  memcpy(newArrP, oldP, oldSize);
  free(oldP);
  return newArrP;
}

// The assignment is to implement a dup2 which does not rely on fcntl function.
// TO DO: in case of closing and returning newfd, it must be atomically.
int mdup2(int oldfd, int newfd)
{
  if (oldfd < 0 || newfd < 0)
  {
    errno = EBADF;
    return EXIT_FAILURE;
  }

  struct rlimit rl;
  if (getrlimit(RLIMIT_NOFILE, &rl) == 0)
  {
    if (newfd > rl.rlim_cur)
    {
      errno = EBADF;
      printf("Error Message: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }
  }
  else
  {
    printf("Error Message: %s\n", strerror(errno));
    printf("The soft limit for number of file limit couldn't be fetched: %s\n", strerror(errno));
    printf("The check will not be made.\n");
  }

  struct stat st;
  // if oldfd is invalid, then the call fails and newfd is not closed.
  if (fstat(oldfd, &st))
  {
    printf("Error Message: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  // if oldfd and newfd are the same then dup2 does nothing and returns it.
  if (oldfd == newfd)
  {
    return newfd;
  }

  // if newfd is already open, then it will be closed silently first.
  memset(&st, 0, sizeof(struct stat));
  if (!fstat(newfd, &st))
  {
    close(newfd);
  }

  size_t i = 0;
  size_t arraySize = 64;
  int tempfd = -1;
  int *tempfdArr = (int *)malloc(sizeof(int) * arraySize);
  while (tempfd != newfd)
  {
    tempfd = dup(oldfd);
    tempfdArr[i] = tempfd;
    if (i++ == arraySize)
    {
      tempfdArr = realloc_array(tempfdArr, arraySize, arraySize * 2);
      arraySize *= 2;
    }
  }

  // Not sure if it is a good idea but I am too lazy to implement a linked list to store all fd's anyway.
  size_t forLim = newfd - oldfd - 1;
  for (size_t i = 0; i < forLim; i++)
  {
    close(tempfdArr[i]);
  }

  free(tempfdArr);
  // if everything is ok dup2 returns the newfd as return value as new valid
  // file descriptor.
  return tempfd;
}

// The rest of the code was taken from somewhere else to test.
int main(int argc, char *argv[])
{
  const int TARGET_FD = 42;
  char buffer[64] = {};

  snprintf(buffer, sizeof(buffer) - 1, "ls -l /proc/%d/fd", getpid());

  // Open some file
  int fd1 = open("/dev/zero", O_RDONLY);
  if (fd1 < 0)
  {
    perror("open");
    return 1;
  }

  system(buffer);

  // Create a gap
  int fd2 = dup2(1, TARGET_FD);
  if (fd2 < 0)
  {
    perror("dup2");
    return 1;
  }

  system(buffer);

  // Try to replace TARGET_FD with /dev/zero
  int fd3 = mdup2(fd1, TARGET_FD);
  if (fd3 < 0 || fd3 != TARGET_FD)
  {
    perror("my_dup2");
    return 1;
  }

  system(buffer);

  return EXIT_SUCCESS;
}
