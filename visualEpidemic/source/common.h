#ifndef _common_h
#define _common_h

#include "linklistMY.h"

//全局变量
extern int globalActive;		//全局活跃页面变量 表示当前活跃页面（0123分别对应主页面/是否框/是否取消框/可输入框等）
extern int globalEdit;			//主页面编辑模式变量 表示主页面是否处于编辑模式（0否1是）
extern int globalPredict;		//主页面预测模式变量 表示主页面是否处于预测模式（0否1是）
extern int globalPredictMode;	//全局预测模式变量，表示当前所处的预测模式（0指数函数1抛物线函数）
extern int globalPredictDays;	//全局预测天数变量，表示预测的天数
extern int globalStatus;		//全局状态变量，表示当前正在进行的操作
extern int globalFile;			//全局文件变量，表示当前有无打开的文件（0无1有）
extern int globalLabel;			//全局标注变量，表示是否显示数字标注（0否1是）
extern int globalDate;			//全局日期标注变量，表示是否显示日期标注（0否1是）
extern int globalHelp;			//全局帮助页面切换变量，表示当前是哪个界面（0绘图；1use；2about）
extern int globalFitIn;			//全局绘图上下限变量，表示当前上下限确定方法（0适应数据；1适应区域）
extern char globalFileName[100];//全局文件名变量，表示当前打开的文件名(空字符串为无)

extern char popTip[200];		//表示在弹窗中显示的内容
extern char popInputTip[200];	//表示在输入框中提示的内容
extern char popInput[200];		//表示输入框中的用户输入
extern int popStatus2;			//表示prompt2弹窗状态
extern int popInputStatus;//表示inputBox弹窗状态   0修改参数，1新增线 2新建文件 3打开文件
//可重复利用的标准函数

//int类型1变0，0变1函数
void commonTF(int *a);

//生成下一天日期字符串函数(将输入字符串改为新日期)
char *nextDate(char today[]);

//得到RECORD链表总天数函数
int getTotalDays( RECORD* p );

//弹窗函数
void console(char s[]);

#endif



