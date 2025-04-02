#include "../inch.h"
static volatile sig_atomic_t flag = 0;
static sigset_t newmask, oldmask, zeromask;

static void sig_handler(int sign) { flag = 1; }

static void TELL_WAIT() {

  if (signal(SIGUSR1, sig_handler) == SIG_ERR) {
    perror("handler could not be registered\n");
  }

  if (signal(SIGUSR2, sig_handler) == SIG_ERR) {
    perror("handler could not be registered\n");
  }

  sigemptyset(&newmask);
  sigemptyset(&oldmask);
  sigemptyset(&zeromask);

  sigaddset(&newmask, SIGUSR1);
  sigaddset(&newmask, SIGUSR2);

  sigprocmask(SIG_BLOCK, &newmask, &oldmask);
}

static void WAIT() {
  while (!flag) {
    sigsuspend(&zeromask);
  }

  flag = 0;
  sigprocmask(SIG_BLOCK, &oldmask, NULL);
}

static void WAKE_CHILD(pid_t pid) { kill(pid, SIGUSR1); }

static void WAKE_PARENT(pid_t pid) { kill(pid, SIGUSR2); }

static void file_operation(FILE *const f) {
  int val;
  fseek(f, 0L, SEEK_SET);
  fread(&val, sizeof(val), 1, f);

  printf("The val is:%d\n", val);
  ++val;

  fseek(f, 0L, SEEK_SET);
  fwrite(&val, sizeof(val), 1, f);
  fflush(f);
}

// Some error checks are skipped for sake of simplicity.
int main(int argc, char *argv[]) {

  FILE *f = fopen("test", "w+");
  if (!f) {
    perror("The file could not be opened!\n");
    return EXIT_FAILURE;
  }

  int init_val = 0;
  fseek(f, 0L, SEEK_SET);
  fwrite(&init_val, sizeof(int), 1, f);

  if (!f) {
    perror("FILE COULD NOT BE CREATED\n");
    return EXIT_FAILURE;
  }

  // Prepare process for waiting.
  TELL_WAIT();

  pid_t parent_pid = getpid();
  pid_t child_pid = fork();

  if (child_pid == -1) {
    perror("fork failed!");
    return EXIT_FAILURE;
  }

  if (!child_pid) {
    // Here child waits for the parent.
    for (size_t i = 0; i < 100; ++i) {
      WAIT();
      printf("Child does the file operation\n");
      file_operation(f);

      printf("Child wakes the parent up\n");
      // Wakes child up.
      WAKE_PARENT(parent_pid);
    }
    _exit(EXIT_SUCCESS);
  }

  // Here comes the parent
  for (size_t i = 0; i < 100; ++i) {
    printf("Parent does the file operation\n");
    file_operation(f);

    printf("Parent wakes child up\n");
    // Wakes child up.
    WAKE_CHILD(child_pid);
    // Then waits.
    WAIT();
    printf("Parent wakes up\n");
  }

  return EXIT_SUCCESS;
}
