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
//全局变量

//杂七杂八函数
void empty(){
	;
}

void err(){
	exit(-1);
}

//以下函数按button顺序排列
//编辑模式On/Off函数

void editOnOff(){
	commonTF(&globalEdit);
}

//新建日期函数
void editNewDate(){
	if(globalEdit)err();
	else empty();
}

//删除最后日期函数
void editDeleteLastDate(){
	if(globalEdit)err();
	else empty();
}

//修改参数函数
void editChangeData(){
	if(globalEdit)err();
	else empty();
}

//新增线函数
void editNewLine(){
	if(globalEdit)err();
	else empty();
}

//删除线函数
void editDeleteLine(){
	if(globalEdit)err();
	else empty();
}