#include "common.h"
#include "visualization.h"
#include "edit.h"
#include "files.h"
#include "predict.h"

//全局变量
double winWidth, winHeight;   // 窗口尺寸
double coordinateX, coordinateY;  //坐标系的左下角点
double coordinateWidth, coordinateHeight;  //坐标系的x/y轴长度，带drawArea();里确定

double highLight[10][4];  //是否高亮，[][0]x坐标，[][1]y坐标，[][2]是否高亮，[][4]位置序号（越小越下面）
double highLightBoxdx; //判断高亮要用到box的宽
int peopleMin = -1, peopleMax = -1, peopleDelta = -1;

RECORD *rpHead = NULL,*rpTail = NULL;
KEY *kpHead = NULL, *kpTail = NULL;

RECORD *rp = NULL, *rpHeadZoom = NULL, *rpTailZoom = NULL;
KEY *kp = NULL;
//局部变量
static char color[10][10]={"Brown", "Pinkish", "Cream", "LightYell", "ThickYell", "Orange","SeaBlue","DarkBlue","CyanGray","LightPur"};
//杂七杂八函数
void visualTip(){
	globalActive = 4;
	strcpy(popTip, "当前未打开或新建文件");
}

void buttonLabel(){
	commonTF(&globalLabel);
}

void buttonFitIn(){
	commonTF(&globalFitIn);
}

void buttonDate(){
	commonTF(&globalDate);
}


//界面刷新
//塞入各部分可视化函数
void display(){

	double buttonBackX, buttonBackHeight, buttonBackWidth;
	double fontA;

	fontA = GetFontHeight() * 1.35;
	buttonBackX = winWidth * 0.45;
	buttonBackHeight = winHeight * 0.05;
	buttonBackWidth = winWidth * 0.1;
	
	DisplayClear();			//清屏函数，自带

	if (globalHelp == 0){
		if (globalFile){
		drawPic();
		}
		drawButtons();			//绘制button
		drawMenu();				//绘制和处理菜单
		drawStatusBox();		//绘制状态栏
		if (globalActive == 0){
	
		}
		else if (globalActive == 1){
			drawPrompt1();
		}
			else if (globalActive == 2){
			drawPrompt2();
		}
		else if (globalActive == 3){
			drawInputBox();
		}
		else if (globalActive == 4){
			drawTipBox();
		}
	}//if (globalHelp == 0)
	else if(globalHelp == 1){
		printUse(buttonBackX, buttonBackHeight, buttonBackWidth, fontA);
	}
	else if(globalHelp == 2){
		printAbout(buttonBackX, buttonBackHeight, buttonBackWidth, fontA);
	}
	else{
		;
	}
	
}

