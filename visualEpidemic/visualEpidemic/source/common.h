#ifndef _common_h
#define _common_h

//全局变量
extern int globalActive;		//全局活跃页面变量 表示当前活跃页面（0123分别对应主页面/是否框/是否取消框/可输入框等）
extern int globalEdit;			//主页面编辑模式变量 表示主页面是否处于编辑模式（0否1是）
extern int globalPredict;		//主页面预测模式变量 表示主页面是否处于预测模式（0否1是）
extern int globalStatus;		//全局状态变量，表示当前正在进行的操作
extern int globalFile;			//全局文件变量，表示当前有无打开的文件（0无1有）
	
extern char popTip[200];		//表示在弹窗中显示的内容
extern char popInputTip[200];	//表示在输入框中提示的内容
extern char popInput[200];		//表示输入框中的用户输入
extern int popStatus2;				//表示弹窗状态
//可重复利用的标准函数

//int类型1变0，0变1函数
void commonTF(int *a);

//生成下一天日期字符串函数(将输入字符串改为新日期)
char *nextDate(char today[]);
void OpenFiles(char *p);

//弹窗函数
void callPrompt2(int n);
void callInputBox(int n);




#endif



