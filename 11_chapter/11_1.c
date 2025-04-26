#include "../inch.h"
#include <pthread.h>

// What we need to do in this example is to create the object in the heap to
// prevent automatic deallocation when it goes out of the scope.
//  In this case we have to take care of the life cycle of the object.

struct foo {
  int a, b, c, d;
};

void printfoo(const char *s, const struct foo *fp) {
  printf("%s", s);
  printf(" structure at 0x%lx\n", (unsigned long)fp);
  printf(" foo.a = %d\n", fp->a);
  printf(" foo.b = %d\n", fp->b);
  printf(" foo.c = %d\n", fp->c);
  printf(" foo.d = %d\n", fp->d);
}

void *thr_fn1(void *arg) {
  //I could have created the object on the heap also here and then pass the pointer to the pthread_exit function. 
  struct foo *p = (struct foo *)arg;
  printfoo("thread 1:\n", arg);
  pthread_exit((void *)arg);
}

void *thr_fn2(void *arg) {
  printf("thread 2: ID is %lu\n", (unsigned long)pthread_self());
  pthread_exit((void *)0);
}

int main(int argc, char *argv[]) {
  int err;
  pthread_t tid1, tid2;

  //A pointer to pass to pthread_join
  struct foo *fp;
  struct foo *p = malloc(sizeof(struct foo));
  if (!p) {
    perror("malloc failed.\n");
  }
  p-> a =1;
  p-> b =2;
  p-> c =3;
  p-> d =4;

  err = pthread_create(&tid1, NULL, thr_fn1, (void *)p);
  if (err != 0)
    perror("can’t create thread 1");
  err = pthread_join(tid1, (void *)&fp);
  if (err != 0)
    perror("can’t join with thread 1");
  sleep(1);
  printf("parent starting second thread\n");
  err = pthread_create(&tid2, NULL, thr_fn2, NULL);
  if (err != 0)
    perror("can’t create thread 2");
  sleep(1);
  printfoo("parent:\n", fp);

  free(p);
  return EXIT_SUCCESS;
}
