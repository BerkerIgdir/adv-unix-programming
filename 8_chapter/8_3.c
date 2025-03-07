#include "../inch.h"

static void pr_exit(const siginfo_t *infop) {

  int wcode = infop->si_code;
  int wstatus = infop->si_status;
  if (wcode & CLD_KILLED) {
    printf("abnormally exited, status = %d\n", wstatus);
  } else {
    printf("exited, status = %d\n", wstatus);
  }
}

int main(int argc, char *argv[]) {
  pid_t pid;
  siginfo_t infop;

  pid = fork();
  if (pid < 0) {
    perror("fork can not be executed\n");
  } else if (pid == 0) {
    _exit(11);
  }

  if (waitid(P_PID, pid, &infop, WEXITED | WSTOPPED)) {
    perror("waitid error");
    exit(EXIT_FAILURE);
  }
  pr_exit(&infop);

  pid = fork();
  if (pid < 0) {
    perror("fork can not be executed\n");
  } else if (pid == 0) {
    abort();
  }

  if (waitid(P_PID, pid, &infop, WEXITED | WSTOPPED)) {
    perror("waitid error");
    exit(EXIT_FAILURE);
  }
  pr_exit(&infop);

  return EXIT_SUCCESS;
}
