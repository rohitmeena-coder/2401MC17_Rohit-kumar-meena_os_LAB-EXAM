// Observe free-page accounting and user address translation:
// freepages() at start, after sbrk, around fork/wait; va2pa() of a
// global, of an unmapped address, and in parent vs child after fork.

#include "types.h"
#include "user.h"

int gvar = 42;

int
main(int argc, char *argv[])
{
  int start, aftersbrk, beforefork, afterfork, afterwait;
  int pid;

  start = freepages();
  printf(1, "freepages at start: %d\n", start);

  if((int)sbrk(10*4096) == -1){
    printf(1, "memtest: sbrk failed\n");
    exit();
  }
  aftersbrk = freepages();
  printf(1, "freepages after sbrk(10 pages): %d\n", aftersbrk);

  printf(1, "gvar virtual address: %x\n", (uint)&gvar);
  printf(1, "gvar physical address: %x\n", va2pa((int)&gvar));
  printf(1, "va2pa(0x7FFFFFFF): %d\n", va2pa(0x7FFFFFFF));

  beforefork = freepages();
  pid = fork();
  if(pid < 0){
    printf(1, "memtest: fork failed\n");
    exit();
  }
  if(pid == 0){
    printf(1, "child: gvar va %x pa %x\n", (uint)&gvar, va2pa((int)&gvar));
    printf(1, "child: freepages %d\n", freepages());
    exit();
  }
  printf(1, "parent: gvar va %x pa %x\n", (uint)&gvar, va2pa((int)&gvar));
  afterfork = freepages();
  printf(1, "parent: freepages after fork: %d\n", afterfork);
  wait();
  afterwait = freepages();
  printf(1, "parent: freepages after wait: %d\n", afterwait);
  if(afterwait == beforefork)
    printf(1, "no leak: PASS\n");
  else
    printf(1, "no leak: FAIL\n");
  exit();
}
