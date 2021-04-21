#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <Windows.h>

typedef struct LinkNode
{
	int data;   //������
	struct LinkNode *next;   //ָ����
}Lnode;

void text01()
{
	//�����ڵ�
	Lnode *node1 = malloc(sizeof(Lnode));
	Lnode *node2 = malloc(sizeof(Lnode));
	Lnode *node3 = malloc(sizeof(Lnode));
	Lnode *node4 = malloc(sizeof(Lnode));
	Lnode *node5 = malloc(sizeof(Lnode));


	//��ʼ���ڵ�������
	node1->data = 100;
	node2->data = 200;
	node3->data = 300;
	node4->data = 400;
	node5->data = 500;

	//������ϵ
	node1->next = node2;
	node2->next = node3;
	node3->next = node4;
	node4->next = node5;
	node5->next = NULL;;

	//��������
	Lnode *pCurrent = node1;     //��ǰָ��
	while (pCurrent)
	{
		printf("%d\n", pCurrent->data);
		pCurrent = pCurrent->next;
	}

	free(node1);
	free(node2);
	free(node3);
	free(node4);
	free(node5);
	node1 = NULL;
	node2 = NULL;
	node3 = NULL;
	node4 = NULL;
	node5 = NULL;
}

int main(void)
{
	text01();

	system("pause");
	return EXIT_SUCCESS;
}
