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
#include "visualization.h"
#include "linklistMY.h"
//ȫ�ֱ���

//�����Ӱ˺���
void empty(){
	;
}
void try0(){
	globalActive=3;
}

void err(){
	exit(-1);
}

void editTip(){
	if(!globalEdit){
		globalActive=4;
		strcpy(popTip,"��ǰδ�����༭ģʽ");
	}else if(!globalFile){
		globalActive=4;
		strcpy(popTip,"��ǰδ�򿪻��½��ļ�");
	}
}



//���º�����button˳������
//�༭ģʽOn/Off����

void editOnOff(){
	commonTF(&globalEdit);
	
}

//�½����ں���
void editNewDate(){
	char datex[10];
	int numx[10];
	int i;

	if(globalEdit&&globalFile){
		//��ʼ����������
		for(i=0;i<=getKeyNumber(kp);i++)numx[i]=0;
			strcpy(datex,rpTailZoom->date);
			addLinkRECORD(&rpTailZoom,nextDate(datex),numx,getKeyNumber(kp));
	}else editTip();
}

//ɾ��������ں���
void editDeleteLastDate(){
	if(globalEdit&&globalFile){
		if(rpHeadZoom!=rpTailZoom){
			rpTailZoom=rpTailZoom->prior;
			deleteLinkRECORD(rpTailZoom->next);
		}else{
			globalActive=4;
			strcpy(popTip,"�����ʣ1���ڵ㣬�޷�ɾ��");
		}
	
	}else editTip();
}

//�޸Ĳ�������
void editChangeData(){
	if(globalEdit&&globalFile)err();
	else editTip();
}

//�����ߺ���
void editNewLine(){
	if(globalEdit&&globalFile)err();
	else editTip();
}

//ɾ���ߺ���
void editDeleteLine(RECORD *head,int numx){
	if(globalEdit&&globalFile){
		
	}else editTip();
}