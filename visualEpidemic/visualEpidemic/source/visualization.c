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

//全局变量
double winWidth, winHeight;   // 窗口尺寸
double coordinateX,coordinateY;  //坐标系的左下角点
double coordinateWidth, coordinateHeight;  //坐标系的x/y轴长度，带drawArea();里确定
double highLight[10][4];  //是否高亮，[][0]x坐标，[][1]y坐标，[][2]是否高亮，[][4]位置序号（越小越下面）
double highLightBoxdx; //判断高亮要用到box的宽

RECORD *rp=NULL, *rpHeadZoom=NULL,*rpTailZoom=NULL;
KEY *kp=NULL,*kpHead=NULL;



//界面刷新

void display(){
	//塞入各部分可视化函数
	DisplayClear();  //清屏函数，自带
	drawPic();
	drawButtons();
	drawMenu();// 绘制和处理菜单
	if(globalActive==0){
	
	}else if(globalActive==1){
		drawPrompt1();
	}else if(globalActive==2){
		drawPrompt2();
	}else if(globalActive==3){
		drawInputBox();
	}else if(globalActive==4){
		drawTipBox();
	}

	
}

void drawPic(){
	int i,j, n=-1, peopleDelta=-1;
	int dateNumber;//首末日期之间天数
	double sx,sy;//第一天+人数min时起笔点坐标，与轴有距离
	double dx,dy;//每个点横纵坐标变化标准间隔
	int peopleMin=-1,peopleMax=-1;
	double barDy;
	RECORD *p;

	drawArea();


	n = getKeyNumber(kp);
	peopleDelta = getTotalPeopleNumber(n,&peopleMax,&peopleMin);
	dateNumber = getDateNumber(rpHeadZoom,rpTailZoom);
	
	dx=coordinateWidth*0.9/(dateNumber);
	dy=coordinateHeight*0.9/peopleDelta;
	sx=coordinateWidth*0.05+coordinateX;
	sy=coordinateHeight*0.05+coordinateY;
	barDy=coordinateHeight*0.4/peopleMax;//柱状图最高占据y轴0.4

	drawYLine(sy,dy,peopleMax,peopleMin,peopleDelta);
	printDateX(sx,sy,dx,dy,dateNumber);

	highLightBoxdx=coordinateWidth/6;

	//折线绘制
	for(i=0;i<n;i++){
		drawFoldLine(sx,sy,dx,dy,peopleMin,peopleMax,dateNumber,i); 
	}

	//柱状图
	if(dx>0.1*coordinateX){
		p=rpHeadZoom;
		j=0;
		while(p->next!=rpTailZoom){
			drawBar(sx, dx, barDy,n, j, p);
			j++;
			p=p->next;
		}
		drawBar(sx, dx, barDy,n, j, p);
		j++;
		p=p->next;
		drawBar(sx, dx, barDy,n, j, p);
	}
	
	//标注线名字
	sortLineName(n);
	adjustLineName(n);
	for(i=0;i<n;i++){
		highLight[i][0]=sx+dx*(dateNumber-0.25);//暴力改bug可能治标不治本
		lineName(highLight[i][0],highLight[i][1],i);
	}
}

//画柱状图
void drawBar(double sx, double dx, double barDy,int n, int j, RECORD* p){
	double width,x,h;
	int i;
	width=dx*0.7/n;
	x=sx+dx*j-0.35*dx;
	SetPenColor("Brown");
	for(i=0;i<n;i++){
		x+=width;
		h=p->number[i]*barDy;
		drawRectangle(x,coordinateY, width,h , 0);
	}
}

//排序标号tag的位置，避免重合
//调整highLight[][4],序号越小越下面(0-6)
//n总共线数量
//#############################################################bug-更改highLight[1][0]开始改值
void sortLineName(int n){
	int i,j,num111;

	for(i=0;i<n;i++){
		num111=0;
		for(j=0;j<n;j++){
			if(i!=j && highLight[i][1]>=highLight[j][1]){
				num111++;
			}//if
		}//for j
		highLight[i][4]=num111;
	}//for i
	
}

int findSort(int n,int i){
	int j;
	for(j=0;j<n;j++){
		if(highLight[j][4]==i){
			break;
		}
	}
	return j;
}