void drawPic(){
	int i, j, n = -1;
	int dateNumber;			//首末日期之间天数
	double sx, sy;			//第一天+人数min时起笔点坐标，与轴有距离
	double dx, dy;			//每个点横纵坐标变化标准间隔
	double pieX,pieY,pieR;
	double barDy;
	RECORD *p;

	drawArea();

	n = getKeyNumber(kpHead);
	dateNumber = getDateNumber(rpHeadZoom, rpTailZoom);

	dx = coordinateWidth * 0.9 / dateNumber;
	dy = coordinateHeight * 0.9 / peopleDelta;
	if (globalFitIn == 0){
		peopleDelta = getTotalPeopleNumber(rpHead, rpTail, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
	else {
		peopleDelta = getTotalPeopleNumber(rpHeadZoom, rpTailZoom, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
	if (peopleDelta == 0){
			peopleDelta=10000;

	}
	sx = coordinateWidth * 0.07 + coordinateX;
	sy = coordinateHeight * 0.05 + coordinateY;
	barDy = coordinateHeight * 0.4 / peopleMax;		//柱状图最高占据y轴0.4
	pieX = winWidth * 0.93;
	pieY = winHeight * 0.2;
	pieR = winWidth * 0.05;
	highLightBoxdx = coordinateWidth / 6;

	drawYLine(sy);
	if (globalDate){
		printDateX(sx, dx);
	}

	//折线绘制
	for (i=0; i<n; i++){
		drawFoldLine(sx, sy, dx, dy, peopleMin, peopleMax, dateNumber, i); 
	}

	//柱状图
	if (dx > (0.05 * coordinateWidth)){
		p = rpHeadZoom;
		j = 0;
		if (p == rpTailZoom){
			drawBar(sx, dx, barDy, n, j, p);
		}
		else{
			while (p->next != rpTailZoom){
				drawBar(sx, dx, barDy, n, j, p);
				j++;
				p = p->next;
			}//while
			drawBar(sx, dx, barDy, n, j, p);
			j++;
			p = p->next;
			drawBar(sx, dx, barDy, n, j, p);
		}//else
	}//if柱状图

	//标注线名字
	sortLineName(n);
	adjustLineName(n);
	for (i=0; i<n; i++){
		highLight[i][0] = sx + dx * (dateNumber);
		lineName(highLight[i][0], highLight[i][1], i);
	}
	if(peopleMax != 0)
	drawPie(pieX, pieY, pieR, n);
}

void printUse(double buttonBackX, double buttonBackHeight, double buttonBackWidth, double fontA){
	double helpX, helpY;
	helpX = winWidth * 0.15;
	helpY = winHeight * 0.9;

	SetPenColor("Black");
	drawLabel((winWidth - TextStringWidth("使用说明"))/2, helpY, "使用说明");
	helpY -= fontA * 2;
	drawLabel(helpX, helpY, "本程序所有需要输入的部分请全部使用英文。注意输入文件需带后缀名。");
	helpY -= fontA;
	SetPenColor("Blue");
	drawLabel(helpX, helpY, "1）基本分区：");
	SetPenColor("Black");
	drawLabel(helpX + TextStringWidth("1）基本分区："), helpY, "初始界面分为菜单栏，浏览控制区和编辑区。空白部分为待加载的绘图区。");
	helpY -= fontA;
	SetPenColor("Blue");
	drawLabel(helpX, helpY, "2）打开文件");
	SetPenColor("Black");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    绘图需要数据支持。菜单栏文件->打开，出现弹窗。");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    单击文本框后，输入提供的数据文件名称。本程序提供多个数据文件，择一即可，例如：6.txt。");
	helpY -= fontA;
	SetPenColor("Blue");
	drawLabel(helpX, helpY, "3）绘图操作");
	SetPenColor("Black");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -若数据文件较大，初始只显示折线图，放大到一定比例后才显示柱状图。右下角饼图根据已有最后一天数据绘制。");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -右边标签是城市，依据折线终点位置按顺序对应折线。");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -移动：左/右移按钮，或在x轴上左右拖拽。");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -高亮：蓝色线为普通线，红色线为当前所选线。可以通过鼠标点击图表右侧的折线名称label切换高亮线。");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -光标：线上的小红圈指示左/右移光标所在位置。");
	helpY -= fontA;
	SetPenColor("Blue");
	drawLabel(helpX, helpY, "4）编辑模式");
	SetPenColor("Black");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -开启编辑：菜单栏->编辑->开启，或界面右边相应按钮开启");
	helpY -= fontA;
	drawLabel(helpX, helpY, "               *关闭编辑模式可以防止无意更改数据。");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -新建文件：菜单栏->文件->新建");
	helpY -= fontA;
	drawLabel(helpX, helpY, "               新建文件后新建日期，注意横坐标变化。点击“缩小”按钮可看到新建效果。");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -新增线：界面右边按钮中的“新增线”，在弹窗中输入该折线的命名。");
	helpY -= fontA;
	drawLabel(helpX, helpY, "               通过单击折线名称label选中（高亮）该线，即可进行下一步编辑。");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -删除线：界面右边按钮中的“删除线”，删除当前选中线。");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -新建日期：界面右边按钮中的“新建日期”，自动生成下一天日期。");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -删除日期：删除最后一天数据。注意，需要最右边数据在界面内才能看到变化。");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -修改链表数据：单击界面右边“修改链表数据”按钮，在弹窗中输入数值，即编辑成功。");
	helpY -= fontA;
	drawLabel(helpX, helpY, "               目前可以支持一直删除到最后一个数据。");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -保存：菜单栏->文件->保存。则更改数据会保存入数据文件。");
	helpY -= fontA;

	if (button(GenUIID(0), buttonBackX, helpY-buttonBackHeight, buttonBackWidth, buttonBackHeight, "返回主页")){
		globalHelp = 0;
	}
}

void printAbout(double buttonBackX, double buttonBackHeight, double buttonBackWidth, double fontA){
	double y;
	SetPenColor("Black");
	y = winHeight * 0.7;
	drawLabel((winWidth - TextStringWidth("关于本组")) / 2, y, "关于本组");
	y -= fontA * 3;
	drawLabel((winWidth - TextStringWidth("组名：undefined")) / 2, y, "组名：undefined");
	y -= fontA * 2;
	drawLabel((winWidth - TextStringWidth("组员：余志臻  林特思  杜宇麒")) / 2, y, "组员：余志臻  林特思  杜宇麒");
	drawLabel((winWidth - TextStringWidth("-----------------------------------------------------undefined未定义----------------------------------------------------")) / 2,
											winHeight * 0.05,
											"-----------------------------------------------------undefined未定义----------------------------------------------------");

	if (button(GenUIID(0), buttonBackX, winHeight * 0.15, buttonBackWidth, buttonBackHeight, "返回主页")){
		globalHelp = 0;
	}
}



//画柱状图
void drawBar(double sx, double dx, double barDy, int n, int j, RECORD *p){
	double width, x, h;
	int i;
	
	width = dx * 0.7 / n;
	if ((width * n / 2) > (sx - coordinateX)){
		width = (sx - coordinateX) * 2 / n * 0.9;
	}

	x = sx + (dx * j) - (width * n / 2.0);
	
	for (i=0; i<n; i++){
		if(p->number[i]==0){
			x += width;
			continue;
		}
		SetPenColor(color[i]);
		h = p->number[i] * barDy;
		drawRectangle(x, coordinateY, width, h, 1);
		x += width;
	}
}
//扇形图填色
void drawFillSector(double x, double y, double r, double start, double delta){
	StartFilledRegion(1);

	MovePen(x + r * cos(start / 180 * PI), y + r * sin(start / 180 * PI));
	DrawArc(r, start, delta);
	DrawLine(-r * cos((start + delta) * PI/ 180), -r * sin((start + delta) * PI / 180));
	DrawLine(r * cos(start / 180 * PI), r * sin(start / 180 * PI));
	
	EndFilledRegion();
}

