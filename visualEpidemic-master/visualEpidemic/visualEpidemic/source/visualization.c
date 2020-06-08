#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include <time.h>
#include "imgui.h"

//�������Դ�ͷ�ļ�
#include "common.h"
#include "linklistMY.h"
#include "visualization.h"
#include "edit.h"
//Ԥ������������
#define futureDays 3
#define DEMO_MENU


//ȫ�ֱ���
double winWidth, winHeight;   // ���ڳߴ�
double coordinateX,coordinateY;  //����ϵ�����½ǵ�
double coordinateWidth, coordinateHeight;  //����ϵ��x/y�᳤�ȣ���drawArea();��ȷ��
RECORD *rp=NULL, *rpHeadZoom=NULL,*rpTailZoom=NULL;
KEY *kp=NULL;

//ʵ�麯��
void err(){
	exit(-1);
}

//����ˢ��
//����buttonǰ��ҲҪ���Ƶ������������

void display(){
	//��������ֿ��ӻ�����
	DisplayClear();  //�����������Դ�
	drawPic();
	drawButtons();
	#if defined(DEMO_MENU)
	// ���ƺʹ���˵�
	drawMenu();
#endif
}


void drawPic(){
	int i, n=-1, peopleDelta=-1;
	int dateNumber;//��ĩ����֮������
	double sx,sy;//��һ��+����minʱ��ʵ����꣬�����о���
	double dx,dy;//ÿ�����������仯��׼���
	int peopleMin=-1,peopleMax=-1;


	drawArea();


	n = getKeyNumber(kp);
	peopleDelta = getTotalPeopleNumber(rp,n,&peopleMax,&peopleMin);
	dateNumber = getDateNumber(rpHeadZoom,rpTailZoom);
	
	dx=coordinateWidth*0.9/(dateNumber);
	dy=coordinateHeight*0.9/peopleDelta;
	sx=coordinateWidth*0.05+coordinateX;
	sy=coordinateHeight*0.05+coordinateY;
	drawYLine(sy,dy,peopleMax,peopleMin,peopleDelta);

	//���߻���
	for(i=0;i<n;i++){
		drawFoldLine(sx,sy,dx,dy,peopleMin,peopleMax,dateNumber,i);  
	}
	
}

//�������Ⱥ���,�����֣������ߣ����п��ޣ�
void drawYLine(double sy,double dy, int peopleMax,int peopleMin,int peopleDelta){
	double d10,ny;//ny��nowY
	int i;
	d10=peopleDelta/5*dy;//6���ߣ�5������
	ny=sy;

	SetPenColor("Gray");

	for(i=0;i<=5;i++){
		ny=sy+i*d10;
		MovePen(coordinateX,ny);
		DrawLine(coordinateWidth,0);
	}
}


//draw����ϵ��Ĵ󳤷����Լ�����ϵx/y��
void drawArea(){
	double areaWidth, areaHeight;  //������area�Ŀ���
	double areaX, areaY;  //ͼ��area���½ǵ�

	areaX=winWidth*0.1;
	areaY=winHeight*0.25;
	areaWidth=winWidth*0.6;
	areaHeight=winHeight*0.5;

	coordinateX=areaX+areaWidth*0.1;
	coordinateY=areaY+areaHeight*0.1;

	coordinateHeight=areaHeight*0.8;  //����ϵ����ռareaWidth��0.8
	coordinateWidth=areaWidth*0.8;  //����ϵ����ռareaHeight��0.8

	MovePen(areaX,areaY);
	DrawLine(0,areaHeight);
	DrawLine(areaWidth,0);
	DrawLine(0,-areaHeight);
	DrawLine(-areaWidth,0);

	//С��ͷ�Ĵ�Сֱ�ӻ���ʱ������֣�Ŀǰ�����᳤��coordinateHeight*0.05
	//y��
	MovePen(coordinateX,coordinateY);
	DrawLine(0,coordinateHeight);
	//С��ͷ
	DrawLine(coordinateHeight*0.05,-coordinateHeight*0.05);
	MovePen(coordinateX,coordinateY+coordinateHeight);
	DrawLine(-coordinateHeight*0.05,-coordinateHeight*0.05);
	//x��
	MovePen(coordinateX,coordinateY);
	DrawLine(coordinateWidth,0);
	//С��ͷ
	DrawLine(-coordinateHeight*0.05,+coordinateHeight*0.05);
	MovePen(coordinateX+coordinateWidth,coordinateY);
	DrawLine(-coordinateHeight*0.05,-coordinateHeight*0.05);
}

