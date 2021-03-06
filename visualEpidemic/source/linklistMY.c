#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//本程序自带头文件
#include "linklistMY.h"

//新建一个头结点为空的RECORD链表
RECORD *newLinkRECORD(){
	RECORD *head = NULL;
	head = (RECORD*)malloc(sizeof(RECORD));
	if (head != NULL){
		head->prior = NULL;
		head->next = NULL;
		return head;
	}
}

//找出给定RECORD链表给定日期所在结点并返回
RECORD *searchLinkRECORD(RECORD *head, char datex[]){
	RECORD *p = head;
	while (strcmp(p->date, datex) != 0){
		p = p->next;
		if (p == NULL){
			break;
		}
	}
	return p;					//如果不存在给定日期结点就返回NULL（注意报错）
}

//向RECORD链表尾部增加一个结点
void addLinkRECORD(RECORD **tail, char datex[], int num[], int totalField){
	RECORD *p = NULL;									//新增结点
	int i;
	p = (RECORD*)malloc(sizeof(RECORD));
	if (p != NULL){
		p->next = NULL;
		strcpy(p->date, datex);							//输入日期
		for (i=0; i<=totalField; i++){
			p->number[i]=num[i];	//输入各字段人数
		}
		p->prior = *tail;
		(*tail)->next = p;
		*tail = p;
	}

}

//在RECORD链表尾部删除一个节点
void deleteLinkRECORD(RECORD *tail){
	if (tail->prior == NULL){			//如果链表只有一个结点就不作为
		return;
	}
	else {								//否则删除尾结点
		tail->prior->next = NULL;			
		free(tail);
	}
}

//在给定RECORD链表结点中给定字段写入新值
void writeLinkRECORD(RECORD *p0, int fieldNum, int numx){
	if (p0 == NULL){
		return;
	}
	else {
		p0->number[fieldNum] = numx;
	}
}



//新建一个头结点为空的KEY链表
KEY *newLinkKEY(){
	KEY *head = NULL;
	head = (KEY*)malloc(sizeof(KEY));
	if (head != NULL){
		head->name[0] = '\0';
		head->prior = NULL;
		head->next = NULL;
		return head;
	}
}
//向KEY链表尾部增加一个结点
void addLinkKEY(KEY **tail, char *namex){
	KEY *p = NULL;
	p = (KEY*)malloc(sizeof(KEY));
	if (p != NULL){
		p->next = NULL;
		strcpy(p->name, namex);					//输入字段名
		p->prior = *tail;
		(*tail)->next = p;
		*tail = p;
	}
}

//在KEY链表尾部删除一个节点
void deleteLinkKEY(KEY *p){
	if (p->prior == NULL && p->next == NULL){		//如果链表只有一个结点就不作为
		return;
	}
	else if (p->prior == NULL){						//如果是头结点
		p->next->prior = NULL;			
		free(p);
	}
	else if (p->next == NULL){						//如果是尾结点
		p->prior->next = NULL;			
		free(p);
	}
	else{											//如果是中间结点
		p->next->prior = p->prior;
		p->prior->next = p->next;
		free(p);
	}
}

//得到给定结点在链表中位置
int getFieldNum(KEY *p0){
	KEY *p = p0;
	int count = 0;
	while (p->prior != NULL){
		p = p->prior;
		count++;
	}
	return count;
}