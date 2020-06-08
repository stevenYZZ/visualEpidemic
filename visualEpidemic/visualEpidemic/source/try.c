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
	char datex[10]="2020";
	char namex[10]="first";
	int num1,num2;

	rpHead=newLinkRECORD();
	rp=rpHead;
	//总共录入1+3=4条
	/*for(i=0;i<=2;i++){
		datex[i]=4;
	}
	datex[i]='\0';*/
	num1=1;
	num2=2;
	strcpy(rpHead->date,datex);
	rpHead->number[0]=num1;
	rpHead->number[1]=num2;
	rpTail=rpHead;

	for(i=0;i<=2;i++){
		//datex不变
		num1=num1+10*i;
		num2=num2+20*i;
		rpHead=addLinkRECORD(rpHead,rpTail,datex,num1,num2);
		rpTail=rpTail->next;
	}
	rpHeadZoom=rp;
	rpTailZoom=rpTail;

	//KEY
	kpHead=newLinkKEY();
	kp=kpHead;
	//总共录入2条
	strcpy(kpHead->name,namex);
	kpTail=kpHead;
	kpHead=addLinkKEY(kpHead,kpTail,namex);
	kpTail=kpTail->next;
	//录入结束



	//可视化模块开始
	SetWindowTitle("疫情分析");
	SetWindowSize(10, 10); // 单位 - 英寸
	InitGraphics();

	winWidth=GetWindowWidth();
	winHeight=GetWindowHeight();

	//dateTail original
	n = getKeyNumber(kp);
	temp=rp;
	if (n != 0){
		getOriginalDateHeadTail(rp,rpHeadZoom,rpTailZoom);
	}

	
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





