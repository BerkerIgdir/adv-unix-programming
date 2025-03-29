#include "../inch.h"

#define MAX_SIGNAL_NAME_LENGTH 20
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int sig2str(int num, char *str)
{
  static const char const *SIGNAL_NAMES[] = {
      [0] = "NULL",
      [SIGHUP] = "SIGHUP",
      [SIGINT] = "SIGINT",
      [SIGQUIT] = "SIGQUIT",
      [SIGILL] = "SIGILL",
      [SIGTRAP] = "SIGTRAP",
      [SIGABRT] = "SIGABRT",
      [SIGIOT] = "SIGIOT",
      [SIGBUS] = "SIGBUS",
      [SIGFPE] = "SIGFPE",
      [SIGKILL] = "SIGKILL",
      [SIGUSR1] = "SIGUSR1",
      [SIGSEGV] = "SIGSEGV",
      [SIGUSR2] = "SIGUSR2",
      [SIGPIPE] = "SIGPIPE",
      [SIGALRM] = "SIGALRM",
      [SIGTERM] = "SIGTERM",
      [SIGSTKFLT] "SIGSTKFLT",
      [SIGCHLD] = "SIGCHLD",
      [SIGCLD] = "SIGCLD",
      [SIGCONT] = "SIGCONT",
      [SIGSTOP] = "SIGSTOP",
      [SIGTSTP] = "SIGTSTP",
      [SIGTTIN] = "SIGTTIN",
      [SIGTTOU] = "SIGTTOU",
      [SIGURG] = "SIGURG",
      [SIGXCPU] = "SIGXCPU",
      [SIGXFSZ] = "SIGXFSZ",
      [SIGVTALRM] "SIGVTALRM",
      [SIGPROF] = "SIGPROF",
      [SIGWINCH] = "SIGWINCH",
      [SIGIO] = "SIGIO",
      [SIGPOLL] = "SIGPOLL",
      [SIGPWR] = "SIGPWR",
      [SIGSYS] = "SIGSYS"};

  // initial checks
  if (num < 1 || num >= ARRAY_SIZE(SIGNAL_NAMES))
  {
    perror("num is out of bond");
    return -1;
  }

  strcpy(str, SIGNAL_NAMES[num] + 3);
  return 0;
}

int main(int argc, char *argv[])
{
  char name[MAX_SIGNAL_NAME_LENGTH];
  if (sig2str(SIGINT, name))
  {
    fprintf(stderr, "sig2str failed\n");
    return EXIT_FAILURE;
  };

  printf("%s\n", name);
  return EXIT_SUCCESS;
}
