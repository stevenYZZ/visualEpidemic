#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include <time.h>
#include "imgui.h"

//typedef struct record{
//    char date[10];                        //����
//    int number[10];                  //������������
//    struct record *prior, *next;    //����ָ��
//}RECORD;
//typedef struct key{
//    char name[20];                  //ÿ�������ֶ���
//    struct record *prior, *next;    //����ָ��
//}KEY; 

//ȫ�ֱ���
int globalEdit = 0; 




//���ļ����������Ӱ˺���


//�ж��Ƿ�Ϊ���꺯��
int ifRun(int n){
	if((n%4==0) && (n%100!=0) || (n%400==0))return 1;
	else return 0;
}
//���ظ����õı�׼����
//int����1��0��0��1����
void commonTF(int *a){
	if(*a==0)*a=1;
	else *a=0;

}





//������һ�������ַ�������
char *nextDate(char today[]){
	char year[4];char month[2];char day[2]; //char����������
	int y,m,d;			//int����������
	int mAdd=0;     //�·��Ƿ��λ
	
	//�õ�int����������
	strncpy(year, today, 4);y=atoi(year);
	strncpy(month, today+4, 2);m=atoi(month);
	strncpy(day, today+6, 2);d=atoi(day);

	//����
	if(d==28 && !ifRun(y) && m==2){
			d=0;
			mAdd++;
	}else if(d==29 && ifRun(y) && m==2){
			d=0;
			mAdd++;
	}else if(d==30 && (m==4 || m==6 || m==9 || m==11)){
			d=0;
			mAdd++;
	}else if(d==31 && m!=2 && m!=4 && m!=6 && m!=9 && m!=11){
			d=0;
			mAdd++;
	}else;
	d=d+1;
	if(m==12 && mAdd==1){
		y++;
		m=1;
	}else m=m+mAdd;

	//������ƴ�ӳ�char����
	sprintf(today,"%d%02d%02d",y,m,d);
	return today;
}



////������
///*
//void linklistEdit(){
// 	RECORD *head,*p;
// 	char date[10]; int number[10];
// 	int choice;
// 
// 	int size=sizeof(RECORD);
// 	scanf("%d",&choice); 
// 	do{     //(1.�½� 2������� 3ɾ����� 4�������� 5�������� 0exit) 
// 	       switch(choice){
//			case 1:head=recordCreat();break;
// 	       
//			case 2:scanf("%s%d%d%d%d",date,&number[1],&number[2],&number[3],&number[4]);// �ֶ��������ںͶ�Ӧ���� 
//	              p=(RECORD*)malloc(size);
//	             
//				  p->number[1]=number[1];p->number[2]=number[2];p->number[3]=number[3];p->number[4]=number[4];
//	            
//				 strcpy(p->date,date);
//				 
//				 head=recordAdd(head,p);
//				 break;
//		     
//		     case 3:scanf("%s",date);
//		           head=recordDelete(head,date);
//		           break;
//		     case 4:scanf("%s%d%d%d%d",date,&number[1],&number[2],&number[3],&number[4]);// �ֶ��������ںͶ�Ӧ���� 
//	              p=(RECORD*)malloc(size);
//	             
//				  p->number[1]=number[1];p->number[2]=number[2];p->number[3]=number[3];p->number[4]=number[4];
//	            
//				 strcpy(p->date,date);
//				  head=recordDelete(head,date);    //ɾ��Ҫ���ĵĽ��
//				    head=recordAdd(head,p);       //������ĺ�Ľ�� 
//		            break;
//			 case 5:scanf("%s",date);
//			        p=recordSearch(head,date);
//		            break;
//		     
//		     case 0:break;
//		                  
//			}
//			
//	             
//	             
//	 }while(choice!=0);
//	  }
//*/
//
////1������㺯��
////���ܣ��������б�����һ���������
//RECORD * recordAdd(RECORD *head,RECORD *add){
//	
//	RECORD *ptr,*ptr1,*ptr2;
//	
//	ptr2=head; ptr=add;  //*ptrָ���������µĽ��
//	
//	if(head==NULL){
//		head=ptr;head->next=NULL; //�²������Ϊͷ���
//		 }
//    else{              //ԭ����Ϊ��ʱ�Ĳ��� 
//    	while((strcmp(ptr->date,ptr2->date)>0)&&(ptr2->next!=NULL)){
//    		ptr1=ptr2;    //ptr1,ptr2������һ�����
//			ptr2=ptr2->next; 
//		}
//		if(strcmp(ptr->date,ptr2->date)<=0){//��ptr1��ptr2֮������½��
//		if(head==ptr2) head=ptr;
//		else ptr1->next=ptr;
//		ptr->next=ptr2; 
//		}
//		else{//�²������Ϊβ��� 
//			ptr2->next=ptr;ptr->next=NULL; 
//		}
//		
//	} 
//	return head;
//}
//
////2�½�������
////���ܣ��½�һ��ȷ�����ڷ�Χ���ֶ�����������
//RECORD * recordCreat(char headDate[],char tailDate[],int fieldNum){
//	RECORD *head,*p;
//	char *pDate = headDate;
//	int size=sizeof(RECORD);
//	int i;
//	head=NULL;
//
//while(strcmp(pDate,tailDate)){                 
//	p=(RECORD *)malloc(size);
//	for(i=0;i<fieldNum;i++)p->number[i]=0;
//	strcpy(p->date,nextDate(pDate));
//	head=recordAdd(head,p);
//	}
//	return head;
//	
//	
//} 
////3ɾ����㺯��
////���ܣ��ڸ����б�ɾ��һ���������
//RECORD * recordDelete(RECORD *head,char date[]){
//	RECORD *ptr1,*ptr2;
//	//Ҫ��ɾ�������Ϊ��ͷ���
//	while(head!=NULL&&(strcmp(head->date,date)==0)){
//		ptr2=head;head=head->next;free(ptr2);
//	} 
//	if(head==NULL) return NULL;   //����Ϊ��
//	
//	ptr1=head; //Ҫ��ɾ���Ľ��Ϊ�Ǳ�ͷ���
//	
//	ptr2=head->next ;//�ӱ�ͷ����һ��������� 
//	
//	while(ptr2!=NULL){
//		if(strcmp(ptr2->date,date)==0){ //����ɾ��Ҫ�� 
//		ptr1->next=ptr2->next;
//		free(ptr2); 
//		}
//		else ptr1=ptr2;   //ptr1���ƶ�һ����� 
//		ptr2=ptr1->next;  //ptr2ָ��ptr1�ĺ�һ����� 
//		}
//		
//	return head; 
//}
//
//RECORD *recordSearch(RECORD *head,char date[]){
//	RECORD *p;   //ֻ�洢�������ݵ���Ϣ
//	RECORD *ptr;
//	int size=sizeof(RECORD);
//	int i;
//	
//	if(head==NULL) return head;//������ 
//	else {
//	for(ptr=head;ptr!=NULL;ptr=ptr->next){
//		if(strcmp(date,ptr->date)==0){
//			p=(RECORD *)malloc(size);
//			for(i=0;i<=9;i++){
//				p->number[i]=ptr->number[1];
//			}   
//			strcpy(p->date,ptr->date);
//		}
//	} }
//	return p;
//}
//void recordChange(int field,int number,RECORD *head);
//
//
