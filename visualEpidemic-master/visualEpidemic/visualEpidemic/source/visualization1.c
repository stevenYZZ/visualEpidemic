#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "linklistMY.h"

//Ԥ������������
#define futureDays 3

////������������
//struct record{
//    char date[10];                  //����
//    int number[10];                 //������������
//    struct record *prior, *next;    //����ָ��
//};
//typedef struct record RECORD;
//
//struct key{
//    char name[20];                  //ÿ�������ֶ���
//    struct key *prior, *next;    //����ָ��
//};
//typedef struct key KEY;


//���extern����

RECORD *rp=NULL, *rpHeadZoom=NULL,*rpTailZoom=NULL;
KEY *kp=NULL;

double windowWidth, windowHeight;  //�����ܳ���

double coordinateX,coordinateY;  //����ϵ�����½ǵ�
double coordinateWidth, coordinateHeight;  //����ϵ��x/y�᳤�ȣ���drawArea();��ȷ��


void drawPic();  //�൱��display()ˢ��
void drawArea();  //��������ϵ���򣨴󳤷��Σ�
void getOriginalDateHeadTail(RECORD *rp,RECORD *rpHeadZoom, RECORD *rpTailZoom);
int getDateNumber(RECORD *rpHeadZoom, RECORD *rpTailZoom);
void drawFoldLine(double sx, double sy, double dx, double dy, int peopleMin, int peopleMax,int dateNumber, int i);  //��������
int getKeyNumber(KEY *p);//�ܹ���������
void drawYLine(double sy,double dy, int peopleMax,int peopleMin,int peopleDelta);
int judgeZoomIn();
void buttonZoomIn(int *dateNumber);
void buttonZoomOut(int *dateNumber);
int getTotalPeopleNumber(RECORD *rp,int n,int *peopleMax, int*peopleMin);
void connnect(RECORD *rp, RECORD *futurep);
void seperate(RECORD *rp);
void buttonLeftest();
void buttonRight();
void buttonCustomize(int day1,int month1,int year1,int day2,int month2, int year2,int upper, int lower);

void Main(){
	RECORD *temp=NULL;
	int i, n;


	//¼������
	//RECORD
	RECORD *rpHead=NULL,*rpTail=NULL;
	KEY *kpHead=NULL, *kpTail=NULL;
	char datex[10]="2020";
	char namex[10]="first";
	int num1,num2;

	rpHead=newLinkRECORD();
	rp=rpHead;
	//�ܹ�¼��1+3=4��
	/*for(i=0;i<=2;i++){
		datex[i]=4;
	}
	datex[i]='\0';*/
	num1=1;
	num2=2;
	strcpy(rpHead->date,datex);
	rpHead->number[0]=num1;
	rpHead->number[1]=num2;
	rpTail=rpHead;

	for(i=0;i<=2;i++){
		//datex����
		num1=num1+10*i;
		num2=num2+20*i;
		rpHead=addLinkRECORD(rpHead,rpTail,datex,num1,num2);
		rpTail=rpTail->next;
	}
	rpHeadZoom=rp;
	rpTailZoom=rpTail;

	//KEY
	kpHead=newLinkKEY();
	kp=kpHead;
	//�ܹ�¼��2��
	strcpy(kpHead->name,namex);
	kpTail=kpHead;
	kpHead=addLinkKEY(kpHead,kpTail,namex);
	kpTail=kpTail->next;
	//¼�����



	//���ӻ�ģ�鿪ʼ
	SetWindowTitle("�������");
	InitGraphics();

	windowWidth=GetWindowWidth();
	windowHeight=GetWindowHeight();

	//dateTail original
	n = getKeyNumber(kp);
	temp=rp;
	if (n != 0){
		getOriginalDateHeadTail(rp,rpHeadZoom,rpTailZoom);
	}

	drawPic();

}

//����ˢ��
//����buttonǰ��ҲҪ���Ƶ������������
void drawPic(){
	int i, n=-1, peopleDelta=-1;
	int dateNumber;//��ĩ����֮������
	double sx,sy;//��һ��+����minʱ��ʵ����꣬�����о���
	double dx,dy;//ÿ�����������仯��׼���
	int peopleMin=-1,peopleMax=-1;
	DisplayClear();  //�����������Դ�

	drawArea();

	n = getKeyNumber(kp);
	peopleDelta = getTotalPeopleNumber(rp,n,&peopleMax,&peopleMin);
	dateNumber = getDateNumber(rpHeadZoom,rpTailZoom);
	
	dx=coordinateWidth*0.9/(dateNumber+futureDays);
	dy=coordinateHeight*0.9/peopleDelta;
	sx=coordinateWidth*0.05+coordinateX;
	sy=coordinateHeight*0.05+coordinateY;

	drawYLine(sy,dy,peopleMax,peopleMin,peopleDelta);

	for(i=0;i<n;i++){
		drawFoldLine(sx,sy,dx,dy,peopleMin,peopleMax,dateNumber,i);  //���߻���
	}
	
}

