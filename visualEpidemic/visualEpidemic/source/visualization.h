#ifndef _visualization_h
#define _visualization_h

#include "common.h"
#include "linklistMY.h"

//全局变量
extern double winWidth, winHeight;   // 窗口尺寸

extern double coordinateX,coordinateY;  //坐标系的左下角点
extern double coordinateWidth, coordinateHeight;  //坐标系的x/y轴长度，带drawArea();里确定
extern double highLight[10][4];  //是否高亮，[][0]x坐标，[][1]y坐标，[][2]是否高亮
extern double highLightBoxdx; //判断高亮要用到box的宽
extern RECORD *rp, *rpHeadZoom,*rpTailZoom;
extern KEY *kp;


void drawPic();  //相当于display()刷新
void drawArea();  //绘制坐标系区域（大长方形）
void getOriginalDateHeadTail(RECORD *rp,RECORD *rpHeadZoom, RECORD *rpTailZoom);
int getDateNumber(RECORD *rpHeadZoom, RECORD *rpTailZoom);
void drawFoldLine(double sx, double sy, double dx, double dy, int peopleMin, int peopleMax,int dateNumber, int i);  //绘制折线
int getKeyNumber(KEY *p);//总共多少条线
void drawYLine(double sy,double dy, int peopleMax,int peopleMin,int peopleDelta);
int judgeZoomIn();
void buttonZoomIn();
void buttonZoomOut();
int getTotalPeopleNumber(int n,int *peopleMax, int*peopleMin);
void connnect(RECORD *rp, RECORD *futurep);
void seperate(RECORD *rp);
void buttonLeftest();
void buttonRightest();
void buttonLeft();
void buttonRight();
void buttonCustomize(int day1,int month1,int year1,int day2,int month2, int year2,int upper, int lower);
void printDateX(double sx, double sy, double dx, double dy,int dateNumber);
void lineName(double px,double py, int i);
void peopleLabel(RECORD *temp, double dx, double labelX, double labelY, int peopleMin, int peopleMax, int i);
void judgeHighLight(double mx,double my);
void updateHighLight(int i,int n);
void sortLineName(int n);
void adjustLineName(int n);
int findSort(int n,int i);
void drawBar(double sx, double dx, double barDy,int n, int j, RECORD* p);
//Button函数
//Button绘制函数
void drawButtons();

//Button排版函数
void addEditButton(int a,int b,void(*p)(),char *str);    //button位置（a,b）a列数(横坐标)，b行数（纵坐标），*p为按钮触发的函数

void addViewButton(int a,int b,void(*p)(),char *str);    //button位置（a,b）a列数(横坐标)，b行数（纵坐标），*p为按钮触发的函数

#endif