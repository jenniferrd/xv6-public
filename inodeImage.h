#include "user.h"
#define HASHSIZE 10

//I forgot that I needed dynamic memory for this so

typedef struct hash hash;

struct hash {
   int type;
   int inum;
   int size;
   hash* next;
};

void initHash(struct hash *hashtab)
{
   for(int i = 0; i < HASHSIZE; i++)
   {
      hashtab[i].type = 0; 
      hashtab[i].inum = 0; 
      hashtab[i].size = 0; 
      hashtab[i].next = 0; 
   }
}

void addHash(struct hash *hashtab, int type, int inum, int size)
{
   int i = inum % HASHSIZE;
   struct hash *ep = &hashtab[i];
   while(ep->next != 0)
      ep = ep->next;
   ep->type = type;
   ep->type = inum;
   ep->type = size;
}

struct hash getHash(struct hash *hashtab, int inum)
{
   int i = inum % HASHSIZE;
   struct hash *ep = &hashtab[i];
   while(ep->inum != inum)
      ep = ep->next;
   return *ep;
}
