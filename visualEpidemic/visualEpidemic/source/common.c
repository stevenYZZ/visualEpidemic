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
//    char date[10];                        //日期
//    int number[10];                  //各组数据人数
//    struct record *prior, *next;    //链表指针
//}RECORD;
//typedef struct key{
//    char name[20];                  //每组数据字段名
//    struct record *prior, *next;    //链表指针
//}KEY; 

//全局变量
int globalEdit = 0; 




//本文件所用杂七杂八函数


//判断是否为闰年函数
int ifRun(int n){
	if((n%4==0) && (n%100!=0) || (n%400==0))return 1;
	else return 0;
}
//可重复利用的标准函数
//int类型1变0，0变1函数
void commonTF(int *a){
	if(*a==0)*a=1;
	else *a=0;

}





//生成下一天日期字符串函数
char *nextDate(char today[]){
	char year[4];char month[2];char day[2]; //char类型年月日
	int y,m,d;			//int类型年月日
	int mAdd=0;     //月份是否进位
	
	//得到int类型年月日
	strncpy(year, today, 4);y=atoi(year);
	strncpy(month, today+4, 2);m=atoi(month);
	strncpy(day, today+6, 2);d=atoi(day);

	//计算
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

	//年月日拼接成char类型
	sprintf(today,"%d%02d%02d",y,m,d);
	return today;
}



////链表函数
///*
//void linklistEdit(){
// 	RECORD *head,*p;
// 	char date[10]; int number[10];
// 	int choice;
// 
// 	int size=sizeof(RECORD);
// 	scanf("%d",&choice); 
// 	do{     //(1.新建 2新增结点 3删除结点 4更改数据 5查找数据 0exit) 
// 	       switch(choice){
//			case 1:head=recordCreat();break;
// 	       
//			case 2:scanf("%s%d%d%d%d",date,&number[1],&number[2],&number[3],&number[4]);// 手动输入日期和对应数据 
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
//		     case 4:scanf("%s%d%d%d%d",date,&number[1],&number[2],&number[3],&number[4]);// 手动输入日期和对应数据 
//	              p=(RECORD*)malloc(size);
//	             
//				  p->number[1]=number[1];p->number[2]=number[2];p->number[3]=number[3];p->number[4]=number[4];
//	            
//				 strcpy(p->date,date);
//				  head=recordDelete(head,date);    //删除要更改的结点
//				    head=recordAdd(head,p);       //加入更改后的结点 
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
////1新增结点函数
////功能：往给定列表新增一个给定结点
//RECORD * recordAdd(RECORD *head,RECORD *add){
//	
//	RECORD *ptr,*ptr1,*ptr2;
//	
//	ptr2=head; ptr=add;  //*ptr指向待插入的新的结点
//	
//	if(head==NULL){
//		head=ptr;head->next=NULL; //新插入结点成为头结点
//		 }
//    else{              //原链表不为空时的插入 
//    	while((strcmp(ptr->date,ptr2->date)>0)&&(ptr2->next!=NULL)){
//    		ptr1=ptr2;    //ptr1,ptr2各后移一个结点
//			ptr2=ptr2->next; 
//		}
//		if(strcmp(ptr->date,ptr2->date)<=0){//在ptr1和ptr2之间插入新结点
//		if(head==ptr2) head=ptr;
//		else ptr1->next=ptr;
//		ptr->next=ptr2; 
//		}
//		else{//新插入结点成为尾结点 
//			ptr2->next=ptr;ptr->next=NULL; 
//		}
//		
//	} 
//	return head;
//}
//
////2新建链表函数
////功能：新建一个确定日期范围和字段数量的链表
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
////3删除结点函数
////功能：在给定列表删除一个给定结点
//RECORD * recordDelete(RECORD *head,char date[]){
//	RECORD *ptr1,*ptr2;
//	//要被删除结点作为表头结点
//	while(head!=NULL&&(strcmp(head->date,date)==0)){
//		ptr2=head;head=head->next;free(ptr2);
//	} 
//	if(head==NULL) return NULL;   //链表为空
//	
//	ptr1=head; //要被删除的结点为非表头结点
//	
//	ptr2=head->next ;//从表头的下一个结点搜索 
//	
//	while(ptr2!=NULL){
//		if(strcmp(ptr2->date,date)==0){ //符合删除要求 
//		ptr1->next=ptr2->next;
//		free(ptr2); 
//		}
//		else ptr1=ptr2;   //ptr1后移动一个结点 
//		ptr2=ptr1->next;  //ptr2指向ptr1的后一个结点 
//		}
//		
//	return head; 
//}
//
//RECORD *recordSearch(RECORD *head,char date[]){
//	RECORD *p;   //只存储查找数据的信息
//	RECORD *ptr;
//	int size=sizeof(RECORD);
//	int i;
//	
//	if(head==NULL) return head;//空链表 
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
