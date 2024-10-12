#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//Depending on the value, it returns different information
//(1) A count of the processes in the system;
//(2) A count of the total number of system calls that the current process has made so far;
//(3) The number of memory pages the current process is using.
uint64
sys_info(void)
{
  int param;
  argint(0,&param);
  
  uint proc_count = 0;
  uint syscall_num;
  uint mem_page;
  
  struct proc *current_p = myproc();
  struct proc *p;

  if(param == 1)
  {
    for(p = proc; p < &proc[NPROC];p++){
      if((p->state == SLEEPING) || (p->state == RUNNABLE ) || (p->state == RUNNING)){
      proc_count ++;
      }
    }
    printf("Total number of processes: %d\n",proc_count);
    return 0;
  }
  else if(param == 2)
  {
    syscall_num = current_p->syscall_count;
    printf("Total number of system calls made by current process: %d\n",syscall_num);
    return 0;
  }
  else if(param ==3)
  {
    uint mem_size = current_p->sz;
    mem_page = mem_size / PGSIZE;
    printf("Total number of memory pages used by current process: %d\n",mem_page);
    return 0;
  }
  else
  {
    return -1;
  }
}
