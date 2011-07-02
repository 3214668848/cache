#include"hash.c"





void init(int size){
    
    cache_size=size;
    cache_num=0;
    p=0;
    t1_num=b1_num=t2_num=b2_num=0;
    cache=(char **)malloc(size*sizeof(char *));
    int i=0;
    for(;i<size;i++)
        cache[i]=(char *)malloc(15*sizeof(char));
    i=0;
    node *p;
    for(;i<2*size;i++){
        p=(node *)malloc(sizeof(node));
        if(!ehead)
            ehead=etail=p;
        else{
            etail->next=p;
            etail=p;
        }
    }
}


int replace(int flag){

    int index;
    if(t1_num>=1 &&
       (t1_num>p || (t1_num==p && 2==flag))){
        index=t1tail->cache_index;
        t1tail->cache_index=-1;
        move_t1tail_to_b1head();
    }
    else{
        index=t2tail->cache_index;    
        t2tail->cache_index=-1;
        move_t2tail_to_b2head();
    }
    return index;
}


char* arc_search_cache(char *name){

    char *ip;
    int index;
    node *result=get(name);
    if(!result || -1==result->cache_index)
        return NULL;
    index=result->cache_index;
    ip=cache[index];
    if(result!=t2head){
        extract_the_node(result);
        move_to_t2head(result);
    }
    return ip;    
}


void move_t1tail_to_b1head(){

    t1_num--;
    b1_num++;
    t1tail->stat=B1;
    if(t1head==t1tail){
        if(!b1head)
            b1head=b1tail=t1tail;
        else{
            t1tail->next=b1head;
            b1head->prev=t1tail;
            b1head=t1tail;
        }
        t1head=t1tail=NULL;
    }
    else{
        if(!b1head)
            b1head=b1tail=t1tail;
        else{
            t1tail->next=b1head;
            b1head->prev=t1tail;
            b1head=t1tail;
        }
        t1tail=t1tail->prev;
        t1tail->next=NULL;
        b1head->prev=NULL;
    }
}


void move_t2tail_to_b2head(){

    t2_num--;
    b2_num++;
    t2tail->stat=B2;
    if(t2head==t2tail){
        if(!b2head)
            b2head=b2tail=t2tail;
        else{
            t2tail->next=b2head;
            b2head->prev=t2tail;
            b2head=t2tail;
        }
        t2head=t2tail=NULL;
    }
    else{
        if(!b2head)
            b2head=b2tail=t2tail;
        else{
            t2tail->next=b2head;
            b2head->prev=t2tail;
            b2head=t2tail;
        }
        t2tail=t2tail->prev;
        t2tail->next=NULL;
        b2head->prev=NULL;
    }
}


void extract_the_node(node *ptr){

    node **head;
    node **tail;
    switch(ptr->stat){

    case T1:
        head=&t1head;
        tail=&t1tail;
        t1_num--;
        break;
    case B1:
        head=&b1head;
        tail=&b1tail;
        b1_num--;
        break;
    case T2:
        head=&t2head;
        tail=&t2tail;
        t2_num--;
        break;
    case B2:
        head=&b2head;
        tail=&b2tail;
        b2_num--;
        break;
    }
    if(*head==*tail)
        *head=*tail=NULL;
    else{
        if(ptr==*head){
            *head=(*head)->next;
            (*head)->prev=NULL;
            ptr->next=NULL;
        }
        else{
            if(ptr==*tail){
                *tail=(*tail)->prev;
                (*tail)->next=NULL;
                ptr->prev=NULL;
            }
            else{
                node *before=ptr->prev;
                node *after=ptr->next;
                before->next=after;
                after->prev=before;
                ptr->next=ptr->prev=NULL;
            }
        }
    }
}


void move_to_t2head(node *ptr){

    if(!t2head)
        t2head=t2tail=ptr;
    else{
        ptr->next=t2head;
        t2head->prev=ptr;
        t2head=ptr;
    }
    t2_num++;
    ptr->stat=T2;
}


void arc_put_into_cache(char *name,char *ip){

    node *ptr=get(name);
    int index,temp;
    if(NULL==ptr){
        if(cache_size==(t1_num+b1_num)){
            if(t1_num<cache_size){
                remove_b1tail();
                index=replace(0);
            }
            else
                index=remove_t1tail();
        }
        else{
            temp=t1_num+t2_num+b1_num+b2_num;
            if(temp>=cache_size){
                if(temp==2*cache_size)
                    remove_b2tail();
                index=replace(0);
            }
            else
                index=cache_num++;
        }
        ptr=ehead;
        if(ehead==etail)
            ehead=etail=NULL;
        else{
            ehead=ehead->next;
            ptr->next=NULL;
        }
        cache[index]=ip;
        ptr->cache_index=index;
        temp=put(name,ptr);
        ptr->hash_index=temp;
        move_to_t1head(ptr);
    }
    else{
        if(B1==ptr->stat){
            temp=b2_num/b1_num;
            temp=(temp>1?temp:1);
            p=p+temp;
            p=(p>cache_size?cache_size:p);
            index=replace(1);
        }
        else{
            if(ptr->stat!=B2){
                printf("some error happened in arc,exit...\n");
                return;
            }
            temp=b1_num/b2_num;
            temp=(temp>1?temp:1);
            p=p-temp;
            p=(p<0?0:p);
            index=replace(2);
        }
        cache[index]=ip;
        ptr->cache_index=index;
        extract_the_node(ptr);
        move_to_t2head(ptr);
    }
}


void remove_b1tail(){

    b1_num--;
    node *ptr=b1tail;
    int index=ptr->hash_index;
    removebykey(index);
    if(b1head==b1tail)
        b1head=b1tail=NULL;
    else{
        b1tail=b1tail->prev;
        b1tail->next=NULL;
        ptr->prev=NULL;
    }
    ptr->hash_index=-1;
    move_to_etail(ptr);
}


void move_to_etail(node *ptr){

    if(!etail)
        ehead=etail=ptr;
    else{
        etail->next=ptr;
        etail=ptr;
    }
}


void move_to_t1head(node *ptr){

    t1_num++;
    ptr->stat=T1;
    if(!t1head)
        t1head=t1tail=ptr;
    else{
        ptr->next=t1head;
        t1head->prev=ptr;
        t1head=ptr;
    }
}


void remove_b2tail(){

    b2_num--;
    node *ptr=b2tail;
    int index=ptr->hash_index;
    removebykey(index);
    if(b2head==b2tail)
        b2head=b2tail=NULL;
    else{
        b2tail=b2tail->prev;
        b2tail->next=NULL;
        ptr->prev=NULL;
    }
    ptr->hash_index=-1;
    move_to_etail(ptr);
}


int remove_t1tail(){

    t1_num--;
    node *ptr=t1tail;
    int index=ptr->hash_index;
    removebykey(index);
    t1tail=t1tail->prev;
    t1tail->next=NULL;
    ptr->prev=NULL;
    ptr->hash_index=-1;
    index=ptr->cache_index;
    ptr->cache_index=-1;
    move_to_etail(ptr);
    return index;
}



