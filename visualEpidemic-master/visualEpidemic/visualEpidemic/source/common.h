#ifndef _common_h
#define _common_h

#include "visualization.h"

//ȫ�ֱ���
extern int globalEdit;
extern int statusNumber;
extern int  promptNumber;
//
////�����������
//
//typedef struct record{
//    char date[10];                        //����
//    int number[10];                  //������������
//    struct record *prior, *next;    //����ָ��
//}RECORD;
//typedef struct key{
//    char name[20];                  //ÿ�������ֶ���
//    struct record *prior, *next;    //����ָ��
//}KEY;
//
//RECORD *recordAdd(RECORD *head,RECORD *add);  //1�������
//
//RECORD *recordCreat(char headDate[],char tailDate[],int fieldNum);    //2�½����� 
//
//RECORD *recordDelete(RECORD *head,char date[]);  //3ɾ����� 
//
//RECORD *recordSearch(RECORD *head,char date[]);  //4��������
//
//void recordChange(int field,int number,RECORD *head);  //5�������� 
//
//void linklistEdit();   //�������������
//



//���ظ����õı�׼����

//int����1��0��0��1����
void commonTF(int *a);
void OpenFiles(char *p);
void Prompt(int n);
void Status(int n);
#endif



