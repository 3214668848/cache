#include <stdlib.h>
#include <stdio.h>


#define NODES_MAX 5000
#define L_WEIGHT  0.99
#define H_WEIGHT  0.01



enum STAT{HIR,LIR};

typedef  struct node{
    int cache_index;
    enum STAT stat;
    int hash_index;
    int in_s;
    struct node *sprev;
    struct node *snext;
    struct node *qprev;
    struct node *qnext;
}node;

node *shead,*stail,*qhead,*qtail,*ehead,*etail;

char** cache;

int lir_max;
int hir_max;
int hir_len;
int lir_len;

int cache_num;



void init(int size);
char* lirs_search_cache(char *name);
void lirs_put_into_cache(char *name,char *ip);
void extract_from_s(node *p);
void move_to_shead(node *p);
node* find_the_victim();
void replace_cache(node *p,char *ip);
void move_to_qtail(node *p);
void promote_the_node(node *p,char *ip);
void demote_stail();
void remove_from_q(node *p);
node* cut_qhead();
void move_to_etail(node *p);
void stack_pruning();
void modify_and_add(node *p,char *name,char *ip);

