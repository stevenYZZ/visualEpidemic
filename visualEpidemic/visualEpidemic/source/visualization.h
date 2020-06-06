#ifndef _visualization_h
#define _visualization_h

#include "common.h"
#include "linklistMY.h"

//ȫ�ֱ���
extern double winWidth, winHeight;   // ���ڳߴ�

extern double coordinateX,coordinateY;  //����ϵ�����½ǵ�
extern double coordinateWidth, coordinateHeight;  //����ϵ��x/y�᳤�ȣ���drawArea();��ȷ��

extern RECORD *rp, *rpHeadZoom,*rpTailZoom;
extern KEY *kp;


void drawPic();  //�൱��display()ˢ��
void drawArea();  //��������ϵ���򣨴󳤷��Σ�
void getOriginalDateHeadTail(RECORD *rp,RECORD *rpHeadZoom, RECORD *rpTailZoom);
int getDateNumber(RECORD *rpHeadZoom, RECORD *rpTailZoom);
void drawFoldLine(double sx, double sy, double dx, double dy, int peopleMin, int peopleMax,int dateNumber, int i);  //��������
int getKeyNumber(KEY *p);//�ܹ���������
void drawYLine(double sy,double dy, int peopleMax,int peopleMin,int peopleDelta);
int judgeZoomIn();
void buttonZoomIn();
void buttonZoomOut();
int getTotalPeopleNumber(RECORD *rp,int n,int *peopleMax, int*peopleMin);
void connnect(RECORD *rp, RECORD *futurep);
void seperate(RECORD *rp);
void buttonLeftest();
void buttonRight();
void buttonCustomize(int day1,int month1,int year1,int day2,int month2, int year2,int upper, int lower);

//Button����
//Button���ƺ���
void drawButtons();

//Button�Ű溯��
void addEditButton(int a,int b,void(*p)(),char *str);    //buttonλ�ã�a,b��a����(������)��b�����������꣩��*pΪ��ť�����ĺ���

void addViewButton(int a,int b,void(*p)(),char *str);    //buttonλ�ã�a,b��a����(������)��b�����������꣩��*pΪ��ť�����ĺ���

#endif