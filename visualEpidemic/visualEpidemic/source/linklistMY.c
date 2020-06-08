#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "linklistMY.h"

//�½�һ��ͷ���Ϊ�յ�RECORD����
RECORD* newLinkRECORD(){
	RECORD *head=NULL;
	head=(RECORD*)malloc(sizeof(RECORD));
	head->prior=NULL;
	head->next=NULL;
	return head;
}

//�ҳ�����RECORD��������������ڽ�㲢����
RECORD* searchLinkRECORD(RECORD* head, char datex[]){
	RECORD *p=head;
	while(strcmp(p->date,datex)!=0){
		p=p->next;
		if(p==NULL)break;
	}
	return p;					//��������ڸ������ڽ��ͷ���NULL��ע�ⱨ��
}

//��RECORD����β������һ�����
void addLinkRECORD(RECORD** tail,char datex[],int num[],int totalField){
	RECORD *p=NULL;									//�������
	int i;
	p=(RECORD*)malloc(sizeof(RECORD));
	p->next=NULL;
	strcpy(p->date,datex);							//��������
	for(i=0;i<=totalField;i++) p->number[i]=num[i];	//������ֶ�����
	p->prior=*tail;
	(*tail)->next=p;
	*tail=p;
}

//��RECORD����β��ɾ��һ���ڵ�
void deleteLinkRECORD(RECORD* tail){
	if(tail->prior==NULL)return;		//�������ֻ��һ�����Ͳ���Ϊ
	else{								//����ɾ��β���
		tail->prior->next=NULL;			
		free(tail);
	}
}

//�ڸ���RECORD�������и����ֶ�д����ֵ
void writeLinkRECORD(RECORD* p0,int fieldNum, int numx){
	if(p0==NULL)return;
	else p0->number[fieldNum]=numx;
}



//�½�һ��ͷ���Ϊ�յ�KEY����
KEY* newLinkKEY(){
	KEY *head=NULL;
	head=(KEY*)malloc(sizeof(KEY));
	head->name[0]='\0';
	head->prior=NULL;
	head->next=NULL;

	return head;
}
//��KEY����β������һ�����
void addLinkKEY(KEY** tail,char *namex){
	KEY* p=NULL;
	p=(KEY*)malloc(sizeof(KEY));
	p->next=NULL;
	strcpy(p->name,namex);					//�����ֶ���
	p->prior=*tail;
	(*tail)->next=p;
	*tail=p;
}

//��KEY����β��ɾ��һ���ڵ�
void deleteLinkKEY(KEY* tail){
	if(tail->prior==NULL)return;		//�������ֻ��һ�����Ͳ���Ϊ
	else{								//����ɾ��β���
		tail->prior->next=NULL;			
		free(tail);
	}
}

//�õ����������������λ��
int getFieldNum(KEY *p0){
	KEY *p=p0;
	int count=0;
	while(p->prior!=NULL){
		p=p->prior;
		count++;
	}
	return count;
}