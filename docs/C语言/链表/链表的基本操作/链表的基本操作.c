#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include "linkList.h"

void test01()
{
	//初始化链表
	Lnode *pHeader = Init_LinkList();

	//遍历链表
	printf("遍历链表结果为：\n");
	Foreach_LinkList(pHeader);

	//插入链表
	Insert_LinkList(pHeader, 20, 50);
	printf("插入链表后，遍历链表结果为：\n");
	Foreach_LinkList(pHeader);

	//链表反转
	Reverse_LinkList(pHeader);
	printf("反转链表后，遍历链表结果为：\n");
	Foreach_LinkList(pHeader);


	//删除链表
	Delete_LinkList(pHeader, 20);
	printf("删除链表后，遍历链表结果为：\n");
	Foreach_LinkList(pHeader);
	
	//清空链表
	Clear_LinkList(pHeader);
	printf("删除链表后，遍历链表结果为：\n");
	Foreach_LinkList(pHeader);

	//销毁链表
	Destory_LinkList(pHeader);
	pHeader = NULL;

}

int main(void)
{
	test01();


	system("pause");
	return EXIT_SUCCESS;
}
