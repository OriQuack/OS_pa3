#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "memlayout.h"
#include "mmu.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "proc.h"
#include "syscall.h"

int main(){
  int fd = open("README", O_RDONLY);
  if(fd == -1){
    printf(1, "open failed");
    exit();
  }

  printf(1, "TEST START\n");

  // FREE MEM
  printf(1, "FREE MEM: %d\n", freemem());

  // MAP POPULATE
  char* src = (char*)mmap(0, 4096 * 2, PROT_READ, MAP_POPULATE, fd, 0);
  if(src == 0){
    printf(2, "POPULATE failed");
    exit();
  }
  printf(2, "SRC MAP POPULATE DONE\n");
  printf(2, "FREE MEM: %d\n", freemem());
  printf(2, "SRC -fd data: %c %c %c %c %c\n", src[0], src[1], src[2], src[3], src[8191]);
  printf(2, "\n");

  // FORK
  printf(2, "FORK:\n");
  int pid = fork();
  if(pid == 0){
    printf(2, "CHILD: FREE MEM: %d\n", freemem());
    printf(2, "CHILD: SRC -fd data: %c %c %c %c %c\n", src[0], src[1], src[2], src[3], src[8191]);
    
    printf(2, "\nFORK FORK:\n");
    int ppid = fork();
    if(ppid == 0){
      printf(2, "CHILDCHILD: SRC -fd data: %c %c %c %c\n", src[0], src[1], src[2], src[3]);
      printf(2, "KILLING CHILD CHILD\n\n");
      exit();
    }
    wait();
    if(munmap((uint)src) < 0){
      printf(2, "CHILD: SRC UNMAP FAILED\n");
      exit();
    }
    printf(2, "CHILD: SRC UNMAP DONE\n");
    printf(2, "CHILD: FREE MEM: %d\n", freemem());
    printf(2, "\n");

    // CHILD ANONY
    char* new = (char*)mmap(0, 4096 * 3, PROT_READ, MAP_ANONYMOUS, -1, 0);
    printf(2, "CHILD: NEW ANONYMOUS DONE\n");
    printf(2, "CHILD: FREE MEM: %d\n", freemem());
    printf(2, "CHILD: NEW ANONYMOUS access %c\n", new[0]);
    printf(2, "CHILD: FREE MEM: %d\n", freemem());
    printf(2, "KILLING CHILD\n\n");
    
    exit();
  }
  else{
    wait();
  }

  // UNMAP
  int k = munmap((uint)src);
  if(k == -1){
    printf(2, "UNMAP FAILED");
    exit();
  }
  printf(2, "SRC UNMAP DONE\n");
  printf(2, "FREE MEM: %d\n", freemem());

  // ANONYMOUS + POPULATE
  char* dst = (char *)mmap(0, 4096, PROT_READ, MAP_ANONYMOUS|MAP_POPULATE, -1, 0);
    if(dst == 0){
    printf(2, "ANONYMOUS POPULATE failed");
    exit();
  }
  printf(2, "DST ANONYMOUS POPULATE DONE\n");
  printf(1, "FREE MEM: %d\n", freemem());
  printf(1, "DST -fd data: %c %c %c %c %c\n", dst[0], dst[1], dst[2], dst[3], dst[4095]);
  printf(1, "\n");

  // ANONYMOUS
  char *anony = (char*)mmap(4096, 4096 * 3, PROT_READ, MAP_ANONYMOUS, -1, 0);
  if(anony == 0){
    printf(1, "ANONY failed");
    exit();
  }
  printf(1, "ANONY ANONYMOUS DONE: %x\n", (uint)anony);
  printf(1, "FREE MEM: %d\n", freemem());
  printf(1, "ANONY -fd data: %c %c %c %c\n", anony[0], anony[1], anony[2], anony[4096]);
  printf(1, "FREE MEM: %d\n", freemem());

  munmap((uint)dst);
  printf(2, "DST UNMAP DONE\n");
  printf(1, "FREE MEM: %d\n", freemem());
  munmap((uint)anony);
  printf(2, "ANONY UNMAP DONE\n");
  printf(1, "FREE MEM: %d\n", freemem());
  printf(1, "DONE TEST\n");

  exit();
}
