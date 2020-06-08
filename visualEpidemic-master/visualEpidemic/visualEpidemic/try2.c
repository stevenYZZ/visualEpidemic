//#include "graphics.h"
//#include "extgraph.h"
//#include "genlib.h"
//#include "simpio.h"
//#include "conio.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <stddef.h>
//
//#include <windows.h>
//#include <olectl.h>
//#include <mmsystem.h>
//#include <wingdi.h>
//#include <ole2.h>
//#include <ocidl.h>
//#include <winuser.h>
//
//#include "imgui.h"
//
//#define DEMO_MENU
//#define DEMO_BUTTON
//#define DEMO_ROTATE
//#define DEMO_EDITBOX
//
//
//
//
//// 全局变量
//static double winwidth, winheight;   // 窗口尺寸
//static float  rot_angle = 0;         // 三角形旋转角度
//static int    enable_rotation = 1;   // 允许旋转
//static int    show_more_buttons = 0; // 显示更多按钮
//
//// 清屏函数，provided in libgraphics
//void DisplayClear(void); 
//// 计时器启动函数，provided in libgraphics
//void startTimer(int id,int timeinterval);
//
//// 用户的显示函数
//void display(void); 
//
//// 用户的字符事件响应函数
//void CharEventProcess(char ch)
//{
//	uiGetChar(ch); // GUI字符输入
//	display(); //刷新显示
//}
//
//// 用户的键盘事件响应函数
//void KeyboardEventProcess(int key, int event)
//{
//	uiGetKeyboard(key,event); // GUI获取键盘
//	display(); // 刷新显示
//}
//
//// 用户的鼠标事件响应函数
//void MouseEventProcess(int x, int y, int button, int event)
//{
//	uiGetMouse(x,y,button,event); //GUI获取鼠标
//	display(); // 刷新显示
//}
//
//// 旋转计时器
//#define MY_ROTATE_TIMER  1
//
//// 用户的计时器时间响应函数
//void TimerEventProcess(int timerID)
//{
//	if( timerID==MY_ROTATE_TIMER && enable_rotation ) 
//	{
//		rot_angle += 10; // 全局变量
//		display(); // 刷新显示
//	}
//}
//
//// 用户主程序入口
//// 仅初始化执行一次
//void Main() 
//{
//	// 初始化窗口和图形系统
//	SetWindowTitle("Graphics User Interface Demo");
//	//SetWindowSize(10, 10); // 单位 - 英寸
//	//SetWindowSize(15, 10);
//	//SetWindowSize(10, 20);  // 如果屏幕尺寸不够，则按比例缩小
//    InitGraphics();
//
//	// 获得窗口尺寸
//    winwidth = GetWindowWidth();
//    winheight = GetWindowHeight();
//
//	// 注册时间响应函数
//	registerCharEvent(CharEventProcess);        // 字符
//	registerKeyboardEvent(KeyboardEventProcess);// 键盘
//	registerMouseEvent(MouseEventProcess);      // 鼠标
//	registerTimerEvent(TimerEventProcess);      // 定时器
//
//	// 开启定时器
//	startTimer(MY_ROTATE_TIMER, 50);            
//
//	// 打开控制台，方便用printf/scanf输出/入变量信息，方便调试
//	// InitConsole(); 
//}
//
//#if defined(DEMO_MENU)
//// 菜单演示程序
//void drawMenu()
//{ 
//	static char * menuListFile[] = {"File",  
//		"Open  | Ctrl-O", // 快捷键必须采用[Ctrl-X]格式，放在字符串的结尾
//		"Save",
//		"Close",
//		"Exit   | Ctrl-E"};
//	static char * menuListEdit[] = {"Edit",
//		"Input",
//		"Modify",
//		"Delete | Ctrl-T"};
//	static char * menuListHelp[] = {"Help",
//		"Use  | Ctrl-M",
//		"About"};
//	static char * selectedLabel = NULL;
//
//	double fH = GetFontHeight();
//	double x = 0; //fH/8;
//	double y = winheight;
//	double h = fH*1.5; // 控件高度
//	double w = TextStringWidth(menuListHelp[0])*2; // 控件宽度
//	double wlist = TextStringWidth(menuListEdit[3])*1.2;
//	double xindent = winheight/20; // 缩进
//	int    selection;
//	
//	// menu bar
//	drawMenuBar(0,y-h,winwidth,h);
//	// File 菜单
//	selection = menuList(GenUIID(0), x, y-h, w, wlist, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));
//	if( selection>0 ) selectedLabel = menuListFile[selection];
//	if( selection==4 )
//		exit(-1); // choose to exit
//	
//	// Tool 菜单
//	menuListEdit[3] = enable_rotation ? "Stop Rotation | Ctrl-T" : "Start Rotation | Ctrl-T";
//	selection = menuList(GenUIID(0),x+w,  y-h, w, wlist,h, menuListEdit,sizeof(menuListEdit)/sizeof(menuListEdit[0]));
//	if( selection>0 ) selectedLabel = menuListEdit[selection];
//	if( selection==3 )
//		enable_rotation = ! enable_rotation;
//	
//	// Help 菜单
//	
//	selection = menuList(GenUIID(0),x+2*w,y-h, w, wlist, h, menuListHelp,sizeof(menuListHelp)/sizeof(menuListHelp[0]));
//	if( selection>0 ) selectedLabel = menuListHelp[selection];
//	if( selection==1 )
//		show_more_buttons = ! show_more_buttons;
//
//	// 显示菜单运行结果
//	x = winwidth/15;
//	y = winheight/8*7;
//	SetPenColor("Blue");
//	drawLabel(x,    y-=h, "Most recently selected menu is:");
//	SetPenColor("Red");
//	drawLabel(x+xindent, y-=h, selectedLabel);
//	SetPenColor("Blue");
//	drawLabel(x,    y-=h, "Control Variable Status");
//	SetPenColor("Red");
//	drawLabel(x+xindent, y-=h, enable_rotation ? "Rotation Enabled" : "Rotation Disabled");
//	drawLabel(x+xindent, y-=h, show_more_buttons ? "More Buttons" : "Less Button");
//}
//#endif // #if defined(DEMO_MENU)
//
//
//void display()
//{
//	// 清屏
//	DisplayClear();
//
//
//
//#if defined(DEMO_MENU)
//	// 绘制和处理菜单
//	drawMenu();
//#endif
//}