//画扇形图
//只支持rpTail数据
void drawPie(double pieX, double pieY, double r, double n){
	double startArc, deltaArc;
	int i, total = 0;
	RECORD *p;
	p = rpTail;
	for(i=0; i<n; i++){
		total += p->number[i];
	}
	
	startArc = 0;

	for (i=0; i<n; i++){
		SetPenColor(color[i]);
		deltaArc = p->number[i] * 1.0 / total * 360;
		drawFillSector(pieX, pieY, r, startArc, deltaArc);
		startArc += deltaArc;
	}
}

//排序标号tag的位置，避免重合
//调整highLight[][3],序号越小越下面(0-6)
//n总共线数量
void sortLineName(int n)
{
	int i, j, num111;

	for (i=0; i<n; i++){
		num111 = 0;
		for (j=0; j<n; j++){
			if ((i != j) && (highLight[i][1] > highLight[j][1])){
				num111++;
			}//if
			if ((i != j) && (highLight[i][1] == highLight[j][1])){
				if (i > j){
					num111++;
				}
			}
		}//for j
		highLight[i][3] = num111;
	}//for i
}

int findSort(int n, int i){
	int j;
	for (j=0; j<n; j++){
		if (highLight[j][3] == i){
			break;
		}
	}
	return j;
}

void adjustLineName(int n){
	int i, mid, findn, middler;
	double fontA;

	fontA = GetFontAscent();
	mid = n / 2;//固定位置的线作为基准，调上下;e.g.共7条，中位线是0-n中的3，即第四条

	middler = findSort(n, mid);
	for (i=(mid + 1); i<n; i++){
		findn = findSort(n, i);
		if (highLight[findn][1] < (highLight[middler][1] + fontA)){
			highLight[findn][1] = highLight[middler][1] + fontA * 1.2;
		}
		middler = findn;
	}
	middler = findSort(n, mid);
	for (i=(mid - 1); i>=0; i--){
		findn = findSort(n, i);
		if ((highLight[findn][1] + fontA) > highLight[middler][1])
		{
			highLight[findn][1] = highLight[middler][1] - fontA * 1.2;
		}
		middler = findn;
	}
}

//图例标注哪条线
//(px, py)是当前线的label左下角坐标，dx控制box宽度，i是第几条线
void lineName(double px, double py, int i){
	int k;
	double fontA, dx;
	KEY *temp;

	dx = coordinateWidth / 6;
	temp = kpHead;
	fontA = GetFontAscent();

	//确定对应的KEY
	for (k=0; k<i; k++){
		temp = temp->next;
	}
	SetPenColor("Light Gray");
	drawBox(px, py, dx, fontA, 1, temp->name, 'c', "Black");
	SetPenColor(color[i]);
	drawRectangle(px+dx, py, dx / 4, fontA, 1);
}

//纵坐标标度横线,无数字（参照线，可有可无）
void drawYLine(double sy){
	double d10, ny;//ny即nowY
	int i;

	d10 = (coordinateY + coordinateHeight - sy) / 5.2;//魔性调参
	ny = sy;

	SetPenColor("Gray");

	for (i=0; i<=5; i++){
		ny = sy + i * d10;
		MovePen(coordinateX, ny);
		DrawLine(coordinateWidth, 0);
	}
}


//draw坐标系外的大长方形以及坐标系x/y轴
void drawArea(){
	double areaWidth, areaHeight;  //长方形area的宽、长
	double areaX, areaY;  //图像area左下角点
	double fontH = GetFontHeight();  //字

	areaX = winWidth * 0.04;
	areaY = winHeight * 0.2;
	areaWidth = winWidth * 0.8;
	areaHeight = winHeight * 0.7;

	coordinateX = areaX + areaWidth * 0.04;
	coordinateY = areaY + areaHeight * 0.15;

	coordinateHeight = areaHeight * 0.8;  //坐标系横轴占areaWidth的0.8
	coordinateWidth = areaWidth * 0.8;  //坐标系纵轴占areaHeight的0.8

	MovePen(areaX, areaY);
	DrawLine(0, areaHeight);
	DrawLine(areaWidth, 0);
	DrawLine(0, -areaHeight);
	DrawLine(-areaWidth, 0);

	//小箭头的大小直接画的时候调数字，目前是纵轴长度coordinateHeight*0.05
	//y轴
	MovePen(coordinateX, coordinateY);
	DrawLine(0, coordinateHeight);
	drawLabel(coordinateX - areaWidth * 0.02, coordinateY + coordinateHeight - fontH * 0, "n");
	drawLabel(coordinateX - areaWidth * 0.02, coordinateY + coordinateHeight - fontH * 1, "u");
	drawLabel(coordinateX - areaWidth * 0.02, coordinateY + coordinateHeight - fontH * 2, "m");
	drawLabel(coordinateX - areaWidth * 0.02, coordinateY + coordinateHeight - fontH * 3, "b");
	drawLabel(coordinateX - areaWidth * 0.02, coordinateY + coordinateHeight - fontH * 4, "e");
	drawLabel(coordinateX - areaWidth * 0.02, coordinateY + coordinateHeight - fontH * 5, "r");
	//小箭头
	MovePen(coordinateX, coordinateY + coordinateHeight);
	DrawLine(coordinateHeight * 0.05, -coordinateHeight * 0.05);
	MovePen(coordinateX, coordinateY + coordinateHeight);
	DrawLine(-coordinateHeight * 0.05, -coordinateHeight * 0.05);
	//x轴
	MovePen(coordinateX, coordinateY);
	DrawLine(coordinateWidth, 0);
	drawLabel(coordinateX + coordinateWidth, coordinateY - fontH, "date");
	//小箭头
	MovePen(coordinateX + coordinateWidth, coordinateY);
	DrawLine(-coordinateHeight * 0.05, coordinateHeight * 0.05);
	MovePen(coordinateX + coordinateWidth, coordinateY);
	DrawLine(-coordinateHeight * 0.05, -coordinateHeight * 0.05);
}

