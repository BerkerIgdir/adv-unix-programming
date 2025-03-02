#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int pr_exit(int wstatus) {

  if (WIFEXITED(wstatus)) {
    printf("exited, status=%d\n", WEXITSTATUS(wstatus));
  } else if (WIFSIGNALED(wstatus)) {
    printf("killed by signal %d\n", WTERMSIG(wstatus));
  } else if (WIFSTOPPED(wstatus)) {
    printf("stopped by signal %d\n", WSTOPSIG(wstatus));
  } else if (WIFCONTINUED(wstatus)) {
    printf("continued\n");
  }

  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
  pid_t pid;
  int status;

  pid = fork();
  if (pid < 0) {
    perror("fork can not be executed\n");
  } else if (pid == 0) {
    _exit(EXIT_SUCCESS);
  }

  waitpid(pid, &status, 0);
  pr_exit(status);

  pid = fork();
  if (pid < 0) {
    perror("fork can not be executed\n");
  } else if (pid == 0) {
    abort();
  }

  waitpid(pid, &status, 0);
  pr_exit(status);

  return EXIT_SUCCESS;
}
