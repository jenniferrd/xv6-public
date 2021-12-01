#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "walkerData.h"

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){

  case T_DEV:
  case T_FILE:
    printf(1, "%s %d %d %d\n", path, st.type, st.ino, st.size);
    walkerData[st.ino].type = st.type;
    walkerData[st.ino].size = st.size;
    break;

  case T_DIR:
    printf(1, "%s %d %d %d\n", path, st.type, st.ino, st.size);
    walkerData[st.ino].type = st.type;
    walkerData[st.ino].size = st.size;
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
       if(de.inum == 0)
          continue;
       if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0){
          continue;
       }
       memmove(p, de.name, DIRSIZ);
       //end of the string
       p[DIRSIZ] = 0;
       if(stat(buf, &st) <0){
          printf(1, "ls: cannot stat %s\n", buf);
          continue;
       }
       //buf will contain the path
       ls(buf);
    }
    break;

  default:
     printf(1,"??? %s\n", path);
     break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i, fdData;
  
  if((fdData = open("dir_data", O_CREATE | O_RDWR)) < 0){
    printf(2, "directoryWalker: cannot open data file\n");
    exit();
  }
  write(fdData, walkerData, sizeof(walkerData));
  close(fdData);
  
  if((fdData = open("dir_data", O_CREATE | O_RDWR)) < 0){
    printf(2, "directoryWalker: cannot open data file\n");
    exit();
  }

  if(argc < 2){
     ls(".");
     write(fdData, walkerData, sizeof(walkerData));
     close(fdData);
     exit();
  }
  
  for(i=1; i<argc; i++)
    ls(argv[i]);
    
  write(fdData, walkerData, sizeof(walkerData));
  close(fdData);
  
  exit();
}
