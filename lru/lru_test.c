#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include"lru.c"


void main(){
    hash_init(1000);
    init(1000);
    int i=0;
    int sum=0;
    int j=0;
    char c;
    srand((unsigned)time(NULL));
    char *name=(char *)malloc(15);
    char *ip=(char *)malloc(15);
    for(;i<100000;i++){
        memset(name,0,15);
        memset(ip,0,15);
        j=0;
        for(;j<3;j++){
            c=(rand()%(122-97))+97;
            char arr[2]={c,'\0'};
            strcat(name,arr);
        }
        printf("----->name:%s\n",name);
        ip=lru_search_cache(name);
        printf("----->ip:%s\n",ip);
        if(!ip){
            ip=(char *)malloc(15);
            printf("ip not exist\n");
            j=0;
            for(;j<2;j++){
            c=(rand()%(122-97))+97;
            char arr[2]={c,'\0'};
            strcat(ip,arr);
            }
            printf("----->new ip:%s\n",ip);
            lru_put_into_cache(name,ip);
        }
        else{ 
            printf("------------->i found it\n");
            sum++;
        }
    }   
    printf("the sum of hits during 100000 accesses is:%d\n",sum);
}


