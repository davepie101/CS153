#include "types.h"
#include "user.h"

int main(int argc, char *argv[])
{
	
	int PScheduler(void);
	int aging(void);

  printf(1, "\n This program tests the correctness of your lab#2\n");

  if (atoi(argv[1]) == 1)
	PScheduler();
  else if (atoi(argv[1]) == 2)
	aging();
  else 
	printf(1, "\ntype \"lab2 1\" to test priority scheduling or \"lab2 2\" to test aging");
  exit(0);	
  return 0;
 }
  
    
     int PScheduler(void){
		 
    // use this part to test the priority scheduler. Assuming that the priorities range between range between 0 to 31
    // 0 is the highest priority and 31 is the lowest priority.  

  int pid;
  int i,j,k;
  
    printf(1, "\n  Step 2: testing the priority scheduler and setpriority(int priority)) systema call:\n");
    printf(1, "\n  Step 2: Assuming that the priorities range between range between 0 to 31\n");
    printf(1, "\n  Step 2: 0 is the highest priority. All processes have a default priority of 10\n");
    printf(1, "\n  Step 2: The parent processes will switch to priority 0\n");
    set_priority(0);
    for (i = 0; i <  3; i++) {
	pid = fork();
	if (pid > 0 ) {
		continue;}
	else if ( pid == 0) {

		set_priority(30-10*i);	
		for (j=0;j<50000;j++) {
			for(k=0;k<1000;k++) {
				asm("nop"); }}
		printf(1, "\n child# %d with priority %d has finished! \n",getpid(),30-10*i);		
		exit(0);
        }
        else {
			printf(2," \n Error \n");
			exit(-1);
        }
	}

	if(pid > 0) {
		for (i = 0; i <  3; i++) {
			wait(0);

		}
                     printf(1,"\n if processes with highest priority finished first then its correct \n");
}
	exit(0);		
	return 0;}

int aging(void) {
  int pid;
  int j;

  printf(1, "\n Step 2: testing the aging of a priority. \n");
  printf(1, "\n Step 2: The parent process, which has the highest priority, will be running. While it's running, its priority is decreasing. \n");
  printf(1, "\n Step 2: The children processes will have their priorities increase while they're waiting. \n");
  printf(1, "\n The parent process with an initial priority of 0, will repeatedly print \"p\". ");
  printf(1, "\n The child process with an initial priority of 5, will repeatedly print \"c\". ");
  printf(1, "\n These two processes will alternative back and forth to demonstrate aging.\n");

  set_priority(0);
  pid = fork();
  if (pid > 0) {
    for (j = 0; j < 200; j++) {
      printf(1, "p");
      if (j % 10 == 0) {
	printf(1, "\n");
      }
    }
    wait(0); 
    exit(0);     
  }
  else {
    set_priority(5);
    for(j = 0; j < 200; j++) {
      printf(1, "c");
      if (j % 10 == 0) {
	printf(1, "\n");
      }
    }
    exit(0);
  }

  return 0;
}
