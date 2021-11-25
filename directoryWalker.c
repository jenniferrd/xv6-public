#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "heap.h"

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
    break;

  case T_DIR:
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
       printf(1, "%s %d %d %d\n", buf, st.type, st.ino, st.size);
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
  int i;

  if(argc < 2){
    ls(".");
    exit();
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit();
}
