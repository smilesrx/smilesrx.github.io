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
	//��ʼ������
	Lnode *pHeader = Init_LinkList();

	//��������
	printf("����������Ϊ��\n");
	Foreach_LinkList(pHeader);

	//��������
	Insert_LinkList(pHeader, 20, 50);
	printf("��������󣬱���������Ϊ��\n");
	Foreach_LinkList(pHeader);

	//����ת
	Reverse_LinkList(pHeader);
	printf("��ת����󣬱���������Ϊ��\n");
	Foreach_LinkList(pHeader);


	//ɾ������
	Delete_LinkList(pHeader, 20);
	printf("ɾ������󣬱���������Ϊ��\n");
	Foreach_LinkList(pHeader);
	
	//�������
	Clear_LinkList(pHeader);
	printf("ɾ������󣬱���������Ϊ��\n");
	Foreach_LinkList(pHeader);

	//��������
	Destory_LinkList(pHeader);
	pHeader = NULL;

}

int main(void)
{
	test01();


	system("pause");
	return EXIT_SUCCESS;
}
