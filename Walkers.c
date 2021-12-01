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

int main(void){

   int dir_pipes[2], ino_pipes[2];
   pipe(dir_pipes);
   pipe(ino_pipes);

   runBothWalkers(dir_pipes[1], ino_pipes[1]);
   int dir_out = dir_pipes[0], ino_out = ino_pipes[0];
   //dir_out and ino_out are file descriptors for the walkers' output
   
   
   struct entry wdi[NINODES] = {0};
   struct entry wdd[NINODES] = {0};
   
   read(ino_out, (char *)wdi, sizeof(wdi));
   read(dir_out, (char *)wdd, sizeof(wdd));

   int broken=0;
   //not sure what inode 0 is but it is weird
   for(int i = 1; i < NINODES; i++)
      if(wdi[i].type > 0 && wdd[i].type == 0){
         walkerData[i] = wdi[i];
         broken++;
      }
   
   printf(1, "Number of broken files: %d\n", broken);    
   for(int i = 0; i < NINODES; i++) //walkerData will hold the broken inodes (the difference between the walkers)
      if(walkerData[i].type > 0)
         printf(1, "%d %d %d\n", walkerData[i].type, i, walkerData[i].size);
  
   //printWalkerData(50);

   //Commented because it does not work correctly with the waits
   //wait();
   //wait();


   exit();
}

