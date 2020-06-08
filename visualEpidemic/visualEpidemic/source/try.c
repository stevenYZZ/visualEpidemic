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
	double mx, my;
	static double omx=0,omy=0;
	int i,moveTimes=0;
	static int flagMouseMove=0;
	double dx, moveArea,moveDistance;  //areaHeight=winHeight*0.5;

	moveArea=(winHeight*0.5)*0.2;
	dx=coordinateWidth*0.9/(getDateNumber(rpHeadZoom, rpTailZoom));//�ͻ�ͼʱ��һ��
 	mx = ScaleXInches(x);/*pixels --> inches*/
 	my = ScaleYInches(y);/*pixels --> inches*/

	
	switch(event){
		case BUTTON_DOWN:
			omx=mx;
			omy=my;
			//�ж������ק��ʼλ���Ƿ��ڸ�����
			if(omy>=coordinateY-moveArea && omy<=coordinateY+moveArea){
				if(omx>=coordinateX && omx<=coordinateX+coordinateWidth){
					flagMouseMove=1;
				}
			}
			break;
		case BUTTON_UP:
			if(flagMouseMove==1){
				//�ж������ק��ֹλ���Ƿ���Ȼ�ڸ�����
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
			//����˳������ǰ
			omx=mx;
			omy=my;
			judgeHighLight(omx,omy);
			break;
	}
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


	//���ӻ�ģ�鿪ʼ
	SetWindowTitle("�������");
	SetWindowSize(10, 10); // ��λ - Ӣ��
	InitGraphics();

	winWidth=GetWindowWidth();
	winHeight=GetWindowHeight();

	//dateTail original
	n = getKeyNumber(kp);
	temp=rp;

	//��ʼ����������,״̬ȫΪ������
	for(i=0;i<n;i++){
		highLight[i][0]=-1;//x��λ������Ϊ����
		highLight[i][2]=0;
	}



	//һ��ʼû��display();�Ļ������к���겻���������ھ�û����ʾ
	display();

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





