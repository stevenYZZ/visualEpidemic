#include "common.h"
#include "edit.h"
#include "files.h"
#include "visualization.h"


//事件响应函数
void MouseEventProcess(int x, int y, int button, int event)
{
	if(globalFile){
	double mx, my;
	static double omx=0,omy=0;
	int i,moveTimes=0;
	static int flagMouseMove=0;
	double dx, moveArea,moveDistance;  //areaHeight=winHeight*0.5;

	moveArea=(winHeight*0.5)*0.2;
	dx=coordinateWidth*0.9/(getDateNumber(rpHeadZoom, rpTailZoom));//和画图时候一样
 	mx = ScaleXInches(x);/*pixels --> inches*/
 	my = ScaleYInches(y);/*pixels --> inches*/

	
	switch(event){
		case BUTTON_DOWN:
			omx=mx;
			omy=my;
			//判断鼠标拖拽起始位置是否在该区域
			if(omy>=coordinateY-moveArea && omy<=coordinateY+moveArea){
				if(omx>=coordinateX && omx<=coordinateX+coordinateWidth){
					flagMouseMove=1;
				}
			}
			break;
		case BUTTON_UP:
			if(flagMouseMove==1){
				//判断鼠标拖拽终止位置是否仍然在该区域
				if(my>=coordinateY-moveArea && my<=coordinateY+moveArea){
					if(mx>=coordinateX && mx<=coordinateX+coordinateWidth){
						moveDistance=mx-omx;
						if(moveDistance>=0){
							moveTimes=moveDistance/dx;
							for(i=1;i<=moveTimes;i++){
								buttonLeft();
							}//for
						}//if>=0
					
						else{
							moveDistance=mx-omx;
							moveDistance=-moveDistance;
							moveTimes=moveDistance/dx;
							for(i=1;i<=moveTimes;i++){
								buttonRight();
							}//for
						}//else
					}
				}
			}//if flagMouseMove
			flagMouseMove=0;
			//这里顺序不能提前
			omx=mx;
			omy=my;
			judgeHighLight(omx,omy);
			break;
	}
	}
	uiGetMouse(x,y,button,event); //GUI获取鼠标
	display(); // 刷新显示
}
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI字符输入
	display(); //刷新显示
}

void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key,event); // GUI获取键盘
	display(); // 刷新显示
}

void Main() 
{

	RECORD *temp=NULL;
	//可视化模块开始
	SetWindowTitle("疫情分析");
	SetWindowSize(12, 7); // 单位 - 英寸
	InitGraphics();

	winWidth=GetWindowWidth();
	winHeight=GetWindowHeight();

	// 注册事件响应函数
	registerCharEvent(CharEventProcess);        // 字符
	registerKeyboardEvent(KeyboardEventProcess);// 键盘
	registerMouseEvent(MouseEventProcess);      // 鼠标
	display();
}