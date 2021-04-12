#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
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
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
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

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  trace();

  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
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

int sys_sigalarm(void){
  int interval;
  uint64 handler;

  if(argint(0, &interval) < 0)
    return -1;
  if(argaddr(1, &handler) < 0)
    return -1;

  struct proc *p = myproc();
  p->interval = interval;
  p->handler = handler;
  p->ticks = 0;
  return 0;
};

int sys_sigreturn(void){
  struct proc *p = myproc();

  p->trapframe->epc = p->alarm.epc;
  p->trapframe->ra = p->alarm.ra;
  p->trapframe->sp = p->alarm.sp;
  p->trapframe->gp = p->alarm.gp;
  p->trapframe->tp = p->alarm.tp;

  p->trapframe->s0 = p->alarm.s0;
  p->trapframe->s1 = p->alarm.s1;
  p->trapframe->s2 = p->alarm.s2;
  p->trapframe->s3 = p->alarm.s3;
  p->trapframe->s4 = p->alarm.s4;
  p->trapframe->s5 = p->alarm.s5;
  p->trapframe->s6 = p->alarm.s6;
  p->trapframe->s7 = p->alarm.s7;
  p->trapframe->s8 = p->alarm.s8;
  p->trapframe->s9 = p->alarm.s9;
  p->trapframe->s10 = p->alarm.s10;
  p->trapframe->s11 = p->alarm.s11;

  p->trapframe->t0 = p->alarm.t0;
  p->trapframe->t1 = p->alarm.t1;
  p->trapframe->t2 = p->alarm.t2;
  p->trapframe->t3 = p->alarm.t3;
  p->trapframe->t4 = p->alarm.t4;
  p->trapframe->t5 = p->alarm.t5;
  p->trapframe->t6 = p->alarm.t6;

  p->trapframe->a0 = p->alarm.a0;
  p->trapframe->a1 = p->alarm.a1;
  p->trapframe->a2 = p->alarm.a2;
  p->trapframe->a3 = p->alarm.a3;
  p->trapframe->a4 = p->alarm.a4;
  p->trapframe->a5 = p->alarm.a5;
  p->trapframe->a6 = p->alarm.a6;
  p->trapframe->a7 = p->alarm.a6;


  p->in = 0;
  return 0;
}