/*
*  ���ܣ�get�м�����
*  ������pΪKEY�ṹ�����׵�ַ
*/
int getKeyNumber(KEY *p){
	int n = 0;

	if(p->next==NULL){
		if(p->name[0]=='\0'){
			return 0;
		}
		else{
			return 1;
		}
	}

	while(p->next != NULL){
		n++;
		p = p->next;
	}
	n++;
	return n;
}


//�ж��Ƿ�����Ŵ�
int judgeZoomIn(){
	int count=0;
	RECORD *temp=NULL;
	temp=rpHeadZoom;
	while(temp->next != rpTailZoom){
		count++;
		temp=temp->next;
		if(count>2){
			break;
		}
	}
	if (count <= 1){  //��ʣ<=3������
		return 0;
	}
	else{  //ʣ��>=4������
		return 1;
	}
}

//�Ŵ����
void buttonZoomIn(){
	if(judgeZoomIn()){//�Ŵ���ͷβ�෴
		rpHeadZoom=rpHeadZoom->next;
		rpTailZoom=rpTailZoom->prior;
	}
	else{
		return;
	}
}



//��С����
void buttonZoomOut(){
	RECORD *temp;
	if(rpHeadZoom->prior == NULL && rpTailZoom->next == NULL){//û����С��أ����˶���ͷ
		return;
	}
	if(rpHeadZoom->prior!=NULL&&rpTailZoom->next!=NULL){//���˶�û��ͷ
		rpHeadZoom=rpHeadZoom->prior;
		rpTailZoom=rpTailZoom->next;
	}
	else if(rpHeadZoom->prior==NULL){//�׵�ͷ���ж�β��
		temp=rpTailZoom->next;
		if(temp->next !=NULL){//β�˿���������
			rpTailZoom=temp->next;
			return;
		}
		else{//β����һ��
			rpTailZoom=temp;
			return;
		}
	}//else if
	else if(rpTailZoom->next == NULL){
		temp=rpHeadZoom->prior;
		if(temp->prior!=NULL){//ͷ������������
			rpHeadZoom=temp->prior;
			return;
		}
		else{//ͷ����һ��
			rpHeadZoom=temp;
			return;
		}
	}//else if
}

//��dateNumber���������ܵ�ͷβ��ַ
void getOriginalDateHeadTail(RECORD *rp,RECORD *rpHeadZoom, RECORD *rpTailZoom){
	RECORD *temp=rp;

	while (temp->next != NULL){
		temp=temp->next;
	}

	rpHeadZoom=rp;
	rpTailZoom=temp;
}

//ԭʼ/���ź�ͷβ֮������
int getDateNumber(RECORD *rpHeadZoom, RECORD *rpTailZoom){
	int count=0;
	RECORD *temp;
	temp = rpHeadZoom;
	while(temp->next!=NULL){
		count++;
		temp=temp->next;
	}
	count++;
	return count;
}

