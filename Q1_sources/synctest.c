// Force three children printing A, B and C (5 times each) into the
// strict order A B C A B C ... using only counting semaphores.
// Semaphore 0 guards A (starts at 1), 1 guards B, 2 guards C.

#include "types.h"
#include "user.h"

void
child(char *letter, int waitid, int postid)
{
  int i;

  for(i = 0; i < 5; i++){
    if(sem_wait(waitid) < 0){
      printf(1, "synctest: sem_wait failed\n");
      exit();
    }
    printf(1, letter);
    if(sem_post(postid) < 0){
      printf(1, "synctest: sem_post failed\n");
      exit();
    }
  }
  exit();
}

int
main(int argc, char *argv[])
{
  if(sem_init(0, 1) < 0 || sem_init(1, 0) < 0 || sem_init(2, 0) < 0){
    printf(1, "synctest: sem_init failed\n");
    exit();
  }
  if(fork() == 0)
    child("A", 0, 1);
  if(fork() == 0)
    child("B", 1, 2);
  if(fork() == 0)
    child("C", 2, 0);
  wait();
  wait();
  wait();
  printf(1, "\nDone\n");
  exit();
}
