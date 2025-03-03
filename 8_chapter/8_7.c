#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

static void open_dir(const char *const path) {
  DIR *dir = opendir(path);
  if (!dir) {
    perror("open dir error");
    exit(EXIT_FAILURE);
  }
  const int fd = dirfd(dir);
  const int flags = fcntl(fd, F_GETFD);
  printf("FD_CLOEXEC flag:%d\n", flags & FD_CLOEXEC);
}

static void open_sys(const char *const path) {
  const int fd = open(path, O_RDONLY, 0);
  if (fd < 0) {
    perror("file open error");
    exit(EXIT_FAILURE);
  }
  const int flags = fcntl(fd, F_GETFD);
  printf("FD_CLOEXEC flag:%d\n", flags & FD_CLOEXEC);
}

int main(int argc, char *argv[]) {
  const char* root = "/";
  open_dir(root);
  open_sys(root);
  return EXIT_SUCCESS;
}
