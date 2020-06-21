#ifndef _visualization_h
#define _visualization_h

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include <time.h>
#include "imgui.h"

#include "linklistMY.h"

//Ԥ������������
#define futureDays 3
#define MAX_DATENUMBER_SHOW 40

//PI
#define PI 3.1415926

//ȫ�ֱ���
extern double winWidth, winHeight;   // ���ڳߴ�
extern double coordinateX, coordinateY;  //����ϵ�����½ǵ�
extern double coordinateWidth, coordinateHeight;  //����ϵ��x/y�᳤�ȣ���drawArea();��ȷ��
extern double highLight[10][4];  //�Ƿ������[][0]x���꣬[][1]y���꣬[][2]�Ƿ����
extern double highLightBoxdx; //�жϸ���Ҫ�õ�box�Ŀ�
extern int peopleMin, peopleMax, peopleDelta;

extern RECORD *rp, *rpHeadZoom, *rpTailZoom;
extern KEY *kp;

extern RECORD *rpHead, *rpTail;
extern KEY *kpHead, *kpTail;

void display();
void drawPic();		//�൱��display()ˢ��
void drawArea();	//��������ϵ���򣨴󳤷��Σ�
int getDateNumber(RECORD *rpHeadZoom, RECORD *rpTailZoom);
void drawFoldLine(double sx, double sy, double dx, double dy, int peopleMin, int peopleMax, int dateNumber, int i);  //��������
int getKeyNumber(KEY *p);	//�ܹ���������
void drawYLine(double sy);
int judgeZoomIn();
void buttonZoomIn();
void buttonZoomOut();
int getTotalPeopleNumber(RECORD *PH,RECORD *PT, int n, int *peopleMax, int *peopleMin);
void connnect(RECORD *p0, RECORD *futurep);
void seperate(RECORD *rp);
void buttonLeftest();
void buttonRightest();
void buttonLeft();
void buttonRight();
void printDateX(double sx, double dx);
void lineName(double px, double py, int i);
void peopleLabel(RECORD *temp, double dx, double labelX, double labelY, int peopleMin, int peopleMax, int i);
void judgeHighLight(double mx, double my);
void judgeHighLightPen(int i);
void updateHighLight(int i, int n);
void sortLineName(int n);
void adjustLineName(int n);
int findSort(int n,int i);
void drawBar(double sx, double dx, double barDy, int n, int j, RECORD* p);
void drawPie(double pieX, double pieY, double r, double n);
void drawFillSector(double x, double y, double r, double start, double delta);
void initHighLight(int n);
void printUse(double buttonBackX, double buttonBackHeight, double buttonBackWidth, double fontA);
void printAbout(double buttonBackX, double buttonBackHeight, double buttonBackWidth, double fontA);

//Button����
//Button���ƺ���
void drawButtons();
//Button�Ű溯��
void addEditButton(int a, int b, void(*p)(), char *str);    //buttonλ�ã�a,b��a����(������)��b�����������꣩��*pΪ��ť�����ĺ���
void addViewButton(int a, int b, void(*p)(), char *str);    //buttonλ�ã�a,b��a����(������)��b�����������꣩��*pΪ��ť�����ĺ���
//Menu���ƺ���
void drawMenu();
//popwindows���ƺ���
void drawPrompt1();//��/���
void drawPrompt2();//��/��/ȡ����
void drawInputBox();//�����
void drawTipBox();//��ʾ��
//״̬�����ƺ���
void drawStatusBox();

#endif