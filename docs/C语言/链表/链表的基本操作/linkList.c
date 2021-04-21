#include "linkList.h"

//初始化链表
Lnode *Init_LinkList()
{
	//创建头节点
	Lnode *pHeader = malloc(sizeof(Lnode));
	if (pHeader==NULL)
	{
		return NULL;
	}

	//初始化头节点
	pHeader->data = 0;  //头节点不维护数据域
	pHeader->next = NULL;

	//记录尾结点的指针
	Lnode *pTail = pHeader;
	//记录用户输入的数据
	int val = -1;
	while (1)
	{
		//让用户输入数据，初始化几个节点
		printf("请输入数据初始化链表，如果输入-1代表结束结束\n");
		scanf("%d", &val);
		if (val == -1)
		{
			break;
		}

		//如果不是-1，建立新节点并插入到链表
		Lnode *newNode = malloc(sizeof(Lnode));
		newNode->data = val;
		newNode->next = NULL;
		//更改指针的指向
		pTail->next = newNode;

		//更新尾结点
		pTail = newNode;
	}
	return pHeader;
}

//遍历链表
void Foreach_LinkList(Lnode *pHeader)
{
	if (pHeader == NULL)
	{
		return;
	}

	//创建指向当前节点的指针
	Lnode *pCurrent = pHeader->next;
	while (pCurrent != NULL)
	{
		printf("%d\n", pCurrent->data);
		pCurrent = pCurrent->next;
	}
}


//插入链表
void Insert_LinkList(Lnode *pHeader, int oldval, int val)
{
	if (pHeader==NULL)
	{
		return;
	}

	//创建两个辅助指针
	Lnode *pPrve = pHeader;
	Lnode *pCurrent = pHeader->next;

	while (pCurrent != NULL)
	{
		if (pCurrent->data == oldval)
		{
			break;
		}
		pPrve = pCurrent;
		pCurrent = pCurrent->next;
	}

	//创建新节点
	Lnode *NewNode = malloc(sizeof(Lnode));

	//初始化新节点
	NewNode->data = val;
	NewNode->next = NULL;

	//修改指针--插入
	pPrve->next = NewNode;
	NewNode->next = pCurrent;
}


//删除链表
void Delete_LinkList(Lnode *pHeader, int val)
{
	if (pHeader == NULL)
	{
		return;
	}

	//创建两个辅助指针
	Lnode *pPrve = pHeader;
	Lnode *pCurrent = pHeader->next;

	while (pCurrent != NULL)
	{
		if (pCurrent->data == val)
		{
			break;
		}
		pPrve = pCurrent;
		pCurrent = pCurrent->next;
	}

	if (pCurrent != NULL)
	{
		//修改指针--删除
		pPrve->next = pCurrent->next;

		//释放节点
		free(pCurrent);
		pCurrent = NULL;
	}
}

//清空链表
void Clear_LinkList(Lnode *pHeader)
{
	if (pHeader == NULL)
	{
		return;
	}

	//创建指向当前节点的指针
	Lnode *pCurrent = pHeader->next;

	while (pCurrent != NULL)
	{
		//创建指向下一节点的指针
		Lnode *pNext = pCurrent->next;
		//释放节点
		free(pCurrent);
		pCurrent = pNext;
	}
	//最后头节点指向空
	pHeader->next = NULL;
}

//销毁链表
void Destory_LinkList(Lnode *pHeader)
{
	if (pHeader == NULL)
	{
		return;
	}

	//先清空链表
	Clear_LinkList(pHeader);

	//释放头结点
	free(pHeader);
	pHeader = NULL;
}


//链表反转
void Reverse_LinkList(Lnode *pHeader)
{
	if (pHeader == NULL)
	{
		return;
	}

	//定义三个辅助指针
	Lnode *pPrve = NULL;
	Lnode *pCurrent = pHeader->next;
	Lnode *pNext = NULL;

	while (pCurrent != NULL)
	{
		pNext = pCurrent->next;
		pCurrent->next = pPrve;

		pPrve = pCurrent;
		pCurrent = pNext;
	}
	pHeader->next = pPrve;
}