/*
*  功能：get有几条线
*  参数：p为KEY结构链表首地址
*/
int getKeyNumber(KEY *p){
	int n = 0;

	if (p->next == NULL){
		if (p->name[0] == '\0'){
			return 0;
		}
		else {
			return 1;
		}
	}

	while (p->next != NULL){
		n++;
		p = p->next;
	}
	n++;
	return n;
}


//判断是否继续放大
int judgeZoomIn(){
	int count = 0;
	RECORD *temp = NULL;

	temp = rpHeadZoom;
	if (temp==rpTailZoom){
		count = 1;
	}
	while(temp->next!=rpTailZoom){
		count++;
		temp=temp->next;
	}
	count+=2;
	if (count <= 5){//仅剩5个数据
		return 0;
	}
	else{
		return 1;
	}
}

//放大调用
void buttonZoomIn(){
	if (judgeZoomIn()){		//放大则头尾相反
		if (rp == rpHeadZoom){
			rpTailZoom = rpTailZoom->prior;
		}
		else if (rp == rpTailZoom){
			rpHeadZoom = rpHeadZoom->next;
		}
		else{
			rpHeadZoom = rpHeadZoom->next;
			rpTailZoom = rpTailZoom->prior;
		}
	}
	else{
		console("无法继续放大");
		return;
	}
	display();
}



//缩小调用
void buttonZoomOut(){
	RECORD *temp;
	if (rpHeadZoom->prior == NULL && rpTailZoom->next == NULL){//没有缩小余地，两端都到头
		console("已显示全部结点");
		return;
	}
	if (getDateNumber(rpHeadZoom,rpTailZoom) >= MAX_DATENUMBER_SHOW){
		console("已到达显示结点上限");
		return;
	}

	if (rpHeadZoom->prior != NULL && rpTailZoom->next != NULL){//两端都没到头
		rpHeadZoom = rpHeadZoom->prior;
		rpTailZoom = rpTailZoom->next;
	}
	else if (rpHeadZoom->prior == NULL){		//首到头，判断尾部
		temp = rpTailZoom->next;
		if (temp->next != NULL){				//尾端可以缩两个
			rpTailZoom = temp->next;
			return;
		}
		else {									//尾端缩一个
			rpTailZoom = temp;
			return;
		}
	}//else if
	else if (rpTailZoom->next == NULL){
		temp = rpHeadZoom->prior;
		if (temp->prior != NULL){//头部可以缩两个
			rpHeadZoom = temp->prior;
			return;
		}
		else{//头部缩一个
			rpHeadZoom = temp;
			return;
		}
	}//else if
	display();
}


//原始/缩放后头尾之间天数
int getDateNumber(RECORD *rpHeadZoom, RECORD *rpTailZoom){
	int count = 0;
	RECORD *temp;

	temp = rpHeadZoom;

	if (temp == rpTailZoom){
		return 1;
	}
	else if (temp->next == rpTailZoom){
		return 2;
	}
	while (temp->next != rpTailZoom){
		count++;
		temp = temp->next;
	}
	count++;
	count++;
	return count;
}

//得到这个文件中数据，人数的最大值、最小值，返回max和min的差值，用作y轴坐标确定
int getTotalPeopleNumber(RECORD *PH,RECORD *PT, int n, int *peopleMax, int *peopleMin){
	int i, max = 0, min = PH->number[0];
	RECORD *temp = NULL;
	temp = PH;
	if (temp == PT){
		for (i=0; i<=n-1; i++){
			if (temp->number[i] > max){
				max = temp->number[i];
			}
			else if (temp->number[i] < min){
				min = temp->number[i];
			}
		}
		*peopleMin = min;
		*peopleMax = max;
		return max - min;
	}
	while (temp->next != PT){
		for (i=0; i<=n-1; i++){
			if (temp->number[i] > max){
				max = temp->number[i];
			}
			else if (temp->number[i] < min){
				min = temp->number[i];
			}
		}
		temp = temp->next;
	}//while
	for (i=0; i<=n-1; i++){
		if (temp->number[i] > max){
			max = temp->number[i];
		}
		else if (temp->number[i] < min){
			min = temp->number[i];
		}
	}
	temp = temp->next;
	for (i=0; i<=n-1; i++){
		if (temp->number[i] > max){
			max = temp->number[i];
		}
		else if (temp->number[i] < min){
			min = temp->number[i];
		}
	}
	*peopleMin = min;
	*peopleMax = max;
	return max - min;
}


