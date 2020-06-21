#include "common.h"
#include "edit.h"
#include "files.h"
#include "visualization.h"


//�¼���Ӧ����
void MouseEventProcess(int x, int y, int button, int event)
{
	if(globalFile){
	double mx, my;
	static double omx=0,omy=0;
	int i,moveTimes=0;
	static int flagMouseMove=0;
	double dx, moveArea,moveDistance;  //areaHeight=winHeight*0.5;

	moveArea=(winHeight*0.5)*0.2;
	dx=coordinateWidth*0.9/(getDateNumber(rpHeadZoom, rpTailZoom));//�ͻ�ͼʱ��һ��
 	mx = ScaleXInches(x);/*pixels --> inches*/
 	my = ScaleYInches(y);/*pixels --> inches*/

	
	switch(event){
		case BUTTON_DOWN:
			omx=mx;
			omy=my;
			//�ж������ק��ʼλ���Ƿ��ڸ�����
			if(omy>=coordinateY-moveArea && omy<=coordinateY+moveArea){
				if(omx>=coordinateX && omx<=coordinateX+coordinateWidth){
					flagMouseMove=1;
				}
			}
			break;
		case BUTTON_UP:
			if(flagMouseMove==1){
				//�ж������ק��ֹλ���Ƿ���Ȼ�ڸ�����
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
			//����˳������ǰ
			omx=mx;
			omy=my;
			judgeHighLight(omx,omy);
			break;
	}
	}
	uiGetMouse(x,y,button,event); //GUI��ȡ���
	display(); // ˢ����ʾ
}
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI�ַ�����
	display(); //ˢ����ʾ
}

void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key,event); // GUI��ȡ����
	display(); // ˢ����ʾ
}

void Main() 
{

	RECORD *temp=NULL;
	//���ӻ�ģ�鿪ʼ
	SetWindowTitle("�������");
	SetWindowSize(12, 7); // ��λ - Ӣ��
	InitGraphics();

	winWidth=GetWindowWidth();
	winHeight=GetWindowHeight();

	// ע���¼���Ӧ����
	registerCharEvent(CharEventProcess);        // �ַ�
	registerKeyboardEvent(KeyboardEventProcess);// ����
	registerMouseEvent(MouseEventProcess);      // ���
	display();
}