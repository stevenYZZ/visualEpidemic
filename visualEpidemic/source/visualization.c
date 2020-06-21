#include "common.h"
#include "visualization.h"
#include "edit.h"
#include "files.h"
#include "predict.h"

//ȫ�ֱ���
double winWidth, winHeight;   // ���ڳߴ�
double coordinateX, coordinateY;  //����ϵ�����½ǵ�
double coordinateWidth, coordinateHeight;  //����ϵ��x/y�᳤�ȣ���drawArea();��ȷ��

double highLight[10][4];  //�Ƿ������[][0]x���꣬[][1]y���꣬[][2]�Ƿ������[][4]λ����ţ�ԽСԽ���棩
double highLightBoxdx; //�жϸ���Ҫ�õ�box�Ŀ�
int peopleMin = -1, peopleMax = -1, peopleDelta = -1;

RECORD *rpHead = NULL,*rpTail = NULL;
KEY *kpHead = NULL, *kpTail = NULL;

RECORD *rp = NULL, *rpHeadZoom = NULL, *rpTailZoom = NULL;
KEY *kp = NULL;
//�ֲ�����
static char color[10][10]={"Brown", "Pinkish", "Cream", "LightYell", "ThickYell", "Orange","SeaBlue","DarkBlue","CyanGray","LightPur"};
//�����Ӱ˺���
void visualTip(){
	globalActive = 4;
	strcpy(popTip, "��ǰδ�򿪻��½��ļ�");
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


//����ˢ��
//��������ֿ��ӻ�����
void display(){

	double buttonBackX, buttonBackHeight, buttonBackWidth;
	double fontA;

	fontA = GetFontHeight() * 1.35;
	buttonBackX = winWidth * 0.45;
	buttonBackHeight = winHeight * 0.05;
	buttonBackWidth = winWidth * 0.1;
	
	DisplayClear();			//�����������Դ�

	if (globalHelp == 0){
		if (globalFile){
		drawPic();
		}
		drawButtons();			//����button
		drawMenu();				//���ƺʹ���˵�
		drawStatusBox();		//����״̬��
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
	int dateNumber;			//��ĩ����֮������
	double sx, sy;			//��һ��+����minʱ��ʵ����꣬�����о���
	double dx, dy;			//ÿ�����������仯��׼���
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
	barDy = coordinateHeight * 0.4 / peopleMax;		//��״ͼ���ռ��y��0.4
	pieX = winWidth * 0.93;
	pieY = winHeight * 0.2;
	pieR = winWidth * 0.05;
	highLightBoxdx = coordinateWidth / 6;

	drawYLine(sy);
	if (globalDate){
		printDateX(sx, dx);
	}

	//���߻���
	for (i=0; i<n; i++){
		drawFoldLine(sx, sy, dx, dy, peopleMin, peopleMax, dateNumber, i); 
	}

	//��״ͼ
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
	}//if��״ͼ

	//��ע������
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
	drawLabel((winWidth - TextStringWidth("ʹ��˵��"))/2, helpY, "ʹ��˵��");
	helpY -= fontA * 2;
	drawLabel(helpX, helpY, "������������Ҫ����Ĳ�����ȫ��ʹ��Ӣ�ġ�ע�������ļ������׺����");
	helpY -= fontA;
	SetPenColor("Blue");
	drawLabel(helpX, helpY, "1������������");
	SetPenColor("Black");
	drawLabel(helpX + TextStringWidth("1������������"), helpY, "��ʼ�����Ϊ�˵���������������ͱ༭�����հײ���Ϊ�����صĻ�ͼ����");
	helpY -= fontA;
	SetPenColor("Blue");
	drawLabel(helpX, helpY, "2�����ļ�");
	SetPenColor("Black");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    ��ͼ��Ҫ����֧�֡��˵����ļ�->�򿪣����ֵ�����");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    �����ı���������ṩ�������ļ����ơ��������ṩ��������ļ�����һ���ɣ����磺6.txt��");
	helpY -= fontA;
	SetPenColor("Blue");
	drawLabel(helpX, helpY, "3����ͼ����");
	SetPenColor("Black");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -�������ļ��ϴ󣬳�ʼֻ��ʾ����ͼ���Ŵ�һ�����������ʾ��״ͼ�����½Ǳ�ͼ�����������һ�����ݻ��ơ�");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -�ұ߱�ǩ�ǳ��У����������յ�λ�ð�˳���Ӧ���ߡ�");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -�ƶ�����/���ư�ť������x����������ק��");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -��������ɫ��Ϊ��ͨ�ߣ���ɫ��Ϊ��ǰ��ѡ�ߡ�����ͨ�������ͼ���Ҳ����������label�л������ߡ�");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -��꣺���ϵ�С��Ȧָʾ��/���ƹ������λ�á�");
	helpY -= fontA;
	SetPenColor("Blue");
	drawLabel(helpX, helpY, "4���༭ģʽ");
	SetPenColor("Black");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -�����༭���˵���->�༭->������������ұ���Ӧ��ť����");
	helpY -= fontA;
	drawLabel(helpX, helpY, "               *�رձ༭ģʽ���Է�ֹ����������ݡ�");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -�½��ļ����˵���->�ļ�->�½�");
	helpY -= fontA;
	drawLabel(helpX, helpY, "               �½��ļ����½����ڣ�ע�������仯���������С����ť�ɿ����½�Ч����");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -�����ߣ������ұ߰�ť�еġ������ߡ����ڵ�������������ߵ�������");
	helpY -= fontA;
	drawLabel(helpX, helpY, "               ͨ��������������labelѡ�У����������ߣ����ɽ�����һ���༭��");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -ɾ���ߣ������ұ߰�ť�еġ�ɾ���ߡ���ɾ����ǰѡ���ߡ�");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -�½����ڣ������ұ߰�ť�еġ��½����ڡ����Զ�������һ�����ڡ�");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -ɾ�����ڣ�ɾ�����һ�����ݡ�ע�⣬��Ҫ���ұ������ڽ����ڲ��ܿ����仯��");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -�޸��������ݣ����������ұߡ��޸��������ݡ���ť���ڵ�����������ֵ�����༭�ɹ���");
	helpY -= fontA;
	drawLabel(helpX, helpY, "               Ŀǰ����֧��һֱɾ�������һ�����ݡ�");
	helpY -= fontA;
	drawLabel(helpX, helpY, "    -���棺�˵���->�ļ�->���档��������ݻᱣ���������ļ���");
	helpY -= fontA;

	if (button(GenUIID(0), buttonBackX, helpY-buttonBackHeight, buttonBackWidth, buttonBackHeight, "������ҳ")){
		globalHelp = 0;
	}
}

