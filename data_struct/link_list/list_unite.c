#include<stdio.h>
#include<stdlib.h>


typedef struct node* List;
struct node
{
    int data;
    struct node *next;
};

List Read()
{
    int x;
    List q, r, head;
    scanf("%d", &x);
    head = (List)malloc(sizeof(struct node));
    r = head;
    while(x != -1)
    {
        q = (List)malloc(sizeof(struct node));
        q->data = x;
        r->next = q;
        r = q;
        scanf("%d", &x);
    }
    r->next = NULL;
    return head;
}

List LinkList(List La, List Lb)
{
    List Lc, pa, pb, pc, ptr;
    pa = La->next;
    pb = Lb->next;
    Lc = La;
    pc = La;
    while(pa != NULL && pb != NULL)
    {
        if(pa->data < pb->data)
        {
            pc->next = pa;
            pc = pa;
            pa = pa->next;
        }
        else if(pa->data > pb->data)
        {
            pc->next = pb;
            pc = pb;
            pb = pb->next;
        }
        else
        {
            pc->next = pa;
            pc = pa;
            pa = pa->next;
            //相等则删除掉listb中数据
            ptr = pb;
            pb = pb->next;
            free(ptr);
        }
    }
    if(pa != NULL)
        pc->next = pa;
    else
        pc->next = pb;
    return (Lc);
}

void Print(List s)
{
    int k;
    k = 0;
    s = s->next;
    if(s == NULL)
    {
        printf("NULL");
        return ;
    }
    while(s != NULL)
    {
        if(k != 0)
            printf(" ");
        printf("%d", s->data);
        s = s->next;
        k++;
    }
    printf("\n");
    return ;
}

int main()
{
    int x;
    List s1, s2, s3;
    s1 = Read();
    s2 = Read();
    s3 = LinkList(s1, s2);
    Print(s3);
    return 0;
}