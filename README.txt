Name: Rohit Kumar Meena
Roll No: 2401MC17
Set: 1

Q1 – Counting Semaphores
1. Created sem.c with 8 semaphores protected by a spinlock. sem_wait() blocks using sleep() and sem_post() wakes waiting processes.
2. Added system calls 22–24 and connected them through kernel and user files.
3. Created synctest.c with three child processes using semaphores to enforce A → B → C execution.

Q2 – Free Page Count & Address Translation
1. Added nfree counter in kalloc.c and exposed it through freepages() syscall (25).
2. Implemented va2pa(va) in vm.c to convert virtual addresses into physical addresses (syscall 26).
3. Created memtest.c to test memory allocation, address translation, and parent-child memory separation after fork().
