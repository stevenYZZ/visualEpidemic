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

//本程序自带头文件
#include "common.h"
#include "linklistMY.h"
#include "visualization.h"
#include "edit.h"
//预测链表有三天
#define futureDays 3
#define DEMO_MENU


//全局变量
double winWidth, winHeight;   // 窗口尺寸
double coordinateX,coordinateY;  //坐标系的左下角点
double coordinateWidth, coordinateHeight;  //坐标系的x/y轴长度，带drawArea();里确定
RECORD *rp=NULL, *rpHeadZoom=NULL,*rpTailZoom=NULL;
KEY *kp=NULL;

//实验函数
void err(){
	exit(-1);
}

//界面刷新
//所有button前端也要复制到这个函数里面

void display(){
	//塞入各部分可视化函数
	DisplayClear();  //清屏函数，自带
	drawPic();
	drawButtons();
	#if defined(DEMO_MENU)
	// 绘制和处理菜单
	drawMenu();
#endif
}


void drawPic(){
	int i, n=-1, peopleDelta=-1;
	int dateNumber;//首末日期之间天数
	double sx,sy;//第一天+人数min时起笔点坐标，与轴有距离
	double dx,dy;//每个点横纵坐标变化标准间隔
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

	//折线绘制
	for(i=0;i<n;i++){
		drawFoldLine(sx,sy,dx,dy,peopleMin,peopleMax,dateNumber,i);  
	}
	
}

