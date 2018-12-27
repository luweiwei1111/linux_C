#include <stdio.h>
#include <stdlib.h>

#define N 19
#define M 4
typedef struct monkey{
	int num;
	struct monkey *next;
} Monkey, *LINK;

/*
M只猴子选大王，选举办法如下：
所有猴子按1...M编号围坐一圈，从第1号开始按顺序1,2...N报数，报到N的猴子退出到圈外，
再从下一个猴子开始继续1,2...N报数，报到N的猴子退出圈外，如此循环，直至圈内只剩下一只猴子时就说大王。
给定M和最后出圈的者的编号S，求最小的N

##使用C语言的双向链表
 */

int main()
{
    LINK head, tail,current;
    printf("sizeof(Monkey)=%ld\n", sizeof(Monkey));
    head = tail = current = (LINK)malloc(sizeof(Monkey));
    //head用于保存首地址
    //tail用于保存尾地址
    //current用于保存当前地址
    
    int i = 1;
    for (i=1; i< N; i++)
    {
    	current = (LINK)malloc(sizeof(Monkey));
    	tail->next = current;
    	tail = current;
    }

    //首位相连
    current->next = head;

    //遍历链表
    //获取首节地址
    current = head;
    for (i=1; i<= N; i++)
    {
    	current->num = i;
    	printf("%d号猴子:%d\n", current->num, current->num);
    	current = current->next;
    }

    current = head;
    i = 0;
    while(1)
    {
    	i++;
    	printf("%d号猴子报:%d\n",current->num,i);

    	if(current->next == current)
    	{
    		//首节点的下一个节点指向自己，while退出
    		break;
    	}

    	if(i==M)
    	{
    		//数到m则需要删除当前节点
    		i = 0;
    		printf("%d号猴被淘汰\n",current->num);
			printf("\n");
    		tail->next = current->next;
    		current = tail->next;
    		continue;
    	}
    	else
    	{
    		if(i == M -1)
    		{
    			//保存删除节点的前一个节点
    			tail = current;
    		}
    		current = current->next;
    	}
    }

    printf("胜出:%d\n",current->num);//最后剩下的结点就是获胜的结点

}