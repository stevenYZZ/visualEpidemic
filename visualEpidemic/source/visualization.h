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

//预测链表有三天
#define futureDays 3
#define MAX_DATENUMBER_SHOW 40

//PI
#define PI 3.1415926

//全局变量
extern double winWidth, winHeight;   // 窗口尺寸
extern double coordinateX, coordinateY;  //坐标系的左下角点
extern double coordinateWidth, coordinateHeight;  //坐标系的x/y轴长度，带drawArea();里确定
extern double highLight[10][4];  //是否高亮，[][0]x坐标，[][1]y坐标，[][2]是否高亮
extern double highLightBoxdx; //判断高亮要用到box的宽
extern int peopleMin, peopleMax, peopleDelta;

extern RECORD *rp, *rpHeadZoom, *rpTailZoom;
extern KEY *kp;

extern RECORD *rpHead, *rpTail;
extern KEY *kpHead, *kpTail;

void display();
void drawPic();		//相当于display()刷新
void drawArea();	//绘制坐标系区域（大长方形）
int getDateNumber(RECORD *rpHeadZoom, RECORD *rpTailZoom);
void drawFoldLine(double sx, double sy, double dx, double dy, int peopleMin, int peopleMax, int dateNumber, int i);  //绘制折线
int getKeyNumber(KEY *p);	//总共多少条线
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

//Button函数
//Button绘制函数
void drawButtons();
//Button排版函数
void addEditButton(int a, int b, void(*p)(), char *str);    //button位置（a,b）a列数(横坐标)，b行数（纵坐标），*p为按钮触发的函数
void addViewButton(int a, int b, void(*p)(), char *str);    //button位置（a,b）a列数(横坐标)，b行数（纵坐标），*p为按钮触发的函数
//Menu绘制函数
void drawMenu();
//popwindows绘制函数
void drawPrompt1();//是/否框
void drawPrompt2();//是/否/取消框
void drawInputBox();//输入框
void drawTipBox();//提示框
//状态栏绘制函数
void drawStatusBox();

#endif