//纵坐标标度横线,无数字（参照线，可有可无）
void drawYLine(double sy,double dy, int peopleMax,int peopleMin,int peopleDelta){
	double d10,ny;//ny即nowY
	int i;
	d10=peopleDelta/5*dy;//6条线，5个区间
	ny=sy;

	SetPenColor("Gray");

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

	areaX=winWidth*0.1;
	areaY=winHeight*0.25;
	areaWidth=winWidth*0.6;
	areaHeight=winHeight*0.5;

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
void buttonZoomIn(){
	if(judgeZoomIn()){//放大则头尾相反
		rpHeadZoom=rpHeadZoom->next;
		rpTailZoom=rpTailZoom->prior;
	}
	else{
		return;
	}
}



//缩小调用
void buttonZoomOut(){
	RECORD *temp;
	if(rpHeadZoom->prior == NULL && rpTailZoom->next == NULL){//没有缩小余地，两端都到头
		return;
	}
	if(rpHeadZoom->prior!=NULL&&rpTailZoom->next!=NULL){//两端都没到头
		rpHeadZoom=rpHeadZoom->prior;
		rpTailZoom=rpTailZoom->next;
	}
	else if(rpHeadZoom->prior==NULL){//首到头，判断尾部
		temp=rpTailZoom->next;
		if(temp->next !=NULL){//尾端可以缩两个
			rpTailZoom=temp->next;
			return;
		}
		else{//尾端缩一个
			rpTailZoom=temp;
			return;
		}
	}//else if
	else if(rpTailZoom->next == NULL){
		temp=rpHeadZoom->prior;
		if(temp->prior!=NULL){//头部可以缩两个
			rpHeadZoom=temp->prior;
			return;
		}
		else{//头部缩一个
			rpHeadZoom=temp;
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


///人数标注,peopleMin/peopleMax加画圈圈
void peopleLabel(RECORD *temp, double dx, double labelX, double labelY, int peopleMin, int peopleMax, int i){
	char num[10];
	double r=dx*0.05;//重点记号圈大小
	double fontA=GetFontAscent();//label的高度，控制y坐标用
	
	SetPenColor("Black");

	drawLabel(labelX,labelY,itoa(temp->number[i],num,10));
	//最大最小给加画圈圈
	if(temp->number[i]==peopleMin||temp->number[i]==peopleMax){
		MovePen(labelX+r,labelY);
		SetPenColor("Red");
		DrawArc(r,0,360);
	}
	SetPenColor("Blue");


}


//折线+横坐标日期+人数标明
//参数rpHeadZoom起始日期对应结构地址，rpTailZoom结束日期对应结构地址,
//sx,sy起笔点坐标，与轴有距离; dx,dy每个点横纵坐标变化标准间隔
//i现在哪条线
void drawFoldLine(double sx, double sy, double dx, double dy, int peopleMin, int peopleMax,int dateNumber, int i){
	RECORD *temp;
	double nowPointY, lastPointY;//当前点基准线以上delta值，上一个点基准线以上delta值
	double coordinateLabelY;
	int j=0,k=0;
	//double r=dx*0.05;//重点记号圈大小
	//double fontA=GetFontAscent();

	temp=rpHeadZoom;
	coordinateLabelY = coordinateY-coordinateHeight*0.05;
	nowPointY=(temp->number[i]-peopleMin)*dy;//基准线以上delta

	SetPenColor("Blue");
	drawLabel(sx, coordinateLabelY, temp->date);//第一个点的横坐标日期
	
	lastPointY=nowPointY;

	//人数标注
	peopleLabel(temp,dx, sx+dx*j, sy+nowPointY, peopleMin, peopleMax, i);
	//下一个点
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
		//人数标注
		peopleLabel(temp,dx, sx+dx*j, sy+nowPointY, peopleMin, peopleMax,i);
		//下一个点
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



//Button排版函数


//Button排版部分
void addEditButton(int a,int b,void(*p)(),char *str){     //button位置（a,b）a列数(横坐标)，b行数（纵坐标），*p为按钮触发的函数
	static double editW;						//编辑区宽度
	static double editX = 8;					//编辑区button首位置x
	static double editY = 6;					//编辑区button首位置y
	static double editScaleX = 6;					//编辑区button宽度间距之比
	static double editScaleY = 6;					//编辑区button高度间距之比
	static double a0 = 4;						//编辑区一行button总个数

	double fontHeight = GetFontHeight();		//字体高度
	double h = fontHeight*2;					//编辑区button高度
	double sh = h/editScaleY;						//编辑区button上下间隔
	static double w=0;							//编辑区button宽度
	double sw = w/editScaleX;						//编辑区button左右间隔
	editW=0.75*winWidth;
	w= editW/(a0*(1+1/editScaleX));
	if (button(GenUIID(a*10+b), editX+(a-1)*w+(a-1)*sw, editY+b*h+(b-1)*sh, w, h, str))           
		statusNumber=1;
		//p();//此处填写函数
}

void addViewButton(int a,int b,void(*p)(),char *str){     //button位置（a,b）a列数(横坐标)，b行数（纵坐标），*p为按钮触发的函数
	static double viewW;						//浏览区宽度
	static double viewX = 0.5;					//浏览区button首位置x
	static double viewY = 0.5;					//浏览区button首位置y
	static double viewScaleX = 6;					//浏览区button宽度间距之比
	static double viewScaleY = 6;					//浏览区button高度间距之比
	static double a0 = 4;						//浏览区一行button总个数

	double fontHeight = GetFontHeight();		//字体高度
	double h = fontHeight*2;					//浏览区button高度
	double sh = h/viewScaleY;						//浏览区button上下间隔
	static double w=0;							//浏览区button宽度
	double sw = w/viewScaleX;						//浏览区button左右间隔
	viewW=0.75*winWidth;
	w= viewW/(a0*(1+1/viewScaleX));
	if (button(GenUIID(a*100+b*10), viewX+(a-1)*w+(a-1)*sw, viewY+b*h+(b-1)*sh, w, h, str))
		
		p();//此处填写函数
}

#if defined(DEMO_MENU)
void drawMenu()
{   
	static char * menuListFile[] = {"文件",  
		"打开  | Ctrl-O", // 快捷键必须采用[Ctrl-X]格式，放在字符串的结尾
		"保存",
		"关闭",
		"Exit   | Ctrl-E"};
	static char * menuListEdit[] = {"编辑",
		"开启",
		"关闭 | Ctrl-T"};
	static char * menuListHelp[] = {"帮助",
		"Use  | Ctrl-M",
		"About"};
	static char * selectedLabel = NULL;
	double fH = GetFontHeight();
	double x = 0;//fH/8;
	double y = winHeight;
	double h = fH*1.5; // 控件高度
	double w = TextStringWidth(menuListHelp[0])*2; // 控件宽度
	double wlist = TextStringWidth(menuListEdit[2])*1.2;
	double xindent = winHeight/20; // 缩进
	int    selection;
	char *p;
	// menu bar
	drawMenuBar(0,y-h,winWidth,h);
	// File 菜单
	selection = menuList(GenUIID(0), x, y-h, w, wlist, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));
     if( selection==1 ) {  p="test.txt";
	               OpenFiles(p); }
	 if( selection==2 )
	if( selection==4 )
		exit(-1); // choose to exit
	
	// Edit 菜单
	selection = menuList(GenUIID(0),x+w,  y-h, w, wlist,h, menuListEdit,sizeof(menuListEdit)/sizeof(menuListEdit[0]));
	if( selection==1 ) globalEdit=1;
	if(selection==2)globalEdit=0;

	
	// Help 菜单
	
	selection = menuList(GenUIID(0),x+2*w,y-h, w, wlist, h, menuListHelp,sizeof(menuListHelp)/sizeof(menuListHelp[0]));
    
	Prompt(promptNumber);
	
	Status(statusNumber);


	}
#endif // #if defined(DEMO_MENU)



void drawButtons(){	
			
	addViewButton(1,1,buttonZoomIn,"放大");
	addViewButton(2,1,buttonZoomOut,"缩小");
	addViewButton(3,1,err,"适应屏幕");
	addViewButton(4,1,err,"备用");
	addViewButton(1,2,err,"左移到头");
	addViewButton(2,2,err,"左移");
	addViewButton(3,2,err,"右移");
	addViewButton(4,2,err,"右移到头");
	addEditButton(1,5,editOnOff,"编辑模式");
	addEditButton(1,4,err,"新建日期");
	addEditButton(1,3,err,"删除最后日期");
	addEditButton(1,2,err,"预测");
	addEditButton(1,1,err,"更改预测模式");
	
}