//人数标注
void peopleLabel(RECORD *temp, double dx, double labelX, double labelY, int peopleMin, int peopleMax, int i){
	char num[10];
	double r = 0.04 + dx * 0.015;//重点记号圈大小
	double fontA = GetFontAscent();//label的高度，控制y坐标用

	SetPenColor("Black");

	if (globalLabel){
		drawLabel(labelX - TextStringWidth(itoa(temp->number[i], num, 10)) / 2, labelY, itoa(temp->number[i], num, 10));
	}
	//当前选中点加画圈圈
	if (rp == temp && highLight[i][2] == 1){
		drawLabel(labelX - TextStringWidth(itoa(temp->number[i], num, 10)) / 2, labelY + 0.1 * fontA, itoa(temp->number[i], num, 10));
		MovePen(labelX + r, labelY);
		SetPenColor("Red");
		DrawArc(r, 0, 360);
	}
	//判断高亮/不高亮  笔触颜色
	judgeHighLightPen(i);
}

//mouseEvent里判断鼠标是否点击box区域，需要高亮
void judgeHighLight(double mx, double my){
	int i, n;
	double fontA;

	fontA = GetFontAscent();
	n = getKeyNumber(kpHead);

	for (i=0; i<n; i++){
		if (mx >= highLight[i][0] && mx <= (highLight[i][0] + highLightBoxdx)){
			if (my >= highLight[i][1] && my <= (highLight[i][1] + fontA)){
				updateHighLight(i, n);
				break;
			}
		}
	}
}
//判断高亮/不高亮  笔触颜色
void judgeHighLightPen(int i){
	if (highLight[i][2] == 0){
		SetPenColor("Blue");
	}
	else if (highLight[i][2] == 1){
		SetPenColor("Red");
	}
	else{//报错黄色
		SetPenColor("Yellow");
	}

}

//i新高亮线，n线总数
//更新高亮，包括清除之前高亮和添加最新高亮
void updateHighLight(int i, int n){
	int j;
	for (j=0; j<n; j++){
		if (j != i){
			highLight[j][2] = 0;
		}
		else {
			highLight[i][2] = 1;
		}
	}//for
	kp = kpHead;
	for (j=0; j<i; j++){
		kp = kp->next;
	}
}

//日期横坐标  竖版排列print函数
void printDateX(double sx, double dx){
	RECORD *temp = NULL;
	double coordinateLabelY;
	char dateChange[10][2];
	int j, k;
	double fontA;

	temp = rpHeadZoom;
	fontA = GetFontAscent();
	coordinateLabelY = coordinateY - coordinateHeight * 0.05;

	SetPenColor("Black");

	if (temp == rpTailZoom){
		k = 4;
		//转换竖排并打印
		while (temp->date[k] != '\0'){
			dateChange[k][0] = temp->date[k];
			dateChange[k][1] = '\0';
			drawLabel(sx - TextStringWidth(dateChange[k]) / 2, coordinateLabelY - (k-4) * fontA, dateChange[k]);
			k++;
		}
	}
	else if (temp->next == rpTailZoom){
		k = 4;
		while (temp->date[k] != '\0'){
			dateChange[k][0] = temp->date[k];
			dateChange[k][1] = '\0';
			drawLabel(sx - TextStringWidth(dateChange[k]) / 2, coordinateLabelY - (k-4) * fontA, dateChange[k]);
			k++;
		}
		temp = temp->next;
		k = 4;
		while (temp->date[k] != '\0'){
			dateChange[k][0] = temp->date[k];
			dateChange[k][1] = '\0';
			drawLabel(sx + dx - TextStringWidth(dateChange[k]) / 2, coordinateLabelY - (k - 4) * fontA, dateChange[k]);
			k++;
		}
	}
	else{
		j = 0;
		while (temp->next != rpTailZoom){
			k = 4;
			while (temp->date[k] != '\0'){
				dateChange[k][0] = temp->date[k];
				dateChange[k][1] = '\0';
				drawLabel(sx + dx * j - TextStringWidth(dateChange[k]) / 2, coordinateLabelY - (k - 4) * fontA, dateChange[k]);
				k++;
			}
			temp = temp->next;
			j++;
		}
		k = 4;
		while (temp->date[k] != '\0'){
			dateChange[k][0] = temp->date[k];
			dateChange[k][1] = '\0';
			drawLabel(sx + dx * j - TextStringWidth(dateChange[k]) / 2, coordinateLabelY - (k - 4) * fontA, dateChange[k]);
			k++;
		}
		temp = temp->next;
		k = 4;
		j++;
		while (temp->date[k] != '\0'){
			dateChange[k][0] = temp->date[k];
			dateChange[k][1] = '\0';
			drawLabel(sx + dx * j - TextStringWidth(dateChange[k]) / 2, coordinateLabelY - (k-4) * fontA, dateChange[k]);
			k++;
		}
	}
}