void printAbout(double buttonBackX, double buttonBackHeight, double buttonBackWidth, double fontA){
	double y;
	SetPenColor("Black");
	y = winHeight * 0.7;
	drawLabel((winWidth - TextStringWidth("���ڱ���")) / 2, y, "���ڱ���");
	y -= fontA * 3;
	drawLabel((winWidth - TextStringWidth("������undefined")) / 2, y, "������undefined");
	y -= fontA * 2;
	drawLabel((winWidth - TextStringWidth("��Ա����־��  ����˼  ������")) / 2, y, "��Ա����־��  ����˼  ������");
	drawLabel((winWidth - TextStringWidth("-----------------------------------------------------undefinedδ����----------------------------------------------------")) / 2,
											winHeight * 0.05,
											"-----------------------------------------------------undefinedδ����----------------------------------------------------");

	if (button(GenUIID(0), buttonBackX, winHeight * 0.15, buttonBackWidth, buttonBackHeight, "������ҳ")){
		globalHelp = 0;
	}
}



//����״ͼ
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
//����ͼ��ɫ
void drawFillSector(double x, double y, double r, double start, double delta){
	StartFilledRegion(1);

	MovePen(x + r * cos(start / 180 * PI), y + r * sin(start / 180 * PI));
	DrawArc(r, start, delta);
	DrawLine(-r * cos((start + delta) * PI/ 180), -r * sin((start + delta) * PI / 180));
	DrawLine(r * cos(start / 180 * PI), r * sin(start / 180 * PI));
	
	EndFilledRegion();
}