void adjustLineName(int n){
	int i,mid,findn,middler;
	double fontA;
	fontA=GetFontAscent();
	mid=n/2;//固定位置的线作为基准，调上下;e.g.共7条，中位线是0-n中的3，即第四条

	middler=findSort(n,mid);
	for(i=(mid+1);i<n;i++){
		findn=findSort(n,i);
		if(highLight[findn][1]<(highLight[middler][1]+fontA)){
			highLight[findn][1]=highLight[middler][1]+fontA*1.2;
		}
		middler=findn;
	}
	middler=findSort(n,mid);
	for(i=(mid-1);i>=0;i--){
		findn=findSort(n,i);
		if((highLight[findn][1]+fontA)>highLight[middler][1]){
			highLight[findn][1]=highLight[middler][1]-fontA*1.2;
		}
		middler=findn;
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
	double fontH=GetFontHeight();  //字

	areaX=winWidth*0.1;
	areaY=winHeight*0.25;
	areaWidth=winWidth*0.6;
	areaHeight=winHeight*0.5;

	coordinateX=areaX+areaWidth*0.1;
	coordinateY=areaY+areaHeight*0.16;

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
	drawLabel(coordinateX-areaWidth*0.05,coordinateY+coordinateHeight-fontH*0,"n");
	drawLabel(coordinateX-areaWidth*0.05,coordinateY+coordinateHeight-fontH*1,"u");
	drawLabel(coordinateX-areaWidth*0.05,coordinateY+coordinateHeight-fontH*2,"m");
	drawLabel(coordinateX-areaWidth*0.05,coordinateY+coordinateHeight-fontH*3,"b");
	drawLabel(coordinateX-areaWidth*0.05,coordinateY+coordinateHeight-fontH*4,"e");
	drawLabel(coordinateX-areaWidth*0.05,coordinateY+coordinateHeight-fontH*5,"r");
	//小箭头
	MovePen(coordinateX,coordinateY+coordinateHeight);
	DrawLine(coordinateHeight*0.05,-coordinateHeight*0.05);
	MovePen(coordinateX,coordinateY+coordinateHeight);
	DrawLine(-coordinateHeight*0.05,-coordinateHeight*0.05);
	//x轴
	MovePen(coordinateX,coordinateY);
	DrawLine(coordinateWidth,0);
	drawLabel(coordinateX+coordinateWidth,coordinateY-fontH,"date");
	//小箭头
	MovePen(coordinateX+coordinateWidth,coordinateY);
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

////得dateNumber总天数和总的头尾地址
//void getOriginalDateHeadTail(RECORD *rp,RECORD *rpHeadZoom, RECORD *rpTailZoom){
//	RECORD *temp=rp;
//
//	while (temp->next != NULL){
//		temp=temp->next;
//	}
//
//	rpHeadZoom=rp;
//	rpTailZoom=temp;
//}

//原始/缩放后头尾之间天数
int getDateNumber(RECORD *rpHeadZoom, RECORD *rpTailZoom){
	int count=0;
	RECORD *temp;
	temp = rpHeadZoom;
	while(temp->next!=rpTailZoom){
		count++;
		temp=temp->next;
	}
	count++;
	count++;
	return count;
}

//得到这个文件中数据，人数的最大值、最小值，返回max和min的差值，用作y轴坐标确定
int getTotalPeopleNumber(int n,int *peopleMax, int *peopleMin){
	int i,max=0,min=rp->number[0];
	RECORD *temp=NULL;
	temp=rpHeadZoom;
	while(temp->next!=rpTailZoom){
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
	temp=temp->next;
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


//人数标注,peopleMin/peopleMax加画圈圈
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
	//判断高亮/不高亮  笔触颜色
	if(highLight[i][2]==0){
		SetPenColor("Blue");
	}
	else if(highLight[i][2]==1){
		SetPenColor("Red");
	}
	else{//报错黄色
		SetPenColor("Cyan");
	}
}

//图例标注哪条线
//(px, py)是当前线的label左下角坐标，dx控制box宽度，i是第几条线
void lineName(double px,double py, int i){
	int k;
	double fontA;
	double dx;
	KEY *temp;

	dx=coordinateWidth/6;
	temp=kp;
	fontA=GetFontAscent();
	//确定对应的KEY
	for(k=0;k<i;k++){
		temp=temp->next;
	}
	SetPenColor("Light Gray");
	drawBox(px, py, dx, fontA,1,temp->name, 'c', "Black");
}

//mouseEvent里判断鼠标是否点击box区域，需要高亮
void judgeHighLight(double mx,double my){
	int i,n;
	double fontA;

	fontA=GetFontAscent();
	n = getKeyNumber(kp);

	for(i=0;i<n;i++){
		if(mx>=highLight[i][0] && mx<=(highLight[i][0]+highLightBoxdx)){
			if(my>=highLight[i][1] && my<=highLight[i][1]+fontA){
				updateHighLight(i,n);
				break;
			}
		}
	}
	
}

//i新高亮线，n线总数
//更新高亮，包括清楚之前高亮和添加最新高亮
void updateHighLight(int i,int n){
	int j;
	for(j=0;j<n;j++){
		if(j!=i){
			highLight[j][2]=0;
		}
		else{
			highLight[i][2]=1;
		}
	}
}




//日期横坐标  竖版排列print函数
void printDateX(double sx, double sy, double dx, double dy,int dateNumber){
	RECORD *temp=NULL;
	double coordinateLabelY;
	char dateChange[10][2];
	int j,k;
	double fontA;
	
	temp=rpHeadZoom;
	fontA = GetFontAscent();
	coordinateLabelY = coordinateY-coordinateHeight*0.05;

	SetPenColor("Black");
	//第一个点的横坐标日期
	k=0;
	while(temp->date[k]!='\0'){
		dateChange[k][0]=temp->date[k];
		dateChange[k][1]='\0';
		drawLabel(sx, coordinateLabelY-k*fontA,dateChange[k]);
		k++;
	}
	dateChange[k][0]='\0';
	temp=temp->next;
	//后面点的横坐标日期
	for(j=1;j<dateNumber;j++){
		//横坐标
		//转换
		k=0;
		while(temp->date[k]!='\0'){
			dateChange[k][0]=temp->date[k];
			dateChange[k][1]='\0';
			drawLabel(sx+dx*j, coordinateLabelY-k*fontA,dateChange[k]);
			k++;
		}
		dateChange[k][0]='\0';
	}
}

//折线+横坐标日期+人数标明
//参数rpHeadZoom起始日期对应结构地址，rpTailZoom结束日期对应结构地址,
//sx,sy起笔点坐标，与轴有距离; dx,dy每个点横纵坐标变化标准间隔
//i现在哪条线
void drawFoldLine(double sx, double sy, double dx, double dy, int peopleMin, int peopleMax,int dateNumber, int i){
	RECORD *temp=NULL;
	double nowPointY, lastPointY;//当前点基准线以上delta值，上一个点基准线以上delta值
	int j=0,k=0;
	double fontA;
	char num[10];
	//double r=dx*0.05;//重点记号圈大小

	fontA=GetFontAscent();
	temp=rpHeadZoom;
	nowPointY=(temp->number[i]-peopleMin)*dy;//基准线以上delta

	//判断高亮/不高亮  笔触颜色
	//颜色改变记得去peopleLabel函数也改掉
	if(highLight[i][2]==0){
		SetPenColor("Blue");
	}
	else if(highLight[i][2]==1){
		SetPenColor("Red");
	}
	else{//报错黄色
		SetPenColor("Yellow");
	}
	//drawLabel(sx, coordinateLabelY, temp->date);//横排版本第一个点的横坐标日期
	
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

		////日期横坐标打印
		//for(k=0;k<=7;k++){//日期打印8位,横排，首数字与点对齐
		//	drawLabel(sx+dx*j, coordinateLabelY, temp->date);
		//}

		//人数标注
		peopleLabel(temp,dx, sx+dx*j, sy+nowPointY, peopleMin, peopleMax,i);
		//下一个点
		temp=temp->next;
	}

	highLight[i][0]=sx+dx*(j-0.25);
	highLight[i][1]=sy+nowPointY-fontA/2;
	////尾端标值，需要对应改highLight[][0]=sx+dx*(j-0.25)
	//drawLabel(sx+dx*(j-0.9),sy+nowPointY-fontA/2,itoa(rpTailZoom->number[i],num,10));
	
	//lineName(sx+dx*(j-0.75), sy+nowPointY,dx,i);
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
	static double editY = 3;					//编辑区button首位置y
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

	if(globalEdit==0)SetPenColor("Gray");

	if (button(GenUIID(a*10+b), editX+(a-1)*w+(a-1)*sw, editY+b*h+(b-1)*sh, w, h, str))
		p();//此处填写函数
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
    
	//Status(statusNumber);
	}
void drawButtons(){	
			
	addViewButton(1,1,buttonZoomIn,"放大");
	addViewButton(2,1,buttonZoomOut,"缩小");
	addViewButton(3,1,try0,"实验");
	addViewButton(4,1,err,"备用");
	addViewButton(1,2,buttonLeftest,"左移到头");
	addViewButton(2,2,buttonLeft,"左移");
	addViewButton(3,2,buttonRight,"右移");
	addViewButton(4,2,buttonRightest,"右移到头");
	addEditButton(1,8,editOnOff,globalEdit?"编辑模式:ON":"编辑模式:OFF");
	addEditButton(1,7,editNewDate,"新建日期");
	addEditButton(1,6,editDeleteLastDate,"删除最后日期");
	addEditButton(1,5,editChangeData,"修改参数");
	addEditButton(1,4,editNewLine,"新增线");
	addEditButton(1,3,editDeleteLine,"删除线");
	addEditButton(1,2,err,"预测");
	addEditButton(1,1,err,"更改预测模式");
	
}

void drawPrompt1(){
	double x=winWidth/2;
	double y=winHeight/2;
	double fH = GetFontHeight();
	double h = fH*1.5; // 控件高度
	SetPenColor("White");
	drawRectangle(x-1.5,y-1,4,2,1);
	SetPenColor("Black");
	drawRectangle(x-1.5,y-1,4,2,0);
	drawLabel(x-2+2*fH+1.5,y-2+fH+2.5,"尚未保存，确认退出？");
	if (button(GenUIID(0), x-2+2*fH+1,y-2+fH+1, 1, h, "确定")) exit(-1);
	if (button(GenUIID(1), x-2+2*fH+2.5,y-2+fH+1, 1, h, "取消"))globalActive=0;
}

void drawPrompt2(){
	double x=winWidth/2;
	double y=winHeight/2;
	double fH = GetFontHeight();
	double h = fH*1.5; // 控件高度
	SetPenColor("White");
	drawRectangle(x-1.5,y-1,4,2,1);
	SetPenColor("Black");
	drawRectangle(x-1.5,y-1,4,2,0);
	drawLabel(x-1,y-2+fH+2.5,"温馨提示：本程序无撤销功能，");
	drawLabel(x-0.5,y+0.5,"是否要进行当前操作？");
	if (button(GenUIID(0), x-2+2*fH+1,y-2+fH+1, 1, h, "是")){
		if(popStatus2=0);
		else if(popStatus2=1);
	}
	if (button(GenUIID(1), x-2+2*fH+2.5,y-2+fH+1, 1, h, "否"))globalActive=0;
}

void drawInputBox(){
	double x=winWidth/2;
	double y=winHeight/2;
	double fH = GetFontHeight();
	double h = fH*1.5; // 控件高度
	SetPenColor("White");
	drawRectangle(x-1.5,y-1,4,2,1);
	SetPenColor("Black");
	drawRectangle(x-1.5,y-1,4,2,0);
	drawLabel(x-2+2*fH+1.5,y-2+fH+2.5,popInputTip);
	if (button(GenUIID(0), x-2+2*fH+1,y-2+fH+1, 1, h, "确定")){
	
	}
	if (button(GenUIID(1), x-2+2*fH+2.5,y-2+fH+1, 1, h, "取消"))globalActive=0;
	if( textbox(GenUIID(0), x, y, 2, h, popInput, sizeof(popInput) ) )
	;
}
	//{	double v = 0;
	//	sscanf(popInput, "%lf", &v);
	//	return v;
	//}

void drawTipBox(){
	double x=winWidth/2;
	double y=winHeight/2;
	double fH = GetFontHeight();
	double h = fH*1.5; // 控件高度
	SetPenColor("White");
	drawRectangle(x-1.5,y-1,4,2,1);
	SetPenColor("Black");
	drawRectangle(x-1.5,y-1,4,2,0);
	drawLabel(x+2*fH-1,y-2+fH+2.5,popTip);
	if (button(GenUIID(0), x+2*fH-0.5,y-2+fH+1, 1, h, "确定")) globalActive=0;
}