//折线+横坐标日期+人数标明
//参数rpHeadZoom起始日期对应结构地址，rpTailZoom结束日期对应结构地址,
//sx,sy起笔点坐标，与轴有距离; dx,dy每个点横纵坐标变化标准间隔
//i现在哪条线
void drawFoldLine(double sx, double sy, double dx, double dy, int peopleMin, int peopleMax, int dateNumber, int i){
	RECORD *temp = NULL;
	double nowPointY, lastPointY;//当前点基准线以上delta值，上一个点基准线以上delta值
	int j = 0, k = 0;
	double fontA;

	fontA = GetFontAscent();
	temp = rpHeadZoom;
	nowPointY = (temp->number[i] - peopleMin) * dy;//基准线以上delta

	//判断高亮/不高亮  笔触颜色
	judgeHighLightPen(i);

	lastPointY = nowPointY;
	if (temp == rpTailZoom){
		//人数标注
		peopleLabel(temp, dx, sx, sy + nowPointY, peopleMin, peopleMax, i);
	}
	else if (temp->next == rpTailZoom){
		//人数标注
		peopleLabel(temp, dx, sx, sy + nowPointY, peopleMin, peopleMax, i);

		//下一个点
		temp = temp->next;

		//补rpTailZoom
		MovePen(sx, sy + nowPointY);
		nowPointY = (temp->number[i] - peopleMin) * dy;
		DrawLine(dx, nowPointY - lastPointY);
		//人数标注
		peopleLabel(temp, dx, sx + dx, sy + nowPointY, peopleMin, peopleMax, i);
	}
	else {
		j = 1;
		//人数标注第一个点
		peopleLabel(temp, dx, sx, sy + nowPointY, peopleMin, peopleMax, i);
		temp = temp->next;
		while (temp->next != rpTailZoom){
			MovePen(sx + dx * (j - 1), sy + nowPointY);
			nowPointY = (temp->number[i] - peopleMin) * dy;
			DrawLine(dx, nowPointY - lastPointY);
			lastPointY = nowPointY;

			//人数标注
			peopleLabel(temp, dx, sx + dx * j, sy + nowPointY, peopleMin, peopleMax, i);
			//下一个点
			temp = temp->next;
			j++;
		}
		//补temp
		MovePen(sx + dx * (j - 1), sy + nowPointY);
		nowPointY = (temp->number[i] - peopleMin) * dy;
		DrawLine(dx, nowPointY - lastPointY);
		lastPointY = nowPointY;
		//人数标注
		peopleLabel(temp, dx, sx + dx * j, sy + nowPointY, peopleMin, peopleMax, i);
		//下一个点
		temp = temp->next;
		j++;

		//补rpTailZoom
		MovePen(sx + dx * (j - 1), sy + nowPointY);
		nowPointY = (temp->number[i] - peopleMin) * dy;
		DrawLine(dx, nowPointY - lastPointY);
		lastPointY = nowPointY;
		//人数标注
		peopleLabel(temp, dx, sx + dx * j, sy + nowPointY, peopleMin, peopleMax, i);
	}

	highLight[i][0] = sx + dx * (j - 0.25);
	highLight[i][1] = sy +nowPointY - fontA / 2;
}

//预测链表与原链表的连接
void connnect(RECORD *p0, RECORD *futurep){
	RECORD *p=futurep;
	p0->next = futurep;
	futurep->prior = p0;
	while(p->next!=NULL)p=p->next;
	rpTail=p;
}

//重新预测，原本拼接链表拆开
void seperate(RECORD *rp){
	int i;
	RECORD *temp = NULL, *futureTemp;
	temp = rp;
	while (temp->next != NULL)
	{//找到链表的tail
		temp = temp->next;
	}


	for (i=1; i<=futureDays; i++)  //反向遍历futureDays次，找到连接点
	{
		temp = temp->prior;
	}
	futureTemp = temp->next;
	temp->next = NULL;
	futureTemp->prior = NULL;
	free(futureTemp);
}

//左移到底，更新rpHeadZoom和rpTailZoom
void buttonLeftest(){
	int i, dateNumber;
	RECORD *temp;
	dateNumber = getDateNumber(rpHeadZoom, rpTailZoom);
	rpHeadZoom = rpHead;
	temp = rpHeadZoom;

	for (i=1; i<dateNumber; i++){
		temp = temp->next;
	}
	rpTailZoom = temp;
	rp = rpHeadZoom;
	display();
}

//右移到底，更新rpHeadZoom和rpTailZoom
void buttonRightest(){
	int i, dateNumber;
	RECORD *temp;

	dateNumber = getDateNumber(rpHeadZoom, rpTailZoom);
	rpTailZoom = rpTail;

	temp = rpTail;
	for (i=1; i<dateNumber; i++){//反向遍历dateNumber
		temp = temp->prior;
	}
	rpHeadZoom = temp;
	rp = rpTailZoom;
	display();
}

//左移一天
void buttonLeft(){	
	if (rp->prior != NULL){
		if (rp == rpHeadZoom){
			rpHeadZoom = rpHeadZoom->prior;
			rpTailZoom = rpTailZoom->prior;
		}
		rp=rp->prior;
	}else {
		console("无法继续左移");
	}
	display();
}

//右移一天
void buttonRight(){
	if (rp->next != NULL){
		if (rp == rpTailZoom){
			rpHeadZoom = rpHeadZoom->next;
			rpTailZoom = rpTailZoom->next;
		}
		rp = rp->next;
	}else {
		console("无法继续右移");
	}
	display();
}

void initHighLight(int n){
	int i;
	for (i=0; i<=n; i++){
		highLight[i][0] = -1;//x定位不可能为负数
		highLight[i][2] = 0;
	}
	highLight[0][2] = 1;
	kp = kpHead;
}

//Button排版函数

