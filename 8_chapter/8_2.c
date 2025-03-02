#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int vfork_demo() {
  pid_t pid;

  if (vfork() < 0) {
    perror("fork error");
  } else if (pid == 0) {
    return 1;
  }

  printf("parent returns from vfork_demo\n");
  return 0;
}

int vfork_demo_1() {
  int ret = vfork_demo();
  if (ret) {
    printf("child exits\n");
    // This exit operation here is important because otherwise child returns
    // from the the function itself in the same virtual memory with parent then
    // when parent tries to run further after child exists, segfault occurs
    // because of the fact that the function itself returned already, hence the
    // stack allocated for it is already deallocated.
    _exit(0);
  } else {
    printf("parent runs\n");
  }
  printf("parent returns from vfork_demo_1\n");
  return 0;
}

int main(int argc, char *argv[]) {
  int ret = vfork_demo_1();
  printf("vfork_demo_1 ret = %d\n", ret);

  return EXIT_SUCCESS;
}
