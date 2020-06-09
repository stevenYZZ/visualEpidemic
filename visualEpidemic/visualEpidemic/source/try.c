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


//��������

//
//void write(char *s){
//	sprintf_s(s,100,"%d",globalEdit);
//	if( button(GenUIID(0), 3.5, 3.5, 2, 1, s) ){
//		
//	}
//}
//
//void winchange(){
//	winWidth = 6;
//}

//���ӻ�ģ��

//����ģ��


//�¼���Ӧ����
void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x,y,button,event); //GUI��ȡ���
	display(); // ˢ����ʾ
}
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI�ַ�����
	display(); //ˢ����ʾ
}

void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key,event); // GUI��ȡ����
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
	rpHead->number[2]=3;
	rpTail=rpHead;
	for(i=0;i<=30;i++){
		num[0]=1+i;
		num[1]=2+i;
		num[2]=3+i;
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
	//addLinkKEY(kpTail,name[2]);
	writeLinkRECORD(searchLinkRECORD(rpHead,"20200101"),getFieldNum(kpHead),10);


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
	// ע���¼���Ӧ����
	registerCharEvent(CharEventProcess);        // �ַ�
	registerKeyboardEvent(KeyboardEventProcess);// ����
	registerMouseEvent(MouseEventProcess);      // ���
	display();
}
//########################################################################################################	
