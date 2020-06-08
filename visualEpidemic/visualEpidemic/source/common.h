#ifndef _common_h
#define _common_h

#include "visualization.h"

//全局变量
extern int globalEdit;
extern int statusNumber;
extern int  promptNumber;
//
////链表操作函数
//
//typedef struct record{
//    char date[10];                        //日期
//    int number[10];                  //各组数据人数
//    struct record *prior, *next;    //链表指针
//}RECORD;
//typedef struct key{
//    char name[20];                  //每组数据字段名
//    struct record *prior, *next;    //链表指针
//}KEY;
//
//RECORD *recordAdd(RECORD *head,RECORD *add);  //1新增结点
//
//RECORD *recordCreat(char headDate[],char tailDate[],int fieldNum);    //2新建链表 
//
//RECORD *recordDelete(RECORD *head,char date[]);  //3删除结点 
//
//RECORD *recordSearch(RECORD *head,char date[]);  //4查找数据
//
//void recordChange(int field,int number,RECORD *head);  //5更改数据 
//
//void linklistEdit();   //链表操作主程序
//



//可重复利用的标准函数

//int类型1变0，0变1函数
void commonTF(int *a);
void OpenFiles(char *p);
void Prompt(int n);
void Status(int n);
#endif



