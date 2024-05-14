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

  char buf[10];
  read(fd, buf, 10);
  printf(1, "read: %c %c\n", buf[0], buf[1]);

  char *anony = (char*)mmap(4096, 4096, PROT_READ, MAP_ANONYMOUS, -1, 0);
  if(anony == 0){
    printf(1, "ANONY failed");
  }
  printf(1, "mmap annony done %d\n", (uint)anony);
  printf(1, "ANONY READ: %c %c\n", anony[0], anony[4095]);

  char* src = (char*)mmap(0, 4096, PROT_READ, MAP_POPULATE, fd, 0);
  if(src == 0){
    printf(1, "POPULATE failed");
    exit();
  }
  printf(2, "POPULATE DONE\n");

  printf(1, "-fd data: %c %c %c %c\n", src[0], src[1], src[2], src[3]);
  exit();
}