//Button排版部分
void addEditButton(int a,int b,void(*p)(),char *str){		//button位置（a,b）a列数(横坐标)，b行数（纵坐标），*p为按钮触发的函数
	static double editW;									//编辑区宽度
	static double editX = 10.5;								//编辑区button首位置x
	static double editY = 2;								//编辑区button首位置y
	static double editScaleX = 6;							//编辑区button宽度间距之比
	static double editScaleY = 6;							//编辑区button高度间距之比
	static double a0 = 4;									//编辑区一行button总个数

	double fontHeight = GetFontHeight();					//字体高度
	double h = fontHeight * 2;								//编辑区button高度
	double sh = h / editScaleY;								//编辑区button上下间隔
	static double w=0;										//编辑区button宽度
	double sw = w / editScaleX;								//编辑区button左右间隔
	editW = 0.55 * winWidth;
	w= editW / (a0 * (1 + 1 / editScaleX));

	if (globalEdit == 0){
		SetPenColor("Gray");
	}

	if (button(GenUIID(a * 10 + b), editX + (a - 1) * w + (a - 1) * sw, editY + b * h + (b - 1) * sh, w, h, str)){
		p();//此处填写函数
	}
}

void addViewButton(int a, int b, void(*p)(), char *str){     //button位置（a,b）a列数(横坐标)，b行数（纵坐标），*p为按钮触发的函数
	static double viewW;									//浏览区宽度
	static double viewX = 0.5;								//浏览区button首位置x
	static double viewY = -0.2;								//浏览区button首位置y
	static double viewScaleX = 6;							//浏览区button宽度间距之比
	static double viewScaleY = 6;							//浏览区button高度间距之比
	static double a0 = 4;									//浏览区一行button总个数

	double fontHeight = GetFontHeight();					//字体高度
	double h = fontHeight * 2;								//浏览区button高度
	double sh = h / viewScaleY;								//浏览区button上下间隔
	static double w = 0;										//浏览区button宽度
	double sw = w / viewScaleX;								//浏览区button左右间隔
	viewW = 0.8 * winWidth;
	w = viewW / (a0 * (1 + 1/viewScaleX));
	if (button(GenUIID(a * 100 + b * 10), viewX + (a - 1) * w + (a - 1) * sw, viewY + b * h + (b - 1) * sh, w, h, str)){
		p();//此处填写函数
	}
}

void drawMenu(){   
	static char * menuListFile[] = {"文件", 
		"新建  | Ctrl-N", // 快捷键必须采用[Ctrl-X]格式，放在字符串的结尾
		"打开  | Ctrl-O", 
		"保存  | Ctrl-S",
		"Exit   | Ctrl-E"};
	static char * menuListEdit[] = {"编辑",
		"开启",
		"关闭 | Ctrl-T"};
	static char * menuListHelp[] = {"帮助",
		"说明 | Ctrl-M",
		"关于"};
	static char * selectedLabel = NULL;
	double fH = GetFontHeight();
	double x = 0;	//fH/8;
	double y = winHeight;
	double h = fH * 1.5;	// 控件高度
	double w = TextStringWidth(menuListHelp[0]) * 2; // 控件宽度
	double wlist = TextStringWidth(menuListEdit[2]) * 1.2;
	double xindent = winHeight / 20; // 缩进
	int selection;
	// menu bar
	drawMenuBar(0, y - h, winWidth, h);
	// File 菜单
	selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0]));
	if ( selection == 1 ){
		strcpy(popInputTip,"请输入文件名");
		popInputStatus = 2;
		globalStatus = 3;
		globalActive = 3;
	}
	if ( selection == 2 ){
		strcpy(popInputTip,"请输入文件名");
		popInputStatus = 3;
		globalStatus = 4;
		globalActive = 3;
	}
	if( selection == 3 ){
		fileSave();
	}
	if( selection == 4 ){
		exit(-1);	// choose to exit
	} 
	
	// Edit 菜单
	selection = menuList(GenUIID(0), x + w,  y - h, w, wlist, h, menuListEdit, sizeof(menuListEdit) / sizeof(menuListEdit[0]));
	if(selection == 1){
		globalEdit = 1;
	}
	if(selection == 2){
		globalEdit = 0;
	}

	// Help 菜单
	selection = menuList(GenUIID(0), x + 2 * w, y - h, w, wlist, h, menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
	if(selection == 1){
		globalHelp = 1;
	}
	if(selection == 2){
		globalHelp = 2;
	}
}

void drawButtons(){	
	addViewButton(1, 1, globalFile ? buttonZoomIn : visualTip, "放大");
	addViewButton(2, 1, globalFile ? buttonZoomOut : visualTip, "缩小");
	addViewButton(3, 1, globalFile ? buttonLabel : visualTip, globalLabel ? "数字标注：ON" : "数字标注：OFF");
	addViewButton(4, 1, globalFile ? buttonDate : visualTip, globalDate ? "日期标注：ON" : "日期标注：OFF");
	addViewButton(1, 2, globalFile ? buttonLeftest : visualTip, "左移到头");
	addViewButton(2, 2, globalFile ? buttonLeft : visualTip, "左移");
	addViewButton(3, 2, globalFile ? buttonRight : visualTip, "右移");
	addViewButton(4, 2, globalFile ? buttonRightest : visualTip, "右移到头");
	addViewButton(1, 3, globalFile ? buttonFitIn : visualTip, globalFitIn ? "适应数据":"适应区域");
	addEditButton(1, 8, editOnOff, globalEdit ? "编辑模式:ON" : "编辑模式:OFF");
	addEditButton(1, 7, globalFile ? editNewDate : editTip, "新建日期");
	addEditButton(1, 6, globalFile ? editDeleteLastDate : editTip, "删除最后日期");
	addEditButton(1, 5, globalFile ? buttonChange : editTip, "修改链表数据");
	addEditButton(1, 4, globalFile ? buttonNewLine : editTip, "新增线");
	addEditButton(1, 3, globalFile ? editDeleteLine : editTip, "删除线");
	addEditButton(1, 2, globalFile ? buttonPredict : editTip, "预测");
	addEditButton(1, 1, globalFile ? buttonPredictChange : editTip, "修改预测参数");
}