//������ͼ
//ֻ֧��rpTail����
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

//������tag��λ�ã������غ�
//����highLight[][3],���ԽСԽ����(0-6)
//n�ܹ�������
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
	mid = n / 2;//�̶�λ�õ�����Ϊ��׼��������;e.g.��7������λ����0-n�е�3����������

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

//ͼ����ע������
//(px, py)�ǵ�ǰ�ߵ�label���½����꣬dx����box��ȣ�i�ǵڼ�����
void lineName(double px, double py, int i){
	int k;
	double fontA, dx;
	KEY *temp;

	dx = coordinateWidth / 6;
	temp = kpHead;
	fontA = GetFontAscent();

	//ȷ����Ӧ��KEY
	for (k=0; k<i; k++){
		temp = temp->next;
	}
	SetPenColor("Light Gray");
	drawBox(px, py, dx, fontA, 1, temp->name, 'c', "Black");
	SetPenColor(color[i]);
	drawRectangle(px+dx, py, dx / 4, fontA, 1);
}

//�������Ⱥ���,�����֣������ߣ����п��ޣ�
void drawYLine(double sy){
	double d10, ny;//ny��nowY
	int i;

	d10 = (coordinateY + coordinateHeight - sy) / 5.2;//ħ�Ե���
	ny = sy;

	SetPenColor("Gray");

	for (i=0; i<=5; i++){
		ny = sy + i * d10;
		MovePen(coordinateX, ny);
		DrawLine(coordinateWidth, 0);
	}
}


//draw����ϵ��Ĵ󳤷����Լ�����ϵx/y��
void drawArea(){
	double areaWidth, areaHeight;  //������area�Ŀ���
	double areaX, areaY;  //ͼ��area���½ǵ�
	double fontH = GetFontHeight();  //��

	areaX = winWidth * 0.04;
	areaY = winHeight * 0.2;
	areaWidth = winWidth * 0.8;
	areaHeight = winHeight * 0.7;

	coordinateX = areaX + areaWidth * 0.04;
	coordinateY = areaY + areaHeight * 0.15;

	coordinateHeight = areaHeight * 0.8;  //����ϵ����ռareaWidth��0.8
	coordinateWidth = areaWidth * 0.8;  //����ϵ����ռareaHeight��0.8

	MovePen(areaX, areaY);
	DrawLine(0, areaHeight);
	DrawLine(areaWidth, 0);
	DrawLine(0, -areaHeight);
	DrawLine(-areaWidth, 0);

	//С��ͷ�Ĵ�Сֱ�ӻ���ʱ������֣�Ŀǰ�����᳤��coordinateHeight*0.05
	//y��
	MovePen(coordinateX, coordinateY);
	DrawLine(0, coordinateHeight);
	drawLabel(coordinateX - areaWidth * 0.02, coordinateY + coordinateHeight - fontH * 0, "n");
	drawLabel(coordinateX - areaWidth * 0.02, coordinateY + coordinateHeight - fontH * 1, "u");
	drawLabel(coordinateX - areaWidth * 0.02, coordinateY + coordinateHeight - fontH * 2, "m");
	drawLabel(coordinateX - areaWidth * 0.02, coordinateY + coordinateHeight - fontH * 3, "b");
	drawLabel(coordinateX - areaWidth * 0.02, coordinateY + coordinateHeight - fontH * 4, "e");
	drawLabel(coordinateX - areaWidth * 0.02, coordinateY + coordinateHeight - fontH * 5, "r");
	//С��ͷ
	MovePen(coordinateX, coordinateY + coordinateHeight);
	DrawLine(coordinateHeight * 0.05, -coordinateHeight * 0.05);
	MovePen(coordinateX, coordinateY + coordinateHeight);
	DrawLine(-coordinateHeight * 0.05, -coordinateHeight * 0.05);
	//x��
	MovePen(coordinateX, coordinateY);
	DrawLine(coordinateWidth, 0);
	drawLabel(coordinateX + coordinateWidth, coordinateY - fontH, "date");
	//С��ͷ
	MovePen(coordinateX + coordinateWidth, coordinateY);
	DrawLine(-coordinateHeight * 0.05, coordinateHeight * 0.05);
	MovePen(coordinateX + coordinateWidth, coordinateY);
	DrawLine(-coordinateHeight * 0.05, -coordinateHeight * 0.05);
}