//�������Ⱥ���,�����֣������ߣ����п��ޣ�
void drawYLine(double sy,double dy, int peopleMax,int peopleMin,int peopleDelta){
	double d10,ny;//ny��nowY
	int i;
	d10=peopleDelta/5*dy;//6���ߣ�5������
	ny=sy;

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

	areaX=windowWidth*0.1;
	areaY=windowHeight*0.25;
	areaWidth=windowWidth*0.6;
	areaHeight=windowHeight*0.5;

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
void buttonZoomIn(int *dateNumber){
	if(judgeZoomIn()){//�Ŵ���ͷβ�෴
		rpHeadZoom=rpHeadZoom->next;
		rpTailZoom=rpTailZoom->prior;
		(*dateNumber)=(*dateNumber)-2;
	}
	else{
		return;
	}
}



//��С����
void buttonZoomOut(int *dateNumber){
	RECORD *temp;
	if(rpHeadZoom->prior == NULL && rpTailZoom->next == NULL){//û����С��أ����˶���ͷ
		return;
	}
	if(rpHeadZoom->prior!=NULL&&rpTailZoom->next!=NULL){//���˶�û��ͷ
		rpHeadZoom=rpHeadZoom->prior;
		rpTailZoom=rpTailZoom->next;
		(*dateNumber)=(*dateNumber)+2;
	}
	else if(rpHeadZoom->prior==NULL){//�׵�ͷ���ж�β��
		temp=rpTailZoom->next;
		if(temp->next !=NULL){//β�˿���������
			rpTailZoom=temp->next;
			(*dateNumber)=(*dateNumber)+2;
			return;
		}
		else{//β����һ��
			rpTailZoom=temp;
			(*dateNumber)=(*dateNumber)+1;
			return;
		}
	}//else if
	else if(rpTailZoom->next == NULL){
		temp=rpHeadZoom->prior;
		if(temp->prior!=NULL){//ͷ������������
			rpHeadZoom=temp->prior;
			(*dateNumber)=(*dateNumber)+2;
			return;
		}
		else{//ͷ����һ��
			rpHeadZoom=temp;
			(*dateNumber)=(*dateNumber)+1;
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


//����+����������
//����rpHeadZoom��ʼ���ڶ�Ӧ�ṹ��ַ��rpTailZoom�������ڶ�Ӧ�ṹ��ַ,
//sx,sy��ʵ����꣬�����о���; dx,dyÿ�����������仯��׼���
//i����������
void drawFoldLine(double sx, double sy, double dx, double dy, int peopleMin, int peopleMax,int dateNumber, int i){
	RECORD *temp;
	double nowPointY, lastPointY;//��ǰ���׼������deltaֵ����һ�����׼������deltaֵ
	double coordinateLabelY;
	char num[10];
	int j=0,k=0;
	double r=dx*0.05;//�ص�Ǻ�Ȧ��С
	double fontA=GetFontAscent();

	temp=rpHeadZoom;
	coordinateLabelY = coordinateY-coordinateHeight*0.05;
	nowPointY=(temp->number[i]-peopleMin)*dy;//��׼������delta

	SetPenColor("Blue");
	drawLabel(sx, coordinateLabelY, temp->date );//��һ����ĺ���������
	
	lastPointY=nowPointY;

	//�����С��������
	SetPenColor("Red");
	if(temp->number[i]==peopleMax){
		drawLabel(sx+dx*j-r,sy+nowPointY+r,itoa(temp->number[i],num,10));
		MovePen(sx+dx*j+r,sy+nowPointY);
		DrawArc(r,0,360);
	}
	else if(temp->number[i]==peopleMin){
		drawLabel(sx+dx*j-r,sy+nowPointY-fontA,itoa(temp->number[i],num,10));
		MovePen(sx+dx*j+r,sy+nowPointY);
		DrawArc(r,0,360);
	}
	SetPenColor("Blue");


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


		//�����С��������
		SetPenColor("Red");
		if(temp->number[i]==peopleMax){
			drawLabel(sx+dx*j-r,sy+nowPointY+r,itoa(temp->number[i],num,10));
			MovePen(sx+dx*j+r,sy+nowPointY);
			DrawArc(r,0,360);
		}
		else if(temp->number[i]==peopleMin){
			drawLabel(sx+dx*j-r,sy+nowPointY-fontA,itoa(temp->number[i],num,10));
			MovePen(sx+dx*j+r,sy+nowPointY);
			DrawArc(r,0,360);
		}
		SetPenColor("Blue");

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