void drawPrompt1(){
	double x = winWidth / 2;
	double y = winHeight / 2;
	double fH = GetFontHeight();
	double h = fH * 1.5; // 控件高度
	SetPenColor("White");
	drawRectangle(x - 1.5, y - 1, 4, 2, 1);
	SetPenColor("Black");
	drawRectangle(x - 1.5, y - 1, 4, 2, 0);
	drawLabel(x - 2 + 2 * fH + 1.5, y - 2 + fH + 2.5, "尚未保存，确认退出？");
	if (button(GenUIID(0), x - 2 + 2 * fH + 1, y - 2 +fH + 1, 1, h, "确定")){
		exit(-1);
	}
	if (button(GenUIID(1), x - 2 + 2 * fH + 2.5, y - 2 + fH + 1, 1, h, "取消")){
		globalActive = 0;
	}
}

void drawPrompt2(){
	double x = winWidth / 2;
	double y = winHeight / 2;
	double fH = GetFontHeight();
	double h = fH * 1.5; // 控件高度
	SetPenColor("White");
	drawRectangle(x - 1.5, y - 1, 4, 2, 1);
	SetPenColor("Black");
	drawRectangle(x - 1.5, y - 1, 4, 2, 0);
	drawLabel(x - 1, y - 2 + fH + 2.5, "温馨提示：本程序无撤销功能，");
	drawLabel(x - 0.5, y + 0.5, "是否要进行当前操作？");
	if (button(GenUIID(0), x - 2 + 2 * fH + 1, y - 2 + fH + 1, 1, h, "是")){
		if (popStatus2 = 0);
		else if (popStatus2 = 1);
	}
	if (button(GenUIID(1), x-2+2*fH+2.5, y-2+fH+1, 1, h, "否")){
		globalActive = 0;
	}
}

void drawInputBox(){
	double x = winWidth / 2;
	double y = winHeight / 2;
	double fH = GetFontHeight();
	double h = fH * 1.5; // 控件高度
	SetPenColor("White");
	drawRectangle(x - 1.5, y - 1, 4, 2, 1);
	SetPenColor("Black");
	drawRectangle(x - 1.5, y - 1, 4, 2, 0);
	drawLabel(x - strlen(popInputTip) / 16, y - 2 + fH + 2.5, popInputTip);
	if (button(GenUIID(0), x - 2 + 2 * fH + 1, y - 2 + fH + 1, 1, h, "确定")){
		if (popInputStatus == 0){//修改参数
			editChange();
			popInputStatus = -1;//初始化inputBox
		}
		else if (popInputStatus == 1){//新增线
			editNewLine();
			popInputStatus = -1;//初始化inputBox
		}
		else if (popInputStatus == 2){//新建文件
			fileNew();
			popInputStatus = -1;//初始化inputBox
		}
		else if (popInputStatus == 3){//打开文件
			fileOpen();
			popInputStatus = -1;//初始化inputBox
		}
		else if (popInputStatus == 4){//修改预测参数
			predictChange();
			popInputStatus = -1;//初始化inputBox
		}
	}
	if (button(GenUIID(1), x - 2 + 2 * fH + 2.5, y - 2 + fH + 1, 1, h, "取消")){
		globalActive = 0;
		globalStatus = 0;	
	}
	if( textbox(GenUIID(0), x - 0.5, y, 2, h, popInput, sizeof(popInput)));
}

void drawTipBox(){
	double x = winWidth / 2;
	double y = winHeight / 2;
	double fH = GetFontHeight();
	double h = fH * 1.5; // 控件高度
	SetPenColor("White");
	drawRectangle(x - 1.5, y - 1, 4, 2, 1);
	SetPenColor("Black");
	drawRectangle(x - 1.5,y - 1,4,2,0);
	drawLabel(x - strlen(popTip) / 8 * fH, y - 2 + fH + 2.5, popTip);
	if (button(GenUIID(0), x + 2 * fH - 0.5, y - 2 + fH + 1, 1, h, "确定")) {
		globalActive = 0;
		globalStatus = 0;
	}
}
//状态栏函数      利用全局变量控制显示什么状态
void drawStatusBox(){
	double fH = GetFontHeight();
	if (globalStatus == 0 && globalFile == 0) drawLabel(11, fH, "状态信息");
	if (globalStatus == 1) drawLabel(10.8, fH, "修改链表参数");
	if (globalStatus == 2) drawLabel(11, fH, "新增线");
	if (globalStatus == 3) drawLabel(11, fH, "新建文件");
	if (globalStatus == 4) drawLabel(11, fH, "打开文件");
	if (globalStatus == 5) drawLabel(11, fH, "保存文件");
	if (globalStatus == 6) drawLabel(11, fH, "修改预测参数");
	if (globalStatus == 0 && globalFile) drawLabel(11, fH, globalFitIn ? "适应区域" : "适应数据");
}