//�õ�����ļ������ݣ����������ֵ����Сֵ������max��min�Ĳ�ֵ������y������ȷ��
int getTotalPeopleNumber(RECORD *rp,int n,int *peopleMax, int *peopleMin){
	int i,max=0,min=rp->number[0];
	RECORD *temp=NULL;
	temp=rp;
	while(temp->next!=NULL){
		for(i=0;i<=n-1;i++){
			if(temp->number[i]>max){
				max=temp->number[i];
			}
			else if(temp->number[i]<min){
				min=temp->number[i];
			}
		}
		temp=temp->next;
	}
	for(i=0;i<=n-1;i++){
		if(temp->number[i]>max){
			max=temp->number[i];
		}
		else if(temp->number[i]<min){
			min=temp->number[i];
		}
	}
	*peopleMin=min;
	*peopleMax=max;
	return max-min;
}


///������ע,peopleMin/peopleMax�ӻ�ȦȦ
void peopleLabel(RECORD *temp, double dx, double labelX, double labelY, int peopleMin, int peopleMax, int i){
	char num[10];
	double r=dx*0.05;//�ص�Ǻ�Ȧ��С
	double fontA=GetFontAscent();//label�ĸ߶ȣ�����y������
	
	SetPenColor("Black");

	drawLabel(labelX,labelY,itoa(temp->number[i],num,10));
	//�����С���ӻ�ȦȦ
	if(temp->number[i]==peopleMin||temp->number[i]==peopleMax){
		MovePen(labelX+r,labelY);
		SetPenColor("Red");
		DrawArc(r,0,360);
	}
	SetPenColor("Blue");


}


//����+����������+��������
//����rpHeadZoom��ʼ���ڶ�Ӧ�ṹ��ַ��rpTailZoom�������ڶ�Ӧ�ṹ��ַ,
//sx,sy��ʵ����꣬�����о���; dx,dyÿ�����������仯��׼���
//i����������
void drawFoldLine(double sx, double sy, double dx, double dy, int peopleMin, int peopleMax,int dateNumber, int i){
	RECORD *temp;
	double nowPointY, lastPointY;//��ǰ���׼������deltaֵ����һ�����׼������deltaֵ
	double coordinateLabelY;
	int j=0,k=0;
	//double r=dx*0.05;//�ص�Ǻ�Ȧ��С
	//double fontA=GetFontAscent();

	temp=rpHeadZoom;
	coordinateLabelY = coordinateY-coordinateHeight*0.05;
	nowPointY=(temp->number[i]-peopleMin)*dy;//��׼������delta

	SetPenColor("Blue");
	drawLabel(sx, coordinateLabelY, temp->date);//��һ����ĺ���������
	
	lastPointY=nowPointY;

	//������ע
	peopleLabel(temp,dx, sx+dx*j, sy+nowPointY, peopleMin, peopleMax, i);
	//��һ����
	temp=temp->next;

	for(j=1;j<dateNumber;j++){
		MovePen(sx+dx*(j-1),sy+nowPointY);
		nowPointY=(temp->number[i]-peopleMin)*dy;
		DrawLine(dx,nowPointY-lastPointY);
		lastPointY=nowPointY;

		//���ں������ӡ
		for(k=0;k<=7;k++){//���ڴ�ӡ8λ,���ţ�������������
			drawLabel(sx+dx*j, coordinateLabelY, temp->date);
		}
		//������ע
		peopleLabel(temp,dx, sx+dx*j, sy+nowPointY, peopleMin, peopleMax,i);
		//��һ����
		temp=temp->next;
	}


}

//Ԥ��������ԭ���������
void connnect(RECORD *rp, RECORD *futurep){
	rp->next=futurep;
	futurep->prior=rp;
}

//����Ԥ�⣬ԭ��ƴ�������
void seperate(RECORD *rp){
	int i;
	RECORD *temp=NULL, *futureTemp;
	temp=rp;
	while(temp->next != NULL){//�ҵ������tail
		temp=temp->next;
	}
	
	for(i=1;i<=futureDays;i++){//�������futureDays�Σ��ҵ����ӵ�
		temp=temp->prior;
	}
	futureTemp=temp->next;
	temp->next=NULL;
	futureTemp->prior=NULL;
	free(futureTemp);
}

