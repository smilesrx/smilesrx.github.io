#include "linkList.h"

//��ʼ������
Lnode *Init_LinkList()
{
	//����ͷ�ڵ�
	Lnode *pHeader = malloc(sizeof(Lnode));
	if (pHeader==NULL)
	{
		return NULL;
	}

	//��ʼ��ͷ�ڵ�
	pHeader->data = 0;  //ͷ�ڵ㲻ά��������
	pHeader->next = NULL;

	//��¼β����ָ��
	Lnode *pTail = pHeader;
	//��¼�û����������
	int val = -1;
	while (1)
	{
		//���û��������ݣ���ʼ�������ڵ�
		printf("���������ݳ�ʼ�������������-1�����������\n");
		scanf("%d", &val);
		if (val == -1)
		{
			break;
		}

		//�������-1�������½ڵ㲢���뵽����
		Lnode *newNode = malloc(sizeof(Lnode));
		newNode->data = val;
		newNode->next = NULL;
		//����ָ���ָ��
		pTail->next = newNode;

		//����β���
		pTail = newNode;
	}
	return pHeader;
}

//��������
void Foreach_LinkList(Lnode *pHeader)
{
	if (pHeader == NULL)
	{
		return;
	}

	//����ָ��ǰ�ڵ��ָ��
	Lnode *pCurrent = pHeader->next;
	while (pCurrent != NULL)
	{
		printf("%d\n", pCurrent->data);
		pCurrent = pCurrent->next;
	}
}


//��������
void Insert_LinkList(Lnode *pHeader, int oldval, int val)
{
	if (pHeader==NULL)
	{
		return;
	}

	//������������ָ��
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

	//�����½ڵ�
	Lnode *NewNode = malloc(sizeof(Lnode));

	//��ʼ���½ڵ�
	NewNode->data = val;
	NewNode->next = NULL;

	//�޸�ָ��--����
	pPrve->next = NewNode;
	NewNode->next = pCurrent;
}


//ɾ������
void Delete_LinkList(Lnode *pHeader, int val)
{
	if (pHeader == NULL)
	{
		return;
	}

	//������������ָ��
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
		//�޸�ָ��--ɾ��
		pPrve->next = pCurrent->next;

		//�ͷŽڵ�
		free(pCurrent);
		pCurrent = NULL;
	}
}

//�������
void Clear_LinkList(Lnode *pHeader)
{
	if (pHeader == NULL)
	{
		return;
	}

	//����ָ��ǰ�ڵ��ָ��
	Lnode *pCurrent = pHeader->next;

	while (pCurrent != NULL)
	{
		//����ָ����һ�ڵ��ָ��
		Lnode *pNext = pCurrent->next;
		//�ͷŽڵ�
		free(pCurrent);
		pCurrent = pNext;
	}
	//���ͷ�ڵ�ָ���
	pHeader->next = NULL;
}

//��������
void Destory_LinkList(Lnode *pHeader)
{
	if (pHeader == NULL)
	{
		return;
	}

	//���������
	Clear_LinkList(pHeader);

	//�ͷ�ͷ���
	free(pHeader);
	pHeader = NULL;
}


//����ת
void Reverse_LinkList(Lnode *pHeader)
{
	if (pHeader == NULL)
	{
		return;
	}

	//������������ָ��
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