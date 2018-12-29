#include<stdio.h>
#include<stdlib.h>
typedef struct BiTNode
{
    char data;
    struct BiTNode *lchild,*rchild;
}BiTNode,*BiTree;

//二叉树的先序遍历
void PreOrderTraverse(BiTree T)
{
    if(T==NULL)
        return ;
    printf("%d ",T->data);
    PreOrderTraverse(T->lchild);
    PreOrderTraverse(T->rchild);
}

//二叉树的中序遍历
void InOrderTraverse(BiTree T)
{
   if(T==NULL)
       return ;
   InOrderTraverse(T->lchild);
    printf("%c ",T->data);
   InOrderTraverse(T->rchild);
}

//后序遍历
void PostOrderTraverse(BiTree T)
{
    if(T==NULL)
        return;
    PostOrderTraverse(T->lchild);
    PostOrderTraverse(T->rchild);
    printf("%c ",T->data);
}

//创建二叉树
void CreateBiTree(BiTree *T)
{
    char ch;
    scanf("%c ",&ch);
    if(ch == '#')
    {
        *T=NULL;
        //return ;
    }
    else
    {
        if(!*T)
            exit(-1);
        *T=(BiTree  )malloc(sizeof(BiTNode));
        (*T)->data=ch;
        CreateBiTree(&(*T)->lchild);
        CreateBiTree(&(*T)->rchild);
    }
}

int main()
{
    BiTree T;
    CreateBiTree(&T);
    PreOrderTraverse (T);
    InOrderTraverse(T);
    PostOrderTraverse(T);
    return 0;
}