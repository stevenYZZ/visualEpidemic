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
#include "visualization.h"
#include "linklistMY.h"
//全局变量

//杂七杂八函数
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
		strcpy(popTip,"当前未开启编辑模式");
	}else if(!globalFile){
		globalActive=4;
		strcpy(popTip,"当前未打开或新建文件");
	}
}



//以下函数按button顺序排列
//编辑模式On/Off函数

void editOnOff(){
	commonTF(&globalEdit);
	
}

//新建日期函数
void editNewDate(){
	char datex[10];
	int numx[10];
	int i;

	if(globalEdit&&globalFile){
		//初始化输入数据
		for(i=0;i<=getKeyNumber(kp);i++)numx[i]=0;
			strcpy(datex,rpTailZoom->date);
			addLinkRECORD(&rpTailZoom,nextDate(datex),numx,getKeyNumber(kp));
	}else editTip();
}

//删除最后日期函数
void editDeleteLastDate(){
	if(globalEdit&&globalFile){
		if(rpHeadZoom!=rpTailZoom){
			rpTailZoom=rpTailZoom->prior;
			deleteLinkRECORD(rpTailZoom->next);
		}else{
			globalActive=4;
			strcpy(popTip,"链表仅剩1个节点，无法删除");
		}
	
	}else editTip();
}

//修改参数函数
void editChangeData(){
	if(globalEdit&&globalFile)err();
	else editTip();
}

//新增线函数
void editNewLine(){
	if(globalEdit&&globalFile)err();
	else editTip();
}

//删除线函数
void editDeleteLine(RECORD *head,int numx){
	if(globalEdit&&globalFile){
		
	}else editTip();
}