#pragma once
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

//��ʼ������
Lnode *Init_LinkList();

//��������
void Foreach_LinkList(Lnode *pHeader);

//��������
void Insert_LinkList(Lnode *pHeader,int oldval,int val);

//����ת
void Reverse_LinkList(Lnode *Pheader);

//ɾ������
void Delete_LinkList(Lnode *pHeader, int pos);

//�������
void Clear_LinkList(Lnode *pHeader);

//��������
void Destory_LinkList(Lnode *pHeader);