#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
 
//���Ľṹ��
typedef struct LinkNode
{
	int data;   //������
	struct LinkNode *next;   //ָ����
}Lnode;


void text01()
{
	//�����ڵ�
	Lnode node1 = { 10,NULL };
	Lnode node2 = { 20,NULL };
	Lnode node3 = { 30,NULL };
	Lnode node4 = { 40,NULL };
	Lnode node5 = { 50,NULL };

	//������ϵ
	node1.next = &node2;
	node2.next = &node3;
	node3.next = &node4;
	node4.next = &node5;

	//��������
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