//���Ƶ��ף�����rpHeadZoom��rpTailZoom
void buttonLeftest(){
	int i,dateNumber;
	RECORD *temp;
	rpHeadZoom=rp;
	temp=rpHeadZoom;
	dateNumber = getDateNumber(rpHeadZoom,rpTailZoom);
	for(i=1;i<dateNumber;i++){
		temp=temp->next;
	}
	rpTailZoom=temp;
}

//���Ƶ��ף�����rpHeadZoom��rpTailZoom
void buttonRightest(){
	int i,dateNumber;
	RECORD *temp;
	temp=rp;
	while(temp->next !=NULL){//�ҵ�ԭ����tail
		temp=temp->next;
	}
	rpTailZoom=temp;
	dateNumber = getDateNumber(rpHeadZoom,rpTailZoom);

	for(i=1;i<dateNumber;i++){//�������dateNumber
		temp=temp->prior;
	}
	rpHeadZoom=temp;
}

//����һ��
void buttonLeft(){
	if(rpHeadZoom->prior != NULL){
		rpHeadZoom=rpHeadZoom->prior;
		rpTailZoom=rpTailZoom->prior;
	}
}

//����һ��
void buttonRight(){
	if(rpTailZoom->next != NULL){
		rpHeadZoom=rpHeadZoom->next;
		rpTailZoom=rpTailZoom->next;
	}
}

//�Զ������ �����û���������ں��������½��ͼ
//������½��Ǹ�ɶ��
void buttonCustomize(int day1,int month1,int year1,int day2,int month2, int year2,int upper, int lower){
	int date1, date2,count=0,flag=0,dateNumber=0;
	RECORD *temp=NULL;
	date1=year1*10000+month1*100+day1;
	date2=year2*10000+month2*100+day2;
	temp=rp;
	dateNumber = getDateNumber(rpHeadZoom,rpTailZoom);

	//���⴦��
	//������λ���򣬲����ı�
	if(date1>=date2){
		return;
	}
	//������ʼС��rpHeadZoom->date����rpHeadZoom=rp
	if(date1<atoi(temp->date)){
		rpHeadZoom=rp;
	}

	while(temp->next!=NULL){
		if(date1==atoi(temp->date)){
			rpHeadZoom=temp;
			break;
		}
		temp=temp->next;
	}

	while(temp->next!=NULL){
		count++;
		if(date2==atoi(temp->date)){
			rpTailZoom=temp;
			flag=1;//�ҵ�ĩβ
			break;
		}
		temp=temp->next;
	}
	//���⴦��
	//���ĩλ����������֮�⣬��ֵrp������ڸ�rpTailZoom
	if(flag==0){
		rpTailZoom=temp;
	}

	dateNumber=count++;
}



//Button�Ű溯��


//Button�Ű沿��
void addEditButton(int a,int b,void(*p)(),char *str){     //buttonλ�ã�a,b��a����(������)��b�����������꣩��*pΪ��ť�����ĺ���
	static double editW;						//�༭�����
	static double editX = 8;					//�༭��button��λ��x
	static double editY = 6;					//�༭��button��λ��y
	static double editScaleX = 6;					//�༭��button��ȼ��֮��
	static double editScaleY = 6;					//�༭��button�߶ȼ��֮��
	static double a0 = 4;						//�༭��һ��button�ܸ���

	double fontHeight = GetFontHeight();		//����߶�
	double h = fontHeight*2;					//�༭��button�߶�
	double sh = h/editScaleY;						//�༭��button���¼��
	static double w=0;							//�༭��button���
	double sw = w/editScaleX;						//�༭��button���Ҽ��
	editW=0.75*winWidth;
	w= editW/(a0*(1+1/editScaleX));
	if (button(GenUIID(a*10+b), editX+(a-1)*w+(a-1)*sw, editY+b*h+(b-1)*sh, w, h, str))           
		statusNumber=1;
		//p();//�˴���д����
}

