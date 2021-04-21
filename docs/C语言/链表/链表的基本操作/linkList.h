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
	int data;   //数据域
	struct LinkNode *next;   //指针域
}Lnode;

//初始化链表
Lnode *Init_LinkList();

//遍历链表
void Foreach_LinkList(Lnode *pHeader);

//插入链表
void Insert_LinkList(Lnode *pHeader,int oldval,int val);

//链表反转
void Reverse_LinkList(Lnode *Pheader);

//删除链表
void Delete_LinkList(Lnode *pHeader, int pos);

//清空链表
void Clear_LinkList(Lnode *pHeader);

//销毁链表
void Destory_LinkList(Lnode *pHeader);