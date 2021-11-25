#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "heap.h"

int
main(int argc, char *argv[])
{
  //int fd = open("inodeWData", O_CREATE | O_RDWR);
  //struct hash hashtab[HASHSIZE];
  
  //initHash(hashtab);
  
  for(int i = 1; i <= 200; i++)
  {
     int size;
     int type =  imeta(i, &size);
     
    /* #define T_DIR  1   // Directory
       #define T_FILE 2   // File
       #define T_DEV  3   // Device    */
     
     if(type > 0) //if inode is allocated
     {
        printf(1, "%d %d %d\n", type, i, size);
        //addHash(hashtab, type, i , size);
        //printf(1, "%d %d %d\n", getHash(hashtab)->type, getHash(hashtab)->inum, getHash(hashtab)->size);
     }
  }
  
  //write(fd, hashtab, sizeof(hashtab));
  exit();
}
