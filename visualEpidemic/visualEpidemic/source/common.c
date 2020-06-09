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

#include "visualization.h"
//全局变量
int globalActive = 0;		//全局活跃页面变量 表示当前活跃页面（0123分别对应主页面/是否框/是否取消框/可输入框等）
int globalEdit = 0;			//主页面编辑模式变量 表示主页面是否处于编辑模式（0否1是）
int globalPredict = 0;		//主页面预测模式变量 表示主页面是否处于预测模式（0否1是）
int globalStatus = 0;		//全局状态变量，表示当前正在进行的操作
int globalFile = 1;			//全局文件变量，表示当前有无打开的文件（0无1有）

char popTip[200];		//表示在提示框中显示的内容
char popInputTip[200];	//表示在输入框中提示的内容
char popInput[200];		//表示输入框中的用户输入
int popStatus2 = -1;	//表示prompt2弹窗状态
int popInputStatus = -1;//表示inputBox弹窗状态   0修改参数，1新增线


//本文件所用杂七杂八函数


//判断是否为闰年函数
int ifRun(int n){
	if((n%4==0) && (n%100!=0) || (n%400==0))return 1;
	else return 0;
}

//可重复利用的标准函数
//int类型1变0，0变1函数
void commonTF(int *a){
	if(*a==0)*a=1;
	else *a=0;

}

//打开当前文件夹下文件
void OpenFiles(char *p){
	FILE*fp;
	if((fp=fopen(p,"a+"))==NULL){
		exit(0);
	}
} 






//状态栏函数      利用全局变量控制显示什么状态
void Status(int n){
	double fH = GetFontHeight();
	if(n==1) drawLabel(0,fH,"编辑模式：开启");

}


//生成下一天日期字符串函数(将输入字符串改为新日期)
char *nextDate(char today[]){
	char year[4];char month[2];char day[2]; //char类型年月日
	int y,m,d;			//int类型年月日
	int mAdd=0;     //月份是否进位
	
	//得到int类型年月日
	strncpy(year, today, 4);y=atoi(year);
	strncpy(month, today+4, 2);m=atoi(month);
	strncpy(day, today+6, 2);d=atoi(day);

	//计算
	if(d==28 && !ifRun(y) && m==2){
			d=0;
			mAdd++;
	}else if(d==29 && ifRun(y) && m==2){
			d=0;
			mAdd++;
	}else if(d==30 && (m==4 || m==6 || m==9 || m==11)){
			d=0;
			mAdd++;
	}else if(d==31 && m!=2 && m!=4 && m!=6 && m!=9 && m!=11){
			d=0;
			mAdd++;
	}else;
	d=d+1;
	if(m==12 && mAdd==1){
		y++;
		m=1;
	}else m=m+mAdd;

	//年月日拼接成char类型
	sprintf(today,"%d%02d%02d",y,m,d);
	return today;
}