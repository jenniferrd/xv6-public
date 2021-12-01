//User program to compare walkers
#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "walkerData.h"

void runBothWalkers(int dir_in, int ino_in){
   if(fork()==0){
      //redirect standard output
      close(1);
      dup(dir_in);

      char *child_argv[] = {"directoryWalker","/",(char*)0};
      exec(child_argv[0],child_argv);
   }

   if(fork()==0){
      //redirect standard output
      close(1);
      dup(ino_in);

      char *child_argv[] = {"inodeTBWalker",(char*)0};
      exec(child_argv[0],child_argv);
   }
}

void gen_filename(char *buf, uint n){
	char *prefix = "/lost+found-";
	int i =0; //our position within the buff
	while (prefix[i] != '\0'){
		buf[i] = prefix[i];
		i++;

	}
	if (n>=1000){
		return;
	}
	else if(n>=100){
		buf[i++] = (n/100) +'0'; //hundreds place
		buf[i++] = ((n/10) % 10) + '0'; //tenth place
		buf[i++] = (n %10) + '0';      //ones place
	}
	else if(n>=10){
		buf[i++] = (n/10) + '0'; //tens place
		buf[i++] = (n %10) + '0'; //ones place
	}
	else{

		buf[i++] = n + '0';

	}
	buf[i] = '\0';


}
int main(void){

   int dir_pipes[2], ino_pipes[2];
   pipe(dir_pipes);
   pipe(ino_pipes);

   runBothWalkers(dir_pipes[1], ino_pipes[1]);
   int dir_out = dir_pipes[0], ino_out = ino_pipes[0];
   //dir_out and ino_out are file descriptors for the walkers' output
   
   
   struct entry wdi;
   struct entry wdd;
   


   int broken=0;
     for(int i = 1; i < NINODES; i++){
	   read(ino_out, &wdi, sizeof(wdi));
	   read(dir_out, &wdd, sizeof(wdd));

        if(wdi.type > 0 && wdd.type == 0){
         walkerData[i] = wdi;
         broken++;
      }
   }
   
   printf(1, "Number of broken files: %d\n", broken);

   char name[64];
   for(int i = 1; i < NINODES; i++){ //walkerData will hold the broken inodes (the difference between the walkers)

      if(walkerData[i].type > 0){
         printf(1, "%d %d %d\n", walkerData[i].type, i, walkerData[i].size);
		 gen_filename(name,i);
		 recover(i,name);
	  }
		
   }
   //printWalkerData(50);

   wait();
   wait();


   exit();
}

