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
//// ȫ�ֱ���
//static double winwidth, winheight;   // ���ڳߴ�
//static float  rot_angle = 0;         // ��������ת�Ƕ�
//static int    enable_rotation = 1;   // ������ת
//static int    show_more_buttons = 0; // ��ʾ���ఴť
//
//// ����������provided in libgraphics
//void DisplayClear(void); 
//// ��ʱ������������provided in libgraphics
//void startTimer(int id,int timeinterval);
//
//// �û�����ʾ����
//void display(void); 
//
//// �û����ַ��¼���Ӧ����
//void CharEventProcess(char ch)
//{
//	uiGetChar(ch); // GUI�ַ�����
//	display(); //ˢ����ʾ
//}
//
//// �û��ļ����¼���Ӧ����
//void KeyboardEventProcess(int key, int event)
//{
//	uiGetKeyboard(key,event); // GUI��ȡ����
//	display(); // ˢ����ʾ
//}
//
//// �û�������¼���Ӧ����
//void MouseEventProcess(int x, int y, int button, int event)
//{
//	uiGetMouse(x,y,button,event); //GUI��ȡ���
//	display(); // ˢ����ʾ
//}
//
//// ��ת��ʱ��
//#define MY_ROTATE_TIMER  1
//
//// �û��ļ�ʱ��ʱ����Ӧ����
//void TimerEventProcess(int timerID)
//{
//	if( timerID==MY_ROTATE_TIMER && enable_rotation ) 
//	{
//		rot_angle += 10; // ȫ�ֱ���
//		display(); // ˢ����ʾ
//	}
//}
//
//// �û����������
//// ����ʼ��ִ��һ��
//void Main() 
//{
//	// ��ʼ�����ں�ͼ��ϵͳ
//	SetWindowTitle("Graphics User Interface Demo");
//	//SetWindowSize(10, 10); // ��λ - Ӣ��
//	//SetWindowSize(15, 10);
//	//SetWindowSize(10, 20);  // �����Ļ�ߴ粻�����򰴱�����С
//    InitGraphics();
//
//	// ��ô��ڳߴ�
//    winwidth = GetWindowWidth();
//    winheight = GetWindowHeight();
//
//	// ע��ʱ����Ӧ����
//	registerCharEvent(CharEventProcess);        // �ַ�
//	registerKeyboardEvent(KeyboardEventProcess);// ����
//	registerMouseEvent(MouseEventProcess);      // ���
//	registerTimerEvent(TimerEventProcess);      // ��ʱ��
//
//	// ������ʱ��
//	startTimer(MY_ROTATE_TIMER, 50);            
//
//	// �򿪿���̨��������printf/scanf���/�������Ϣ���������
//	// InitConsole(); 
//}
//
//#if defined(DEMO_MENU)
//// �˵���ʾ����
//void drawMenu()
//{ 
//	static char * menuListFile[] = {"File",  
//		"Open  | Ctrl-O", // ��ݼ��������[Ctrl-X]��ʽ�������ַ����Ľ�β
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
//	double h = fH*1.5; // �ؼ��߶�
//	double w = TextStringWidth(menuListHelp[0])*2; // �ؼ����
//	double wlist = TextStringWidth(menuListEdit[3])*1.2;
//	double xindent = winheight/20; // ����
//	int    selection;
//	
//	// menu bar
//	drawMenuBar(0,y-h,winwidth,h);
//	// File �˵�
//	selection = menuList(GenUIID(0), x, y-h, w, wlist, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));
//	if( selection>0 ) selectedLabel = menuListFile[selection];
//	if( selection==4 )
//		exit(-1); // choose to exit
//	
//	// Tool �˵�
//	menuListEdit[3] = enable_rotation ? "Stop Rotation | Ctrl-T" : "Start Rotation | Ctrl-T";
//	selection = menuList(GenUIID(0),x+w,  y-h, w, wlist,h, menuListEdit,sizeof(menuListEdit)/sizeof(menuListEdit[0]));
//	if( selection>0 ) selectedLabel = menuListEdit[selection];
//	if( selection==3 )
//		enable_rotation = ! enable_rotation;
//	
//	// Help �˵�
//	
//	selection = menuList(GenUIID(0),x+2*w,y-h, w, wlist, h, menuListHelp,sizeof(menuListHelp)/sizeof(menuListHelp[0]));
//	if( selection>0 ) selectedLabel = menuListHelp[selection];
//	if( selection==1 )
//		show_more_buttons = ! show_more_buttons;
//
//	// ��ʾ�˵����н��
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
//	// ����
//	DisplayClear();
//
//
//
//#if defined(DEMO_MENU)
//	// ���ƺʹ���˵�
//	drawMenu();
//#endif
//}
