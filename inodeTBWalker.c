#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "walkerData.h"

int
main(int argc, char *argv[])
{ 
  int fdData;
  
  if((fdData = open("ino_data", O_CREATE | O_RDWR)) < 0){
    printf(2, "inodeTBWalker: cannot open data file\n");
    exit();
  }
  write(fdData, walkerData, sizeof(walkerData));
  close(fdData);
  //because xv6 has no lseek, this is how the data file is zeroed out

  if((fdData = open("ino_data", O_CREATE | O_RDWR)) < 0){
    printf(2, "inodeTBWalker: cannot open data file\n");
    exit();
  }

  for(int i = 1; i <= 200; i++)
  {
     int size;
     int type;
     if((type =  imeta(i, &size)) < 0){
        printf(2, "imeta failed with %d\n", type);
        exit();
    }
     
    /* #define T_DIR  1   // Directory
       #define T_FILE 2   // File
       #define T_DEV  3   // Device    */

     if(type > 0) //if inode is allocated
     {
        printf(1,"%d %d %d\n", type, i, size);
        walkerData[i].type = type;
        walkerData[i].size = size;
     }
  }
  write(fdData, walkerData, sizeof(walkerData));
  close(fdData);
  exit();
}