void addViewButton(int a,int b,void(*p)(),char *str){     //buttonλ�ã�a,b��a����(������)��b�����������꣩��*pΪ��ť�����ĺ���
	static double viewW;						//��������
	static double viewX = 0.5;					//�����button��λ��x
	static double viewY = 0.5;					//�����button��λ��y
	static double viewScaleX = 6;					//�����button��ȼ��֮��
	static double viewScaleY = 6;					//�����button�߶ȼ��֮��
	static double a0 = 4;						//�����һ��button�ܸ���

	double fontHeight = GetFontHeight();		//����߶�
	double h = fontHeight*2;					//�����button�߶�
	double sh = h/viewScaleY;						//�����button���¼��
	static double w=0;							//�����button���
	double sw = w/viewScaleX;						//�����button���Ҽ��
	viewW=0.75*winWidth;
	w= viewW/(a0*(1+1/viewScaleX));
	if (button(GenUIID(a*100+b*10), viewX+(a-1)*w+(a-1)*sw, viewY+b*h+(b-1)*sh, w, h, str))
		
		p();//�˴���д����
}

#if defined(DEMO_MENU)
void drawMenu()
{   
	static char * menuListFile[] = {"�ļ�",  
		"��  | Ctrl-O", // ��ݼ��������[Ctrl-X]��ʽ�������ַ����Ľ�β
		"����",
		"�ر�",
		"Exit   | Ctrl-E"};
	static char * menuListEdit[] = {"�༭",
		"����",
		"�ر� | Ctrl-T"};
	static char * menuListHelp[] = {"����",
		"Use  | Ctrl-M",
		"About"};
	static char * selectedLabel = NULL;
	double fH = GetFontHeight();
	double x = 0;//fH/8;
	double y = winHeight;
	double h = fH*1.5; // �ؼ��߶�
	double w = TextStringWidth(menuListHelp[0])*2; // �ؼ����
	double wlist = TextStringWidth(menuListEdit[2])*1.2;
	double xindent = winHeight/20; // ����
	int    selection;
	char *p;
	// menu bar
	drawMenuBar(0,y-h,winWidth,h);
	// File �˵�
	selection = menuList(GenUIID(0), x, y-h, w, wlist, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));
     if( selection==1 ) {  p="test.txt";
	               OpenFiles(p); }
	 if( selection==2 )
	if( selection==4 )
		exit(-1); // choose to exit
	
	// Edit �˵�
	selection = menuList(GenUIID(0),x+w,  y-h, w, wlist,h, menuListEdit,sizeof(menuListEdit)/sizeof(menuListEdit[0]));
	if( selection==1 ) globalEdit=1;
	if(selection==2)globalEdit=0;

	
	// Help �˵�
	
	selection = menuList(GenUIID(0),x+2*w,y-h, w, wlist, h, menuListHelp,sizeof(menuListHelp)/sizeof(menuListHelp[0]));
    
	Prompt(promptNumber);
	
	Status(statusNumber);


	}
#endif // #if defined(DEMO_MENU)



void drawButtons(){	
			
	addViewButton(1,1,buttonZoomIn,"�Ŵ�");
	addViewButton(2,1,buttonZoomOut,"��С");
	addViewButton(3,1,err,"��Ӧ��Ļ");
	addViewButton(4,1,err,"����");
	addViewButton(1,2,err,"���Ƶ�ͷ");
	addViewButton(2,2,err,"����");
	addViewButton(3,2,err,"����");
	addViewButton(4,2,err,"���Ƶ�ͷ");
	addEditButton(1,5,editOnOff,"�༭ģʽ");
	addEditButton(1,4,err,"�½�����");
	addEditButton(1,3,err,"ɾ���������");
	addEditButton(1,2,err,"Ԥ��");
	addEditButton(1,1,err,"����Ԥ��ģʽ");
	
}