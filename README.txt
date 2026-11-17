Name: Rohit Kumar Meena
Roll No: 2401MC17
Set: 1

Q1 - Counting semaphores (sem.c, syscalls, synctest.c):
1. Added kernel/sem.c with a table of 8 semaphores (value + in-use flag) guarded by one
   spinlock (seminit() is called from main() before userinit()); sem_wait() blocks with
   sleep(chan, lock) while value == 0 holding the same lock sem_post() uses, and re-checks
   the value after every wake-up; all three calls return -1 on bad id/value or unused slot.
2. Wired the calls end-to-end: syscall numbers 22-24 in syscall.h, dispatch entries in
   syscall.c, argint wrappers in sysproc.c, user prototypes in user.h, stubs in usys.S,
   plus sem.o in Makefile OBJS.
3. synctest.c initialises semaphores 0/1/2 to 1/0/0 and forks three children; child A/B/C
   waits on its own semaphore, prints its letter 5 times, then posts the next semaphore,
   forcing strict A B C A B C ... order with no user-space sleep or busy waiting; the
   parent reaps all three children and prints Done. Verified output: ABCABCABCABCABC.

Q2 - Free-page count and address translation (kalloc.c, vm.c, memtest.c):
1. kalloc.c keeps an nfree counter (incremented in kfree, decremented in kalloc, both
   under kmem.lock when in use, so boot-time frees before the lock exists are counted too)
   and exposes it via syscall freepages() (number 25, wired through syscall.c/sysproc.c/
   user.h/usys.S like the Q1 calls).
2. vm.c adds va2pa(va), which uses walkpgdir() on the calling process's pgdir and returns
   frame address + page offset, or -1 when the PTE is missing or lacks PTE_U (PTE_P checked
   too); declared in defs.h and exposed as syscall 26.
3. memtest.c prints freepages() at start and after sbrk(10*4096) (drops by exactly 10),
   the virtual/physical address of a global, va2pa(0x7FFFFFFF) = -1, then shows parent and
   child share the virtual address but get different physical pages after fork(), and that
   freepages() after wait() returns exactly to its pre-fork value (no leak: PASS).

Build/run: make qemu-nox CPUS=1 (single CPU for deterministic output); exit QEMU with
Ctrl-a then x. Q1_output.txt / Q2_output.txt are console logs of synctest and memtest runs
with `echo 2401MC17` first so the roll number is visible.
Note: the tree keeps the stock -Werror Makefile; it was verified warning-clean (all new and
touched files compile with -Werror) and functionally tested with QEMU on Ubuntu 26.04.
