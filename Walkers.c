//User program to compare walkers
#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "walkerData.h"

void runBothWalkers(){//int dir_in, int ino_in, int times){
   if(fork()==0){
      //redirect standard output
      //close(1);
      //dup(dir_in);

      char *child_argv[] = {"directoryWalker",(char*)0};
      exec(child_argv[0],child_argv);
   }

   if(fork()==0){
      //redirect standard output
      //close(1);
      //dup(ino_in);

      char *child_argv[] = {"inodeTBWalker",(char*)0};
      exec(child_argv[0],child_argv);
   }
}

int main(void){
   int fdi, fdd;
   int broken = 0;
   
   if((fdi = open("ino_data", O_CREATE | O_RDONLY)) < 0){
     printf(2, "Walker: cannot open data file\n");
     exit();
   }

   if((fdd = open("dir_data", O_CREATE | O_RDONLY)) < 0){
     printf(2, "Walker: cannot open data file\n");
     exit();
   }


   //int dir_pipes[2], ino_pipes[2];
   //pipe(dir_pipes);
   //pipe(ino_pipes);

   runBothWalkers();//dir_pipes[1], ino_pipes[1], 2);
   //int dir_out = dir_pipes[0], ino_out = ino_pipes[0];
   //dir_out and ino_out are file descriptors for the walkers' output

   //increments to avoid complaints at compiling time(maybe change)
   //ino_out++;
   //dir_out++;
   
   wait();
   wait();
   
   struct entry wdi[NINODES] = {0};
   struct entry wdd[NINODES] = {0};
   
   read(fdi, (char *)wdi, sizeof(wdi));
   read(fdd, (char *)wdd, sizeof(wdd));
   
   for(int i = 0; i < NINODES; i++)
      if(wdi[i].type > 0 && wdd[i].type == 0){
         walkerData[i] = wdi[i];
         broken++;
      }
   
   printf(1, "Number of broken files: %d\n", broken);    
   for(int i = 0; i < NINODES; i++) //walkerData will hold the broken inodes (the difference between the walkers)
      if(walkerData[i].type > 0)
         printf(1, "%d %d %d\n", walkerData[i].type, i, walkerData[i].size);
   
   //printWalkerData(50);
	
   exit();
}