/*
*  ���ܣ�get�м�����
*  ������pΪKEY�ṹ�����׵�ַ
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


//�ж��Ƿ�����Ŵ�
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
	if (count <= 5){//��ʣ5������
		return 0;
	}
	else{
		return 1;
	}
}

//�Ŵ����
void buttonZoomIn(){
	if (judgeZoomIn()){		//�Ŵ���ͷβ�෴
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
		console("�޷������Ŵ�");
		return;
	}
	display();
}



//��С����
void buttonZoomOut(){
	RECORD *temp;
	if (rpHeadZoom->prior == NULL && rpTailZoom->next == NULL){//û����С��أ����˶���ͷ
		console("����ʾȫ�����");
		return;
	}
	if (getDateNumber(rpHeadZoom,rpTailZoom) >= MAX_DATENUMBER_SHOW){
		console("�ѵ�����ʾ�������");
		return;
	}

	if (rpHeadZoom->prior != NULL && rpTailZoom->next != NULL){//���˶�û��ͷ
		rpHeadZoom = rpHeadZoom->prior;
		rpTailZoom = rpTailZoom->next;
	}
	else if (rpHeadZoom->prior == NULL){		//�׵�ͷ���ж�β��
		temp = rpTailZoom->next;
		if (temp->next != NULL){				//β�˿���������
			rpTailZoom = temp->next;
			return;
		}
		else {									//β����һ��
			rpTailZoom = temp;
			return;
		}
	}//else if
	else if (rpTailZoom->next == NULL){
		temp = rpHeadZoom->prior;
		if (temp->prior != NULL){//ͷ������������
			rpHeadZoom = temp->prior;
			return;
		}
		else{//ͷ����һ��
			rpHeadZoom = temp;
			return;
		}
	}//else if
	display();
}


//ԭʼ/���ź�ͷβ֮������
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

//�õ�����ļ������ݣ����������ֵ����Сֵ������max��min�Ĳ�ֵ������y������ȷ��
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


//������ע
void peopleLabel(RECORD *temp, double dx, double labelX, double labelY, int peopleMin, int peopleMax, int i){
	char num[10];
	double r = 0.04 + dx * 0.015;//�ص�Ǻ�Ȧ��С
	double fontA = GetFontAscent();//label�ĸ߶ȣ�����y������

	SetPenColor("Black");

	if (globalLabel){
		drawLabel(labelX - TextStringWidth(itoa(temp->number[i], num, 10)) / 2, labelY, itoa(temp->number[i], num, 10));
	}
	//��ǰѡ�е�ӻ�ȦȦ
	if (rp == temp && highLight[i][2] == 1){
		drawLabel(labelX - TextStringWidth(itoa(temp->number[i], num, 10)) / 2, labelY + 0.1 * fontA, itoa(temp->number[i], num, 10));
		MovePen(labelX + r, labelY);
		SetPenColor("Red");
		DrawArc(r, 0, 360);
	}
	//�жϸ���/������  �ʴ���ɫ
	judgeHighLightPen(i);
}

//mouseEvent���ж�����Ƿ���box������Ҫ����
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
//�жϸ���/������  �ʴ���ɫ
void judgeHighLightPen(int i){
	if (highLight[i][2] == 0){
		SetPenColor("Blue");
	}
	else if (highLight[i][2] == 1){
		SetPenColor("Red");
	}
	else{//�����ɫ
		SetPenColor("Yellow");
	}

}

//i�¸����ߣ�n������
//���¸������������֮ǰ������������¸���
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

//���ں�����  ��������print����
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
		//ת�����Ų���ӡ
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


//����+����������+��������
//����rpHeadZoom��ʼ���ڶ�Ӧ�ṹ��ַ��rpTailZoom�������ڶ�Ӧ�ṹ��ַ,
//sx,sy��ʵ����꣬�����о���; dx,dyÿ�����������仯��׼���
//i����������
void drawFoldLine(double sx, double sy, double dx, double dy, int peopleMin, int peopleMax, int dateNumber, int i){
	RECORD *temp = NULL;
	double nowPointY, lastPointY;//��ǰ���׼������deltaֵ����һ�����׼������deltaֵ
	int j = 0, k = 0;
	double fontA;

	fontA = GetFontAscent();
	temp = rpHeadZoom;
	nowPointY = (temp->number[i] - peopleMin) * dy;//��׼������delta

	//�жϸ���/������  �ʴ���ɫ
	judgeHighLightPen(i);

	lastPointY = nowPointY;
	if (temp == rpTailZoom){
		//������ע
		peopleLabel(temp, dx, sx, sy + nowPointY, peopleMin, peopleMax, i);
	}
	else if (temp->next == rpTailZoom){
		//������ע
		peopleLabel(temp, dx, sx, sy + nowPointY, peopleMin, peopleMax, i);

		//��һ����
		temp = temp->next;

		//��rpTailZoom
		MovePen(sx, sy + nowPointY);
		nowPointY = (temp->number[i] - peopleMin) * dy;
		DrawLine(dx, nowPointY - lastPointY);
		//������ע
		peopleLabel(temp, dx, sx + dx, sy + nowPointY, peopleMin, peopleMax, i);
	}
	else {
		j = 1;
		//������ע��һ����
		peopleLabel(temp, dx, sx, sy + nowPointY, peopleMin, peopleMax, i);
		temp = temp->next;
		while (temp->next != rpTailZoom){
			MovePen(sx + dx * (j - 1), sy + nowPointY);
			nowPointY = (temp->number[i] - peopleMin) * dy;
			DrawLine(dx, nowPointY - lastPointY);
			lastPointY = nowPointY;

			//������ע
			peopleLabel(temp, dx, sx + dx * j, sy + nowPointY, peopleMin, peopleMax, i);
			//��һ����
			temp = temp->next;
			j++;
		}
		//��temp
		MovePen(sx + dx * (j - 1), sy + nowPointY);
		nowPointY = (temp->number[i] - peopleMin) * dy;
		DrawLine(dx, nowPointY - lastPointY);
		lastPointY = nowPointY;
		//������ע
		peopleLabel(temp, dx, sx + dx * j, sy + nowPointY, peopleMin, peopleMax, i);
		//��һ����
		temp = temp->next;
		j++;

		//��rpTailZoom
		MovePen(sx + dx * (j - 1), sy + nowPointY);
		nowPointY = (temp->number[i] - peopleMin) * dy;
		DrawLine(dx, nowPointY - lastPointY);
		lastPointY = nowPointY;
		//������ע
		peopleLabel(temp, dx, sx + dx * j, sy + nowPointY, peopleMin, peopleMax, i);
	}

	highLight[i][0] = sx + dx * (j - 0.25);
	highLight[i][1] = sy +nowPointY - fontA / 2;
}

//Ԥ��������ԭ���������
void connnect(RECORD *p0, RECORD *futurep){
	RECORD *p=futurep;
	p0->next = futurep;
	futurep->prior = p0;
	while(p->next!=NULL)p=p->next;
	rpTail=p;
}

//����Ԥ�⣬ԭ��ƴ�������
void seperate(RECORD *rp){
	int i;
	RECORD *temp = NULL, *futureTemp;
	temp = rp;
	while (temp->next != NULL)
	{//�ҵ������tail
		temp = temp->next;
	}


	for (i=1; i<=futureDays; i++)  //�������futureDays�Σ��ҵ����ӵ�
	{
		temp = temp->prior;
	}
	futureTemp = temp->next;
	temp->next = NULL;
	futureTemp->prior = NULL;
	free(futureTemp);
}

//���Ƶ��ף�����rpHeadZoom��rpTailZoom
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

//���Ƶ��ף�����rpHeadZoom��rpTailZoom
void buttonRightest(){
	int i, dateNumber;
	RECORD *temp;

	dateNumber = getDateNumber(rpHeadZoom, rpTailZoom);
	rpTailZoom = rpTail;

	temp = rpTail;
	for (i=1; i<dateNumber; i++){//�������dateNumber
		temp = temp->prior;
	}
	rpHeadZoom = temp;
	rp = rpTailZoom;
	display();
}

//����һ��
void buttonLeft(){	
	if (rp->prior != NULL){
		if (rp == rpHeadZoom){
			rpHeadZoom = rpHeadZoom->prior;
			rpTailZoom = rpTailZoom->prior;
		}
		rp=rp->prior;
	}else {
		console("�޷���������");
	}
	display();
}

//����һ��
void buttonRight(){
	if (rp->next != NULL){
		if (rp == rpTailZoom){
			rpHeadZoom = rpHeadZoom->next;
			rpTailZoom = rpTailZoom->next;
		}
		rp = rp->next;
	}else {
		console("�޷���������");
	}
	display();
}

void initHighLight(int n){
	int i;
	for (i=0; i<=n; i++){
		highLight[i][0] = -1;//x��λ������Ϊ����
		highLight[i][2] = 0;
	}
	highLight[0][2] = 1;
	kp = kpHead;
}

//Button�Ű溯��

//Button�Ű沿��
void addEditButton(int a,int b,void(*p)(),char *str){		//buttonλ�ã�a,b��a����(������)��b�����������꣩��*pΪ��ť�����ĺ���
	static double editW;									//�༭�����
	static double editX = 10.5;								//�༭��button��λ��x
	static double editY = 2;								//�༭��button��λ��y
	static double editScaleX = 6;							//�༭��button��ȼ��֮��
	static double editScaleY = 6;							//�༭��button�߶ȼ��֮��
	static double a0 = 4;									//�༭��һ��button�ܸ���

	double fontHeight = GetFontHeight();					//����߶�
	double h = fontHeight * 2;								//�༭��button�߶�
	double sh = h / editScaleY;								//�༭��button���¼��
	static double w=0;										//�༭��button���
	double sw = w / editScaleX;								//�༭��button���Ҽ��
	editW = 0.55 * winWidth;
	w= editW / (a0 * (1 + 1 / editScaleX));

	if (globalEdit == 0){
		SetPenColor("Gray");
	}

	if (button(GenUIID(a * 10 + b), editX + (a - 1) * w + (a - 1) * sw, editY + b * h + (b - 1) * sh, w, h, str)){
		p();//�˴���д����
	}
}

void addViewButton(int a, int b, void(*p)(), char *str){     //buttonλ�ã�a,b��a����(������)��b�����������꣩��*pΪ��ť�����ĺ���
	static double viewW;									//��������
	static double viewX = 0.5;								//�����button��λ��x
	static double viewY = -0.2;								//�����button��λ��y
	static double viewScaleX = 6;							//�����button��ȼ��֮��
	static double viewScaleY = 6;							//�����button�߶ȼ��֮��
	static double a0 = 4;									//�����һ��button�ܸ���

	double fontHeight = GetFontHeight();					//����߶�
	double h = fontHeight * 2;								//�����button�߶�
	double sh = h / viewScaleY;								//�����button���¼��
	static double w = 0;										//�����button���
	double sw = w / viewScaleX;								//�����button���Ҽ��
	viewW = 0.8 * winWidth;
	w = viewW / (a0 * (1 + 1/viewScaleX));
	if (button(GenUIID(a * 100 + b * 10), viewX + (a - 1) * w + (a - 1) * sw, viewY + b * h + (b - 1) * sh, w, h, str)){
		p();//�˴���д����
	}
}

void drawMenu(){   
	static char * menuListFile[] = {"�ļ�", 
		"�½�  | Ctrl-N", // ��ݼ��������[Ctrl-X]��ʽ�������ַ����Ľ�β
		"��  | Ctrl-O", 
		"����  | Ctrl-S",
		"Exit   | Ctrl-E"};
	static char * menuListEdit[] = {"�༭",
		"����",
		"�ر� | Ctrl-T"};
	static char * menuListHelp[] = {"����",
		"˵�� | Ctrl-M",
		"����"};
	static char * selectedLabel = NULL;
	double fH = GetFontHeight();
	double x = 0;	//fH/8;
	double y = winHeight;
	double h = fH * 1.5;	// �ؼ��߶�
	double w = TextStringWidth(menuListHelp[0]) * 2; // �ؼ����
	double wlist = TextStringWidth(menuListEdit[2]) * 1.2;
	double xindent = winHeight / 20; // ����
	int selection;
	// menu bar
	drawMenuBar(0, y - h, winWidth, h);
	// File �˵�
	selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0]));
	if ( selection == 1 ){
		strcpy(popInputTip,"�������ļ���");
		popInputStatus = 2;
		globalStatus = 3;
		globalActive = 3;
	}
	if ( selection == 2 ){
		strcpy(popInputTip,"�������ļ���");
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
	
	// Edit �˵�
	selection = menuList(GenUIID(0), x + w,  y - h, w, wlist, h, menuListEdit, sizeof(menuListEdit) / sizeof(menuListEdit[0]));
	if(selection == 1){
		globalEdit = 1;
	}
	if(selection == 2){
		globalEdit = 0;
	}

	// Help �˵�
	selection = menuList(GenUIID(0), x + 2 * w, y - h, w, wlist, h, menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
	if(selection == 1){
		globalHelp = 1;
	}
	if(selection == 2){
		globalHelp = 2;
	}
}

void drawButtons(){	
	addViewButton(1, 1, globalFile ? buttonZoomIn : visualTip, "�Ŵ�");
	addViewButton(2, 1, globalFile ? buttonZoomOut : visualTip, "��С");
	addViewButton(3, 1, globalFile ? buttonLabel : visualTip, globalLabel ? "���ֱ�ע��ON" : "���ֱ�ע��OFF");
	addViewButton(4, 1, globalFile ? buttonDate : visualTip, globalDate ? "���ڱ�ע��ON" : "���ڱ�ע��OFF");
	addViewButton(1, 2, globalFile ? buttonLeftest : visualTip, "���Ƶ�ͷ");
	addViewButton(2, 2, globalFile ? buttonLeft : visualTip, "����");
	addViewButton(3, 2, globalFile ? buttonRight : visualTip, "����");
	addViewButton(4, 2, globalFile ? buttonRightest : visualTip, "���Ƶ�ͷ");
	addViewButton(1, 3, globalFile ? buttonFitIn : visualTip, globalFitIn ? "��Ӧ����":"��Ӧ����");
	addEditButton(1, 8, editOnOff, globalEdit ? "�༭ģʽ:ON" : "�༭ģʽ:OFF");
	addEditButton(1, 7, globalFile ? editNewDate : editTip, "�½�����");
	addEditButton(1, 6, globalFile ? editDeleteLastDate : editTip, "ɾ���������");
	addEditButton(1, 5, globalFile ? buttonChange : editTip, "�޸���������");
	addEditButton(1, 4, globalFile ? buttonNewLine : editTip, "������");
	addEditButton(1, 3, globalFile ? editDeleteLine : editTip, "ɾ����");
	addEditButton(1, 2, globalFile ? buttonPredict : editTip, "Ԥ��");
	addEditButton(1, 1, globalFile ? buttonPredictChange : editTip, "�޸�Ԥ�����");
}

void drawPrompt1(){
	double x = winWidth / 2;
	double y = winHeight / 2;
	double fH = GetFontHeight();
	double h = fH * 1.5; // �ؼ��߶�
	SetPenColor("White");
	drawRectangle(x - 1.5, y - 1, 4, 2, 1);
	SetPenColor("Black");
	drawRectangle(x - 1.5, y - 1, 4, 2, 0);
	drawLabel(x - 2 + 2 * fH + 1.5, y - 2 + fH + 2.5, "��δ���棬ȷ���˳���");
	if (button(GenUIID(0), x - 2 + 2 * fH + 1, y - 2 +fH + 1, 1, h, "ȷ��")){
		exit(-1);
	}
	if (button(GenUIID(1), x - 2 + 2 * fH + 2.5, y - 2 + fH + 1, 1, h, "ȡ��")){
		globalActive = 0;
	}
}

void drawPrompt2(){
	double x = winWidth / 2;
	double y = winHeight / 2;
	double fH = GetFontHeight();
	double h = fH * 1.5; // �ؼ��߶�
	SetPenColor("White");
	drawRectangle(x - 1.5, y - 1, 4, 2, 1);
	SetPenColor("Black");
	drawRectangle(x - 1.5, y - 1, 4, 2, 0);
	drawLabel(x - 1, y - 2 + fH + 2.5, "��ܰ��ʾ���������޳������ܣ�");
	drawLabel(x - 0.5, y + 0.5, "�Ƿ�Ҫ���е�ǰ������");
	if (button(GenUIID(0), x - 2 + 2 * fH + 1, y - 2 + fH + 1, 1, h, "��")){
		if (popStatus2 = 0);
		else if (popStatus2 = 1);
	}
	if (button(GenUIID(1), x-2+2*fH+2.5, y-2+fH+1, 1, h, "��")){
		globalActive = 0;
	}
}

void drawInputBox(){
	double x = winWidth / 2;
	double y = winHeight / 2;
	double fH = GetFontHeight();
	double h = fH * 1.5; // �ؼ��߶�
	SetPenColor("White");
	drawRectangle(x - 1.5, y - 1, 4, 2, 1);
	SetPenColor("Black");
	drawRectangle(x - 1.5, y - 1, 4, 2, 0);
	drawLabel(x - strlen(popInputTip) / 16, y - 2 + fH + 2.5, popInputTip);
	if (button(GenUIID(0), x - 2 + 2 * fH + 1, y - 2 + fH + 1, 1, h, "ȷ��")){
		if (popInputStatus == 0){//�޸Ĳ���
			editChange();
			popInputStatus = -1;//��ʼ��inputBox
		}
		else if (popInputStatus == 1){//������
			editNewLine();
			popInputStatus = -1;//��ʼ��inputBox
		}
		else if (popInputStatus == 2){//�½��ļ�
			fileNew();
			popInputStatus = -1;//��ʼ��inputBox
		}
		else if (popInputStatus == 3){//���ļ�
			fileOpen();
			popInputStatus = -1;//��ʼ��inputBox
		}
		else if (popInputStatus == 4){//�޸�Ԥ�����
			predictChange();
			popInputStatus = -1;//��ʼ��inputBox
		}
	}
	if (button(GenUIID(1), x - 2 + 2 * fH + 2.5, y - 2 + fH + 1, 1, h, "ȡ��")){
		globalActive = 0;
		globalStatus = 0;	
	}
	if( textbox(GenUIID(0), x - 0.5, y, 2, h, popInput, sizeof(popInput)));
}

void drawTipBox(){
	double x = winWidth / 2;
	double y = winHeight / 2;
	double fH = GetFontHeight();
	double h = fH * 1.5; // �ؼ��߶�
	SetPenColor("White");
	drawRectangle(x - 1.5, y - 1, 4, 2, 1);
	SetPenColor("Black");
	drawRectangle(x - 1.5,y - 1,4,2,0);
	drawLabel(x - strlen(popTip) / 8 * fH, y - 2 + fH + 2.5, popTip);
	if (button(GenUIID(0), x + 2 * fH - 0.5, y - 2 + fH + 1, 1, h, "ȷ��")) {
		globalActive = 0;
		globalStatus = 0;
	}
}
//״̬������      ����ȫ�ֱ���������ʾʲô״̬
void drawStatusBox(){
	double fH = GetFontHeight();
	if (globalStatus == 0 && globalFile == 0) drawLabel(11, fH, "״̬��Ϣ");
	if (globalStatus == 1) drawLabel(10.8, fH, "�޸��������");
	if (globalStatus == 2) drawLabel(11, fH, "������");
	if (globalStatus == 3) drawLabel(11, fH, "�½��ļ�");
	if (globalStatus == 4) drawLabel(11, fH, "���ļ�");
	if (globalStatus == 5) drawLabel(11, fH, "�����ļ�");
	if (globalStatus == 6) drawLabel(11, fH, "�޸�Ԥ�����");
	if (globalStatus == 0 && globalFile) drawLabel(11, fH, globalFitIn ? "��Ӧ����" : "��Ӧ����");
}
