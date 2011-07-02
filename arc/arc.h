#include<stdlib.h>
#include<stdio.h>


enum STAT{T1,B1,T2,B2};

typedef struct node{
    int cache_index;
    enum STAT stat;
    int hash_index;
    struct node *next;
    struct node *prev;
}node;

node *t1head,*t1tail,*b1head,*b1tail,
     *t2head,*t2tail,*b2head,*b2tail;

node *ehead,*etail;

char **cache;

int cache_size;

int cache_num;

int p;

int t1_num,b1_num,t2_num,b2_num;




void init(int size);
int replace(int flag);
char* arc_search_cache(char *name);
void move_t1tail_to_b1head();
void move_t2tail_to_b2head();
void extract_the_node(node *ptr);
void move_to_t1head(node *ptr);
void move_to_t2head(node *ptr);
void arc_put_into_cache(char *name,char *ip);
void remove_b1tail();
void remove_b2tail();
int remove_t1tail();
void move_to_etail(node *ptr);

