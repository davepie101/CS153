#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

struct {
  struct spinlock lock;
  struct shm_page {
    uint id;
    char *frame;
    int refcnt;
  } shm_pages[64];
} shm_table;

void shminit() {
  int i;
  initlock(&(shm_table.lock), "SHM lock");
  acquire(&(shm_table.lock));
  for (i = 0; i< 64; i++) {
    shm_table.shm_pages[i].id =0;
    shm_table.shm_pages[i].frame =0;
    shm_table.shm_pages[i].refcnt =0;
  }
  release(&(shm_table.lock));
}

int shm_open(int id, char **pointer) {

  struct proc *curproc = myproc();
  int i;
  int exists = 0;
  int first_empty = -1;

  acquire(&(shm_table.lock));
  for (i = 0; i < 64; i++) { //Look through shm_table for id
    if (shm_table.shm_pages[i].id == id) {
      exists = 1;
      break;
    }
    if (shm_table.shm_pages[i].id == 0 && first_empty == -1) {
      first_empty = i;
    }
  }

  if (exists) { //Another process did shm_open and id exists
    uint va = PGROUNDUP(curproc->sz);

    if (mappages(curproc->pgdir, (void*)va, PGSIZE, V2P(shm_table.shm_pages[i].frame), PTE_W | PTE_U) != -1) { //Find pa and map to available page in our va space
      shm_table.shm_pages[i].refcnt++; //Increment reference
      *pointer = (char *)va;           //Return pointer to virtual address
      curproc->sz += PGSIZE;           //Update sz
    }
    release(&(shm_table.lock));
    return 0;
  }
  else {       //id doesn't exist
    char* temp = kalloc();
    i = first_empty;
    //Allocate a page to the frame
    memset(temp, 0, PGSIZE);
    
    uint va = PGROUNDUP(curproc->sz);
    if (mappages(curproc->pgdir, (void*)va, PGSIZE, V2P(temp), PTE_W | PTE_U) != -1) {
      shm_table.shm_pages[i].id = id;
      shm_table.shm_pages[i].frame = temp;
      shm_table.shm_pages[i].refcnt = 1;
 
      *pointer = (char *)va;
       curproc->sz += PGSIZE;
    }
    release(&(shm_table.lock));
    return 0;
  }
  
  return -1;           //Errors occurred
}


int shm_close(int id) {
  int i = 0;
  int exists = 0;
  acquire(&(shm_table.lock));
  for (i = 0; i < 64; i++) {
    if (shm_table.shm_pages[i].id == id) {
      exists = 1;
      break;
    }
  }

  if (exists) {
    int num_ref = shm_table.shm_pages[i].refcnt--;
    if (num_ref == 0) {
      shm_table.shm_pages[i].frame = 0;
      shm_table.shm_pages[i].id = 0;
    }
  }
  release(&(shm_table.lock));
  return 0; //added to remove compiler warning -- you should decide what to return
}
