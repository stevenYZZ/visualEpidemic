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

//预测链表有三天
#define futureDays 3

////生成链表所需
//struct record{
//    char date[10];                  //日期
//    int number[10];                 //各组数据人数
//    struct record *prior, *next;    //链表指针
//};
//typedef struct record RECORD;
//
//struct key{
//    char name[20];                  //每组数据字段名
//    struct key *prior, *next;    //链表指针
//};
//typedef struct key KEY;


//最后extern进来

RECORD *rp=NULL, *rpHeadZoom=NULL,*rpTailZoom=NULL;
KEY *kp=NULL;

double windowWidth, windowHeight;  //窗口总长宽

double coordinateX,coordinateY;  //坐标系的左下角点
double coordinateWidth, coordinateHeight;  //坐标系的x/y轴长度，带drawArea();里确定


void drawPic();  //相当于display()刷新
void drawArea();  //绘制坐标系区域（大长方形）
void getOriginalDateHeadTail(RECORD *rp,RECORD *rpHeadZoom, RECORD *rpTailZoom);
int getDateNumber(RECORD *rpHeadZoom, RECORD *rpTailZoom);
void drawFoldLine(double sx, double sy, double dx, double dy, int peopleMin, int peopleMax,int dateNumber, int i);  //绘制折线
int getKeyNumber(KEY *p);//总共多少条线
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


	//录入数据
	//RECORD
	RECORD *rpHead=NULL,*rpTail=NULL;
	KEY *kpHead=NULL, *kpTail=NULL;
	char datex[10]="2020";
	char namex[10]="first";
	int num1,num2;

	rpHead=newLinkRECORD();
	rp=rpHead;
	//总共录入1+3=4条
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
		//datex不变
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
	//总共录入2条
	strcpy(kpHead->name,namex);
	kpTail=kpHead;
	kpHead=addLinkKEY(kpHead,kpTail,namex);
	kpTail=kpTail->next;
	//录入结束



	//可视化模块开始
	SetWindowTitle("疫情分析");
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

//界面刷新
//所有button前端也要复制到这个函数里面
void drawPic(){
	int i, n=-1, peopleDelta=-1;
	int dateNumber;//首末日期之间天数
	double sx,sy;//第一天+人数min时起笔点坐标，与轴有距离
	double dx,dy;//每个点横纵坐标变化标准间隔
	int peopleMin=-1,peopleMax=-1;
	DisplayClear();  //清屏函数，自带

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
		drawFoldLine(sx,sy,dx,dy,peopleMin,peopleMax,dateNumber,i);  //折线绘制
	}
	
}

//纵坐标标度横线,无数字（参照线，可有可无）
void drawYLine(double sy,double dy, int peopleMax,int peopleMin,int peopleDelta){
	double d10,ny;//ny即nowY
	int i;
	d10=peopleDelta/5*dy;//6条线，5个区间
	ny=sy;

	for(i=0;i<=5;i++){
		ny=sy+i*d10;
		MovePen(coordinateX,ny);
		DrawLine(coordinateWidth,0);
	}
}


//draw坐标系外的大长方形以及坐标系x/y轴
void drawArea(){
	double areaWidth, areaHeight;  //长方形area的宽、长
	double areaX, areaY;  //图像area左下角点

	areaX=windowWidth*0.1;
	areaY=windowHeight*0.25;
	areaWidth=windowWidth*0.6;
	areaHeight=windowHeight*0.5;

	coordinateX=areaX+areaWidth*0.1;
	coordinateY=areaY+areaHeight*0.1;

	coordinateHeight=areaHeight*0.8;  //坐标系横轴占areaWidth的0.8
	coordinateWidth=areaWidth*0.8;  //坐标系纵轴占areaHeight的0.8

	MovePen(areaX,areaY);
	DrawLine(0,areaHeight);
	DrawLine(areaWidth,0);
	DrawLine(0,-areaHeight);
	DrawLine(-areaWidth,0);

	//小箭头的大小直接画的时候调数字，目前是纵轴长度coordinateHeight*0.05
	//y轴
	MovePen(coordinateX,coordinateY);
	DrawLine(0,coordinateHeight);
	//小箭头
	DrawLine(coordinateHeight*0.05,-coordinateHeight*0.05);
	MovePen(coordinateX,coordinateY+coordinateHeight);
	DrawLine(-coordinateHeight*0.05,-coordinateHeight*0.05);
	//x轴
	MovePen(coordinateX,coordinateY);
	DrawLine(coordinateWidth,0);
	//小箭头
	DrawLine(-coordinateHeight*0.05,+coordinateHeight*0.05);
	MovePen(coordinateX+coordinateWidth,coordinateY);
	DrawLine(-coordinateHeight*0.05,-coordinateHeight*0.05);
}

