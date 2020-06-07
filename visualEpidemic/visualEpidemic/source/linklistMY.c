#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "linklistMY.h"

RECORD* newLinkRECORD();
RECORD* addLinkRECORD(RECORD* head, RECORD* tail,char *datex,int num1,int num2);

KEY* newLinkKEY();
KEY* addLinkKEY(KEY* head, KEY* tail,char *namex);

RECORD* newLinkRECORD(){
	RECORD *head=NULL;
	head=(RECORD*)malloc(sizeof(RECORD));
	head->prior=NULL;
	head->next=NULL;

	return head;
}

RECORD* addLinkRECORD(RECORD* head, RECORD* tail,char *datex,int num1,int num2,int num3){
	RECORD* p=NULL;

	p=(RECORD*)malloc(sizeof(RECORD));
	p->next=NULL;
	strcpy(p->date,datex);
	p->number[0]=num1;
	p->number[1]=num2;
	p->number[2]=num3;
	p->prior=tail;
	tail->next=p;

	return head;
}

KEY* newLinkKEY(){
	KEY *head=NULL;
	head=(KEY*)malloc(sizeof(KEY));
	head->name[0]='\0';
	head->prior=NULL;
	head->next=NULL;

	return head;
}

KEY* addLinkKEY(KEY* head, KEY* tail,char *namex){
	KEY* p=NULL;

	p=(KEY*)malloc(sizeof(KEY));
	p->next=NULL;
	strcpy(p->name,namex);


	p->prior=tail;
	tail->next=p;

	return head;
}

//int main(void){
//	RECORD *rpHead=NULL,*rpTail=NULL;
//	RECORD* temp=NULL;
//	char datex[10];
//	int num1,num2,i=0;
//
//	rpHead=newLinkRECORD();
//
//	//总共录入1+3=4条
//	scanf("%s %d %d",datex,&num1,&num2);
//	strcpy(rpHead->date,datex);
//	rpHead->number[0]=num1;
//	rpHead->number[1]=num2;
//	rpTail=rpHead;
//
//	for(i=0;i<=2;i++){
//		scanf("%s %d %d",datex,&num1,&num2);
//		rpHead=addLinkRECORD(rpHead,rpTail,datex,num1,num2);
//		rpTail=rpTail->next;
//	}
//	
//	temp=rpHead;
//	while(temp->next!=NULL){
//		printf("%s  %d  %d\n",temp->date,temp->number[0],temp->number[1]);
//		temp=temp->next;
//	}
//	printf("%s  %d  %d\n",temp->date,temp->number[0],temp->number[1]);
//	return 0;
//}