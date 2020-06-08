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

//�������Դ�ͷ�ļ�
#include "common.h"
#include "edit.h"
#include "files.h"
#include "visualization.h"
#include "predict.h"
#include "linklistMY.h"

//ȫ�ֱ���



void DisplayClear(void); 
void startTimer(int id,int timeinterval);
void display(void); 
void addEditButton(int a,int b,void(*p)(),char *str);

//��������


void write(char *s){
	sprintf_s(s,100,"%d",globalEdit);
	if( button(GenUIID(0), 3.5, 3.5, 2, 1, s) ){
		
	}
}

void winchange(){
	winWidth = 6;
}

//���ӻ�ģ��

//����ģ��

void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x,y,button,event); //GUI��ȡ���
	display(); // ˢ����ʾ
}

void Main() 
{

	RECORD *temp=NULL;
	int i, n;


	//¼������
	//RECORD
	RECORD *rpHead=NULL,*rpTail=NULL;
	KEY *kpHead=NULL, *kpTail=NULL;
	char datex[10]="2020";
	char namex[10]="first";
	int num1,num2;

	rpHead=newLinkRECORD();
	rp=rpHead;
	//�ܹ�¼��1+3=4��
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
		//datex����
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
	//�ܹ�¼��2��
	strcpy(kpHead->name,namex);
	kpTail=kpHead;
	kpHead=addLinkKEY(kpHead,kpTail,namex);
	kpTail=kpTail->next;
	//¼�����



	//���ӻ�ģ�鿪ʼ
	SetWindowTitle("�������");
	SetWindowSize(10, 10); // ��λ - Ӣ��
	InitGraphics();

	winWidth=GetWindowWidth();
	winHeight=GetWindowHeight();

	//dateTail original
	n = getKeyNumber(kp);
	temp=rp;
	if (n != 0){
		getOriginalDateHeadTail(rp,rpHeadZoom,rpTailZoom);
	}

	
	registerMouseEvent(MouseEventProcess);      // ���
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





