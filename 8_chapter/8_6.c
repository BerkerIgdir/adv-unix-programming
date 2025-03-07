#include "../inch.h" 

static pid_t create_zombi() {
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork error");
  } else if (pid == 0) {
    exit(EXIT_FAILURE);
  }

  // we do not wait for child which qualifies it to become a zombie;
  sleep(1);
  return pid;
}

int main(int argc, char *argv[]) {
  pid_t pid = create_zombi();

  // Now child exited but parent still continues to execution.
  char cmdstr[40]; 
  snprintf(cmdstr, sizeof(cmdstr), "ps -s -p %d", pid);

  int sysret = system(cmdstr);

  if (sysret < 0) {
    perror("fork error");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
