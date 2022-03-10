#include <stdio.h>
#include <stdlib.h>

typedef struct Node* Polynomial;//多项式
struct Node {
    int coef;//系数
    int expon;//指数
    Polynomial next;// 多项式下一个子项
};

void Attach(int c, int e, Polynomial* pRear) {
    //加项，插入一新节点
    Polynomial P;
    P = (Polynomial)malloc(sizeof(struct Node));
    P->coef = c;//系数c
    P->expon = e;//指数e
    P->next = NULL;
    (*pRear)->next = P; // P指向的新结点插入到当前尾项的后面
    *pRear = P;   // pRear指针移动到P结点
}

int Compare(int a,int b){
    // 比大小
    int c;
    if (a>b) c=1;
    else if (a<b) c=-1;
    else c=0;
    return c;
}

Polynomial Read() {
    // 读取规格化输入的内容，整理为链表
    Polynomial P, Rear, t;
    int c, e, N;

    scanf("%d", &N);//输入项数
    P = (Polynomial)malloc(sizeof(struct Node));// 链表头空结点
    P->next = NULL;
    Rear = P;
    while (N--) {
        scanf("%d %d", &c, &e);//输入系数，指数
        Attach(c, e, &Rear);   // 将当前项插入多项式尾部
    }
    t = P; P = P->next; free(t); // 删除临时生成的头结点
    return P;
}

Polynomial Add(Polynomial P1,Polynomial P2){
    Polynomial front,rear,temp;
    int sum;
    rear=(Polynomial)malloc(sizeof(struct Node));
    front=rear;//front 用来记录链表头结点
    while(P1&&P2){

        switch(Compare(P1->expon,P2->expon)){
            // 比较1 2 的指数
            case 1:
                Attach(P1->coef,P1->expon,&rear);
                P1=P1->next;
                break;
            case -1:
                Attach(P2->coef,P2->expon,&rear);
                P2=P2->next;
                break;
            case 0:
                // 相同加系数
                sum=P1->coef+P2->coef;
                if (sum){
                    Attach(sum,P1->expon,&rear);
                }
                P1=P1->next;
                P2=P2->next;
                break;

        }
    }
    for(;P1;P1=P1->next) Attach(P1->coef,P1->expon,&rear);
    for(;P2;P2=P2->next) Attach(P2->coef,P2->expon,&rear);
    rear->next=NULL;
    temp=front;
    front=front->next;
    free(temp);
    return front;
}

Polynomial Div(Polynomial P1,Polynomial P2){
    Polynomial front,rear,temp;
    int sum;
    rear=(Polynomial)malloc(sizeof(struct Node));
    front=rear;//front 用来记录链表头结点
    while(P1&&P2){

        switch(Compare(P1->expon,P2->expon)){
            // 比较1 2 的指数
            case 1:
                Attach(P1->coef,P1->expon,&rear);
                P1=P1->next;
                break;
            case -1:
                Attach(P2->coef,P2->expon,&rear);
                P2=P2->next;
                break;
            case 0:
                // 相同加系数
                sum=P1->coef-P2->coef;
                if (sum){
                    Attach(sum,P1->expon,&rear);
                }
                P1=P1->next;
                P2=P2->next;
                break;

        }
    }
    for(;P1;P1=P1->next) Attach(P1->coef,P1->expon,&rear);
    for(;P2;P2=P2->next) Attach(P2->coef,P2->expon,&rear);
    rear->next=NULL;
    temp=front;
    front=front->next;
    free(temp);
    return front;
}

Polynomial Mult(Polynomial P1, Polynomial P2)
{
    // 多项式相乘
    Polynomial P, Rear, t1, t2, t;//P结果多项式,Rear结果多项式尾项,t1多项式1，t2多项式2
    int c, e;
    if (!P1 || !P2)
        return NULL;// 判断如果有一个为空，则返回空;

    t1 = P1;
    t2 = P2;
    P = (Polynomial)malloc(sizeof(struct Node));
    P->next = NULL;
    Rear = P;//P始终指向结果多项式开头，rear指向尾部
    // p1的第一项和所有的t2相乘
    while (t2)//
    {
        Attach(t1->coef * t2->coef, t1->expon + t2->expon, &Rear);
        // 系数相乘，阶数相加，加入到结果多项式尾部
        t2 = t2->next;
    }//已经计算出结果多项式的一部分下面的都是在这个上增补
    t1 = t1->next;// p1的第一项已经乘好，后移到第二项
    while (t1) //
    {
        t2 = P2;//t2重置为p2开头
        Rear = P;
        while (t2)
        {
            e = t1->expon + t2->expon;//指数相加
            c = t1->coef * t2->coef;//系数相乘
            while (Rear->next && Rear->next->expon > e)
            {
                Rear = Rear->next;//当相乘得到的 e小于多项式指数时，Rear指针往后移，依次比较指数直到保持指数递减排列
            }
            if (Rear->next && Rear->next->expon == e)//当相乘得到的e等于多项式指数时，合并同类项
            {
                if (Rear->next->coef + c)
                    Rear->next->coef += c;//如果当前多项式系数的和不为 0，加上新的系数后重新赋值
                else
                {
                    t = Rear->next;
                    Rear->next = t->next;//当前项合并同类项之后系数为零时，删除这项
                    free(t);//如果多项式的系数为0，则 Rear指针指向下一个项，这个结点释放
                }
            }
            else
            {
                t = (Polynomial)malloc(sizeof(struct Node));//新得的项指数大于多项式指数时，新建一个临时项t复制当前项的指数和系数
                t->coef = c;//系数
                t->expon = e;//指数
                t->next = Rear->next;
                Rear->next = t;//把t加入到结果多项式的尾部
                Rear = Rear->next;// 如果多项式没有到最后并且目前没有这一项，
                // 则分配内存空间，新建结点，并将结点插入链表，Rear指针指到新结点 t
            }
            t2 = t2->next;//t2后移一项
        }
        t1 = t1->next;//t1后移
    }
    t2 = P;
    P = P->next;//计算下一项
    free(t2);
    return P;
}

void PrintPoly(Polynomial P) {
    // 格式化输出
    int flag = 0;

    if (!P) {
        printf("0\n");
        return;
    }

    while (P) {
        if (!flag) flag = 1;
        else printf(" ");
        printf("(%d) x^%d +", P->coef, P->expon);
        P = P->next;
    }
    printf("\n");
}

int main()
{
    Polynomial P1, P2, PP, PS, PD;
    P1 = Read();
    P2 = Read();
    PS=Add(P1,P2);
    PrintPoly(PS);
    PD= Div(P1,P2);
    PrintPoly(PD);
    PP = Mult(P1, P2);
    PrintPoly(PP);

    return 0;
}