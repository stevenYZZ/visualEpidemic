#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "linklistMY.h"

//新建一个头结点为空的RECORD链表
RECORD* newLinkRECORD(){
	RECORD *head=NULL;
	head=(RECORD*)malloc(sizeof(RECORD));
	head->prior=NULL;
	head->next=NULL;
	return head;
}

//找出给定RECORD链表给定日期所在结点并返回
RECORD* searchLinkRECORD(RECORD* head, char datex[]){
	RECORD *p=head;
	while(strcmp(p->date,datex)!=0){
		p=p->next;
		if(p==NULL)break;
	}
	return p;					//如果不存在给定日期结点就返回NULL（注意报错）
}

//向RECORD链表尾部增加一个结点
void addLinkRECORD(RECORD** tail,char datex[],int num[],int totalField){
	RECORD *p=NULL;									//新增结点
	int i;
	p=(RECORD*)malloc(sizeof(RECORD));
	p->next=NULL;
	strcpy(p->date,datex);							//输入日期
	for(i=0;i<=totalField;i++) p->number[i]=num[i];	//输入各字段人数
	p->prior=*tail;
	(*tail)->next=p;
	*tail=p;
}

//在RECORD链表尾部删除一个节点
void deleteLinkRECORD(RECORD* tail){
	if(tail->prior==NULL)return;		//如果链表只有一个结点就不作为
	else{								//否则删除尾结点
		tail->prior->next=NULL;			
		free(tail);
	}
}

//在给定RECORD链表结点中给定字段写入新值
void writeLinkRECORD(RECORD* p0,int fieldNum, int numx){
	if(p0==NULL)return;
	else p0->number[fieldNum]=numx;
}



//新建一个头结点为空的KEY链表
KEY* newLinkKEY(){
	KEY *head=NULL;
	head=(KEY*)malloc(sizeof(KEY));
	head->name[0]='\0';
	head->prior=NULL;
	head->next=NULL;

	return head;
}
//向KEY链表尾部增加一个结点
void addLinkKEY(KEY** tail,char *namex){
	KEY* p=NULL;
	p=(KEY*)malloc(sizeof(KEY));
	p->next=NULL;
	strcpy(p->name,namex);					//输入字段名
	p->prior=*tail;
	(*tail)->next=p;
	*tail=p;
}

//在KEY链表尾部删除一个节点
void deleteLinkKEY(KEY* tail){
	if(tail->prior==NULL)return;		//如果链表只有一个结点就不作为
	else{								//否则删除尾结点
		tail->prior->next=NULL;			
		free(tail);
	}
}

//得到给定结点在链表中位置
int getFieldNum(KEY *p0){
	KEY *p=p0;
	int count=0;
	while(p->prior!=NULL){
		p=p->prior;
		count++;
	}
	return count;
}