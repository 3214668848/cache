#include<stdlib.h>
#include<stdio.h>


typedef struct node{
   int cache_index;
   int hash_index;
   struct node *prev;
   struct node *next;
}node;

node *head,*tail,*ehead,*etail;

char **cache;

int cache_num;




char* lru_search_cache(char *name);
void move_to_head(node *ptr);
void lru_put_into_cache(char *name,char *ip);
void init(int size);
