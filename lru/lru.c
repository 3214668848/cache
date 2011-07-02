#include"hash.c"




void init(int size){

    cache_num=0;
    cache=(char **)malloc(size*sizeof(char *));
    node *p; 
    int i=0;
    for(;i<size;i++){
        cache[i]=(char *)malloc(15*sizeof(char));
        p=(node *)malloc(sizeof(node));
        if(!ehead)
            ehead=etail=p;
        else{
            etail->next=p;
            etail=p;
        }
    } 
}


void move_to_head(node *ptr){

    if(!head)
        head=tail=ptr;
    else{
        ptr->next=head;
        head->prev=ptr;
        head=ptr;
    }   
}


char* lru_search_cache(char *name){

    int index;
    char *ip;
    node *result=get(name);
    if(!result)
        return NULL;
    index=result->cache_index;
    ip=cache[index];
    move_to_head(result);
    return ip;
}


void lru_put_into_cache(char *name,char *ip){

    int index,temp;
    node *ptr=get(name);
    if(ptr){
        printf("strange,it should not exist\n");
        return;
    }
    if(ehead){
        ptr=ehead;
        if(ehead==etail)
            ehead=etail=NULL;
        else{
            ehead=ehead->next;
            ptr->next=NULL;
        }
        index=cache_num++;
    }
    else{
        ptr=tail;
        tail=tail->prev;
        tail->next=NULL;
        ptr->prev=NULL;
        temp=ptr->hash_index;
        removebykey(temp);
        index=ptr->cache_index;
    }
    cache[index]=ip;
    ptr->cache_index=index;
    temp=put(name,ptr);
    ptr->hash_index=temp;
    move_to_head(ptr);
}





