// Counting semaphores for user processes.
// The table lives in the kernel so it is shared by a parent and the
// children it creates with fork(). All state is protected by one
// spinlock. sem_wait() blocks with sleep() while the value is 0 and
// re-checks the value after every wake-up; the sleeper holds the same
// lock that sem_post() holds while changing the value, so no wake-up
// is lost.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "spinlock.h"

#define NSEM 8

struct {
  struct spinlock lock;
  int value[NSEM];
  int inuse[NSEM];
} semtable;

void
seminit(void)
{
  int i;

  initlock(&semtable.lock, "sem");
  for(i = 0; i < NSEM; i++){
    semtable.value[i] = 0;
    semtable.inuse[i] = 0;
  }
}

int
sem_init(int id, int value)
{
  if(id < 0 || id >= NSEM || value < 0)
    return -1;
  acquire(&semtable.lock);
  semtable.value[id] = value;
  semtable.inuse[id] = 1;
  wakeup(&semtable.value[id]);
  release(&semtable.lock);
  return 0;
}

int
sem_wait(int id)
{
  if(id < 0 || id >= NSEM)
    return -1;
  acquire(&semtable.lock);
  if(!semtable.inuse[id]){
    release(&semtable.lock);
    return -1;
  }
  while(semtable.value[id] == 0){
    if(myproc()->killed){
      release(&semtable.lock);
      return -1;
    }
    sleep(&semtable.value[id], &semtable.lock);
  }
  semtable.value[id]--;
  release(&semtable.lock);
  return 0;
}

int
sem_post(int id)
{
  if(id < 0 || id >= NSEM)
    return -1;
  acquire(&semtable.lock);
  if(!semtable.inuse[id]){
    release(&semtable.lock);
    return -1;
  }
  semtable.value[id]++;
  wakeup(&semtable.value[id]);
  release(&semtable.lock);
  return 0;
}
