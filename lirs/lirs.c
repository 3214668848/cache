#include"hash.c"




void init(int size){

    lir_max=size*L_WEIGHT;
    hir_max=size*H_WEIGHT;
    cache_num=0;
    lir_len=0;
    hir_len=0;

    int i=0;
    for(;i<NODES_MAX;i++){
        node *p=(node*)malloc(sizeof(node));
        p->cache_index=-1;
        if(ehead){
            p->snext=ehead;
            ehead=p;
        }
        else
            ehead=etail=p;
    }

    cache=(char **)malloc(size*sizeof(char *));
    for(i=0;i<size;i++)
        cache[i]=(char *)malloc(15*sizeof(char));
}


char* lirs_search_cache(char *name){

    node *result=get(name);
    if(NULL==result || -1==result->cache_index)
        return NULL;
    int index=result->cache_index;
    char *ip=cache[index];
    if(LIR==result->stat){
        extract_from_s(result);
        move_to_shead(result);
        if(HIR==stail->stat)
            stack_pruning();
    }
    else{
        if(1==result->in_s){
            promote_the_node(result,ip);
            demote_stail();
            if(HIR==stail->stat)
                stack_pruning();
        }
        else{
            move_to_shead(result);
            remove_from_q(result);
            move_to_qtail(result);
        }
    }
    return ip;
}


void lirs_put_into_cache(char *name,char *ip){

    node *ptr=get(name);
    if(ptr!=NULL){
        promote_the_node(ptr,ip);
        demote_stail();
        if(HIR==stail->stat)
            stack_pruning();
    }
    else{
        if(ehead!=NULL){
            ptr=ehead;
            if(ehead==etail)
                ehead=etail=NULL;
            else{
                ehead=ehead->snext;
                ptr->snext=NULL;
            }
        }
        else{
            ptr=find_the_victim();
            int temp=ptr->hash_index;
            removebykey(temp);
        }
        modify_and_add(ptr,name,ip);  
    }
}


void extract_from_s(node *p){

    if(p==shead)
        return;
    else{
        if(p==stail){
            stail=stail->sprev;
            stail->snext=NULL;
            p->sprev=NULL;
        }
        else{
            node *before=p->sprev;
            node *after=p->snext;
            before->snext=after;
            after->sprev=before;
            p->sprev=p->snext=NULL;
        }
    }
}


void move_to_shead(node *p){

    if(p==shead)
        return;
    else{
        if(NULL==shead)
            shead=stail=p;
        else{
            p->snext=shead;
            shead->sprev=p;
            shead=p;
        }       
    }
    p->in_s=1;
}


node* find_the_victim(){

    node * p=stail;
    while(p->stat!=HIR)
        p=p->sprev;
    extract_from_s(p);
    return p;
}


void replace_cache(node *p,char *ip){

    node *q=cut_qhead();
    int index=q->cache_index;
    cache[index]=ip;
    p->cache_index=index;
    q->cache_index=-1;
}


void move_to_qtail(node *p){

    if(NULL==qtail)
        qtail=qhead=p;
    else{
        qtail->qnext=p;
        p->qprev=qtail;
        qtail=p;
    }
}


void promote_the_node(node *p,char *ip){

    extract_from_s(p);
    move_to_shead(p);
    if(-1==p->cache_index)
        replace_cache(p,ip);
    else
        remove_from_q(p);
    p->stat=LIR;
}


void demote_stail(){

    node *p=stail;
    extract_from_s(p);
    p->stat=HIR;
    p->in_s=0;
    move_to_qtail(p);
}


void remove_from_q(node *p){

    if(p==qhead)
        p=cut_qhead();
    else{
        if(p==qtail){
            qtail=qtail->qprev;
            qtail->qnext=NULL;
            p->qprev=NULL;
        }
        else{
            node *before=p->qprev;
            node *after=p->qnext;
            before->qnext=after;
            after->qprev=before;
            p->qprev=p->qnext=NULL;
        }
    }
}


node* cut_qhead(){

    node *p=qhead;
    qhead=qhead->qnext;
    p->qnext=qhead->qprev=NULL;
    if(0==p->in_s){
        int temp=p->hash_index;
        removebykey(temp);
        move_to_etail(p);
    }
    return p;
}


void move_to_etail(node *p){

    if(NULL==ehead)
        ehead=etail=p;
    else{
        etail->snext=p;
        etail=p;
    }
}


void stack_pruning(){

    node *before=stail;
    node *after=NULL;
    int temp=0;
    while(HIR==before->stat){
        after=before;
        before=before->sprev;
        before->snext=NULL;
        after->sprev=NULL;
        after->in_s=0;
        if(-1==after->cache_index){
            temp=after->hash_index;
            removebykey(temp);
            move_to_etail(after);
        }
    }
    stail=before;
}


void modify_and_add(node *p,char *name,char *ip){

    int h_index=put(name,p);
    p->hash_index=h_index;
    move_to_shead(p);
    if(hir_len>=hir_max){
        replace_cache(p,ip);
        move_to_qtail(p);
        p->stat=HIR;
    }
    else{
        if(lir_len>=lir_max){
            hir_len++;
            p->stat=HIR;
            move_to_qtail(p);
        }
        else{
            lir_len++;
            p->stat=LIR;
        }
        cache[cache_num]=ip;
        p->cache_index=cache_num++;
    }
}