/*
*  功能：get有几条线
*  参数：p为KEY结构链表首地址
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


//判断是否继续放大
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
	if (count <= 1){  //仅剩<=3个数据
		return 0;
	}
	else{  //剩余>=4个数据
		return 1;
	}
}

//放大调用
void buttonZoomIn(int *dateNumber){
	if(judgeZoomIn()){//放大则头尾相反
		rpHeadZoom=rpHeadZoom->next;
		rpTailZoom=rpTailZoom->prior;
		(*dateNumber)=(*dateNumber)-2;
	}
	else{
		return;
	}
}



//缩小调用
void buttonZoomOut(int *dateNumber){
	RECORD *temp;
	if(rpHeadZoom->prior == NULL && rpTailZoom->next == NULL){//没有缩小余地，两端都到头
		return;
	}
	if(rpHeadZoom->prior!=NULL&&rpTailZoom->next!=NULL){//两端都没到头
		rpHeadZoom=rpHeadZoom->prior;
		rpTailZoom=rpTailZoom->next;
		(*dateNumber)=(*dateNumber)+2;
	}
	else if(rpHeadZoom->prior==NULL){//首到头，判断尾部
		temp=rpTailZoom->next;
		if(temp->next !=NULL){//尾端可以缩两个
			rpTailZoom=temp->next;
			(*dateNumber)=(*dateNumber)+2;
			return;
		}
		else{//尾端缩一个
			rpTailZoom=temp;
			(*dateNumber)=(*dateNumber)+1;
			return;
		}
	}//else if
	else if(rpTailZoom->next == NULL){
		temp=rpHeadZoom->prior;
		if(temp->prior!=NULL){//头部可以缩两个
			rpHeadZoom=temp->prior;
			(*dateNumber)=(*dateNumber)+2;
			return;
		}
		else{//头部缩一个
			rpHeadZoom=temp;
			(*dateNumber)=(*dateNumber)+1;
			return;
		}
	}//else if
}

//得dateNumber总天数和总的头尾地址
void getOriginalDateHeadTail(RECORD *rp,RECORD *rpHeadZoom, RECORD *rpTailZoom){
	RECORD *temp=rp;

	while (temp->next != NULL){
		temp=temp->next;
	}

	rpHeadZoom=rp;
	rpTailZoom=temp;
}

//原始/缩放后头尾之间天数
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

//得到这个文件中数据，人数的最大值、最小值，返回max和min的差值，用作y轴坐标确定
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


//折线+横坐标日期
//参数rpHeadZoom起始日期对应结构地址，rpTailZoom结束日期对应结构地址,
//sx,sy起笔点坐标，与轴有距离; dx,dy每个点横纵坐标变化标准间隔
//i现在哪条线
void drawFoldLine(double sx, double sy, double dx, double dy, int peopleMin, int peopleMax,int dateNumber, int i){
	RECORD *temp;
	double nowPointY, lastPointY;//当前点基准线以上delta值，上一个点基准线以上delta值
	double coordinateLabelY;
	char num[10];
	int j=0,k=0;
	double r=dx*0.05;//重点记号圈大小
	double fontA=GetFontAscent();

	temp=rpHeadZoom;
	coordinateLabelY = coordinateY-coordinateHeight*0.05;
	nowPointY=(temp->number[i]-peopleMin)*dy;//基准线以上delta

	SetPenColor("Blue");
	drawLabel(sx, coordinateLabelY, temp->date );//第一个点的横坐标日期
	
	lastPointY=nowPointY;

	//最大最小人数标明
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

		//日期横坐标打印
		for(k=0;k<=7;k++){//日期打印8位,横排，首数字与点对齐
			drawLabel(sx+dx*j, coordinateLabelY, temp->date);
		}


		//最大最小人数标明
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

//预测链表与原链表的连接
void connnect(RECORD *rp, RECORD *futurep){
	rp->next=futurep;
	futurep->prior=rp;
}

//重新预测，原本拼接链表拆开
void seperate(RECORD *rp){
	int i;
	RECORD *temp=NULL, *futureTemp;
	temp=rp;
	while(temp->next != NULL){//找到链表的tail
		temp=temp->next;
	}
	
	for(i=1;i<=futureDays;i++){//反向遍历futureDays次，找到连接点
		temp=temp->prior;
	}
	futureTemp=temp->next;
	temp->next=NULL;
	futureTemp->prior=NULL;
	free(futureTemp);
}

//左移到底，更新rpHeadZoom和rpTailZoom
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

//右移到底，更新rpHeadZoom和rpTailZoom
void buttonRightest(){
	int i,dateNumber;
	RECORD *temp;
	temp=rp;
	while(temp->next !=NULL){//找到原链表tail
		temp=temp->next;
	}
	rpTailZoom=temp;
	dateNumber = getDateNumber(rpHeadZoom,rpTailZoom);

	for(i=1;i<dateNumber;i++){//反向遍历dateNumber
		temp=temp->prior;
	}
	rpHeadZoom=temp;
}

//左移一天
void buttonLeft(){
	if(rpHeadZoom->prior != NULL){
		rpHeadZoom=rpHeadZoom->prior;
		rpTailZoom=rpTailZoom->prior;
	}
}

//右移一天
void buttonRight(){
	if(rpTailZoom->next != NULL){
		rpHeadZoom=rpHeadZoom->next;
		rpTailZoom=rpTailZoom->next;
	}
}

//自定义比例 根据用户输入的日期和人数上下界绘图
//这个上下界是个啥？
void buttonCustomize(int day1,int month1,int year1,int day2,int month2, int year2,int upper, int lower){
	int date1, date2,count=0,flag=0,dateNumber=0;
	RECORD *temp=NULL;
	date1=year1*10000+month1*100+day1;
	date2=year2*10000+month2*100+day2;
	temp=rp;
	dateNumber = getDateNumber(rpHeadZoom,rpTailZoom);

	//特殊处理
	//日期首位反向，不做改变
	if(date1>=date2){
		return;
	}
	//日期起始小于rpHeadZoom->date，则rpHeadZoom=rp
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
			flag=1;//找到末尾
			break;
		}
		temp=temp->next;
	}
	//特殊处理
	//如果末位日期在链表之外，则赋值rp最后日期给rpTailZoom
	if(flag==0){
		rpTailZoom=temp;
	}

	dateNumber=count++;
}