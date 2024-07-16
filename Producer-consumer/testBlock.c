/*************************************************************************
    > File Name: testBlock.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年07月16日 星期二 21时28分35秒
 ************************************************************************/

#include"BlockQ.h"

int main(){

    BlockQ* q = blockq_create();
    blockq_push(q,1);
    blockq_push(q,2);
    blockq_push(q,3);
    blockq_push(q,4);
    blockq_push(q,5);
    blockq_push(q,6);

    int result = blockq_peek(q);
    printf("%d \n",result);
    blockq_pop(q);

    result = blockq_peek(q);
    printf("%d \n",result);
    blockq_pop(q);

    result = blockq_peek(q);
    printf("%d \n",result);
    blockq_pop(q);

    result = blockq_peek(q);
    printf("%d \n",result);
    blockq_pop(q);
    
    result = blockq_peek(q);
    printf("%d \n",result);
    blockq_pop(q);

    result = blockq_peek(q);
    printf("%d \n",result);
    blockq_pop(q);
    return 0;
}
