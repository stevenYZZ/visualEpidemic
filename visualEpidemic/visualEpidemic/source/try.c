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

//本程序自带头文件
#include "common.h"
#include "edit.h"
#include "files.h"
#include "visualization.h"
#include "predict.h"
#include "linklistMY.h"

//全局变量



void DisplayClear(void); 
void startTimer(int id,int timeinterval);
void display(void); 
void addEditButton(int a,int b,void(*p)(),char *str);

//新增函数


void write(char *s){
	sprintf_s(s,100,"%d",globalEdit);
	if( button(GenUIID(0), 3.5, 3.5, 2, 1, s) ){
		
	}
}

void winchange(){
	winWidth = 6;
}

//可视化模块

//链表模块

void MouseEventProcess(int x, int y, int button, int event)
{	
	double mx, my;
	static double omx=0,omy=0;
	int i,moveTimes=0;
	static int flagMouseMove=0;
	double dx, moveArea,moveDistance;  //areaHeight=winHeight*0.5;

	moveArea=(winHeight*0.5)*0.2;
	dx=coordinateWidth*0.9/(getDateNumber(rpHeadZoom, rpTailZoom));//和画图时候一样
 	mx = ScaleXInches(x);/*pixels --> inches*/
 	my = ScaleYInches(y);/*pixels --> inches*/

	
	switch(event){
		case BUTTON_DOWN:
			omx=mx;
			omy=my;
			//判断鼠标拖拽起始位置是否在该区域
			if(omy>=coordinateY-moveArea && omy<=coordinateY+moveArea){
				if(omx>=coordinateX && omx<=coordinateX+coordinateWidth){
					flagMouseMove=1;
				}
			}
			break;
		case BUTTON_UP:
			if(flagMouseMove==1){
				//判断鼠标拖拽终止位置是否仍然在该区域
				if(my>=coordinateY-moveArea && my<=coordinateY+moveArea){
					if(mx>=coordinateX && mx<=coordinateX+coordinateWidth){
						moveDistance=mx-omx;
						if(moveDistance>=0){
							moveTimes=moveDistance/dx;
							for(i=1;i<=moveTimes;i++){
								buttonLeft();
							}//for
						}//if>=0
					
						else{
							moveDistance=mx-omx;
							moveDistance=-moveDistance;
							moveTimes=moveDistance/dx;
							for(i=1;i<=moveTimes;i++){
								buttonRight();
							}//for
						}//else
					}
				}
			}//if flagMouseMove
			flagMouseMove=0;
			//这里顺序不能提前
			omx=mx;
			omy=my;
			judgeHighLight(omx,omy);
			break;
	}
	uiGetMouse(x,y,button,event); //GUI获取鼠标
	display(); // 刷新显示
}



void Main() 
{

	RECORD *temp=NULL;
	int i, n;


	//录入数据
	//RECORD
	RECORD *rpHead=NULL,*rpTail=NULL;
	KEY *kpHead=NULL, *kpTail=NULL;
	char datex[10]="20200101";
	char name[4][10]={"first","second","third"};
	int num[10];
	rpHead=newLinkRECORD();
	rp=rpHead;
	strcpy(rpHead->date,datex);
	rpHead->number[0]=1;
	rpHead->number[1]=2;
	rpHead->number[2]=4;
	rpTail=rpHead;
	for(i=0;i<=30;i++){
		num[0]=1;
		num[1]=2+i;
		num[2]=3+i*2;
		addLinkRECORD(&rpTail,nextDate(datex),num,3);
	}
	rpHeadZoom=rp;
	rpTailZoom=rpTail;

	//KEY
	kpHead=newLinkKEY();
	kp=kpHead;
	strcpy(kpHead->name,name[0]);
	kpTail=kpHead;
	addLinkKEY(&kpTail,name[1]);
	addLinkKEY(&kpTail,name[2]);


	//可视化模块开始
	SetWindowTitle("疫情分析");
	SetWindowSize(10, 10); // 单位 - 英寸
	InitGraphics();

	winWidth=GetWindowWidth();
	winHeight=GetWindowHeight();

	//dateTail original
	n = getKeyNumber(kp);
	temp=rp;

	//初始化高亮设置,状态全为不高亮
	for(i=0;i<n;i++){
		highLight[i][0]=-1;//x定位不可能为负数
		highLight[i][2]=0;
	}



	//一开始没有display();的话，运行后鼠标不划到界面内就没有显示
	display();

	registerMouseEvent(MouseEventProcess);      // 鼠标
//########################################################################################################

	

	
}



//void Trydraw(){
//	char s[100];
//	char s1[100];
//	char s2[100];
//	char t[8][100];
//	double num0 = 10;
//	int i;
//	RECORD *p = activeRECORD;
//	num0=winWidth;
//	num0=0.75*winWidth/(4*(1+1/6));
//	//sprintf_s(s1,100,"global=%d", globalEdit);
//	sprintf_s(s,100,"20200128");
//	sprintf_s(s1,100,"20200204");
//
//	activeRECORD = recordCreat(s,s1,3);
//	for(i=0;i<=2;i++){
//		sprintf_s(t[i],400,"d=%s %d %d %d",p->date,p->number[0],p->number[1],p->number[2]);
//	}
//
//	if( button(GenUIID(0), 4, 4, 2, 1, s2) ){
//		write(s);
//	}
//
//	/*
//	addViewButton(1,3,err,t[5]);
//	addViewButton(2,3,err,t[6]);
//	addViewButton(3,3,err,t[7]);
//	addViewButton(4,3,err,t[8]);
//	addViewButton(1,4,err,t[1]);
//	addViewButton(2,4,err,t[2]);
//	addViewButton(3,4,err,t[3]);
//	addViewButton(4,4,err,t[4]);
//	*/
//}





