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

void main(){
  int fd = open("README.md", O_RDONLY);
  if(fd == -1){
    printf(1, "open failed");
    exit();
  }

  char* src = mmap(0, 4096, PROT_READ, MAP_POPULATE, fd, 0);
  if(src == 0){
    printf(1, "mmap failed");
    exit();
  }

  printf(1, "-fd data: %c %c %c\n", src[0], src[1], src[2]);
  exit();
}