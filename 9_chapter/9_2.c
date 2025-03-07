#include "../inch.h"

int main(int argc, char *argv[]) {
  int status;
  const pid_t pid = fork();

  if (pid == -1) {
  } else if (!pid) {
    // child creates a session.
    const pid_t sid = setsid();
    printf("setsid returns %d\n", sid);
    if (sid == -1) {
      perror("session coult not be created");
      exit(EXIT_FAILURE);
    }

    const pid_t child_pid = getpid();
    // check if the child is the leader of the session it created.
    if (child_pid != sid) {
      perror("current processe is not the leader!");
      exit(EXIT_FAILURE);
    }
    const pid_t control_term_pid = tcgetsid(1);
    printf("tcgetsid returns %d\n", control_term_pid);
    // checks if the child (itself) has the controlling terminal.
    if (child_pid == control_term_pid) {
      perror("child process has the controlling terminal!");
      exit(EXIT_FAILURE);
    }
  }
  wait(&status);

  exit(EXIT_SUCCESS);
}
