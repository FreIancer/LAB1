#include <stdio.h>
#include <stdlib.h>

/**
 *
 */

typedef struct Node *Polynomial;
struct Node {
    int coef;
    int expon;
    Polynomial next;
};

void Attach(int c,int e,Polynomial *pRear) {
    // 加项，插入一新节点
    Polynomial P;
    P=(Polynomial)malloc(sizeof(struct Node));
    P->coef=c;
    P->expon=e;
    P->next=NULL;
    (*pRear)->next=P; // P指向的新结点插入到当前尾项的后面
    *pRear=P;   // pRear指针移动到 P结点
}

int Compare(int a,int b){
    // a,b比大小
    int c;
    if (a>b) c=1;
    else if (a<b) c=-1;
    else c=0;
    return c;
}

Polynomial ReadPoly(){
    // 读取规格化输入的内容，整理为链表
    Polynomial P,Rear,t;
    int c,e,N;

    scanf("%d",&N);
    P=(Polynomial)malloc(sizeof(struct Node));// 链表头空结点
    P->next=NULL;
    Rear=P;
    while(N--){
        scanf("%d %d",&c,&e);
        Attach(c,e,&Rear);   // 将当前项插入多项式尾部
    }
    t=P;P=P->next;free(t); // 删除临时生成的头结点
    return P;
}

Polynomial Mult(Polynomial P1,Polynomial P2){
    // 多项式相乘
    Polynomial P,Rear,t1,t2,t;
    int c,e;
    if (!P1||!P2)
        return NULL;// 判断如果有一个为空，则返回空;

    t1=P1;t2=P2;
    P=(Polynomial)malloc(sizeof(struct Node));
    P->next=NULL;
    Rear=P;
    // p1的第一项和所有的t2相乘
    while(t2){
        Attach(t1->coef*t2->coef,t1->expon+t2->expon,&Rear);// 系数相乘，阶数相乘
        t2=t2->next;
    }
    t1=t1->next;// p1的第一项已经乘好，后移到第二项
    while (t1){
        t2=P2;
        Rear=P;
        while(t2){
            e=t1->expon+t2->expon;
            c=t1->coef*t2->coef;
            while(Rear->next && Rear->next->expon > e)
                Rear=Rear->next;//当相乘得到的 e小于多项式系数时，Rear指针往后移
            if(Rear->next && Rear->next->expon == e){
                if(Rear->next->coef + c)
                    Rear->next->coef+=c;//如果多项式系数的和不为 0，则用新的系数赋值
                else{
                    t=Rear->next;
                    Rear->next=t->next;
                    free(t);//如果多项式的系数为0，则 Rear指针指向下一个项，这个结点释放
                }
            }
            else{
                t=(Polynomial)malloc(sizeof(struct Node));
                t->coef=c;
                t->expon=e;
                t->next=Rear->next;
                Rear->next=t;
                Rear=Rear->next;// 如果多项式没有到最后并且目前没有这一项，
                                // 则分配内存空间，新建结点，并将结点插入链表，Rear指针指到新结点 t
            }
            t2=t2->next;
        }
        t1=t1->next;
    }
    t2=P;
    P=P->next;
    free(t2);
    return P;
}

void PrintPoly(Polynomial P){
    // 格式化输出
    int flag=0;

    if(!P) {
        printf("0 0\n");
        return;
    }

    while (P){
        if(!flag) flag=1;
        else printf(" ");
        printf("%d %d",P->coef,P->expon);
        P=P->next;
    }
    printf("\n");
}

int main()
{
    Polynomial P1,P2,PP;
    P1=ReadPoly();
    P2=ReadPoly();
    PP=Mult(P1,P2);
    PrintPoly(PP);

    return 0;
}