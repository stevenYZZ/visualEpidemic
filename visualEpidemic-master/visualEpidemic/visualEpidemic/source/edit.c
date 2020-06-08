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

//全局变量



//以下函数按button顺序排列
//编辑模式On/Off函数

void editOnOff(){
	commonTF(&globalEdit);
}

//新建日期函数
void editNewDate();

//删除最后日期函数
void editDeleteLastDate();

//修改参数函数
void editChangeData();

//新增线函数
void editNewLine();

//删除线函数
void editDeleteLine();
