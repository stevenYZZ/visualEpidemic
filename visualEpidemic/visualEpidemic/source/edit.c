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
//ȫ�ֱ���

//�����Ӱ˺���
void empty(){
	;
}

void err(){
	exit(-1);
}

//���º�����button˳������
//�༭ģʽOn/Off����

void editOnOff(){
	commonTF(&globalEdit);
}

//�½����ں���
void editNewDate(){
	if(globalEdit)err();
	else empty();
}

//ɾ��������ں���
void editDeleteLastDate(){
	if(globalEdit)err();
	else empty();
}

//�޸Ĳ�������
void editChangeData(){
	if(globalEdit)err();
	else empty();
}

//�����ߺ���
void editNewLine(){
	if(globalEdit)err();
	else empty();
}

//ɾ���ߺ���
void editDeleteLine(){
	if(globalEdit)err();
	else empty();
}