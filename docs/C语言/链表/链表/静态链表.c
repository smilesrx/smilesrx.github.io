#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
 
//结点的结构体
typedef struct LinkNode
{
	int data;   //数据域
	struct LinkNode *next;   //指针域
}Lnode;


void text01()
{
	//创建节点
	Lnode node1 = { 10,NULL };
	Lnode node2 = { 20,NULL };
	Lnode node3 = { 30,NULL };
	Lnode node4 = { 40,NULL };
	Lnode node5 = { 50,NULL };

	//建立关系
	node1.next = &node2;
	node2.next = &node3;
	node3.next = &node4;
	node4.next = &node5;

	//遍历链表
	Lnode *pCurrent = &node1;
	while (pCurrent)
	{
		printf("%d\n", pCurrent->data);
		pCurrent = pCurrent->next;
	}
}



int main(void)
{
	text01();

	system("pause");
	return EXIT_SUCCESS;
}
