#include "common.h"
#include "visualization.h"

//全局变量

//杂七杂八函数
void empty(){
	;
}

void err(){
	exit(-1);
}

void editTip(){
	if (!globalFile){
		globalActive = 4;
		strcpy(popTip, "当前未打开或新建文件");
	}
	else if (!globalEdit){
		globalActive = 4;
		strcpy(popTip, "当前未开启编辑模式");
	}
}

//以下函数按button顺序排列
//编辑模式On/Off函数

void editOnOff(){
	if (globalFile){
		commonTF(&globalEdit);
	}
	else {
		globalActive = 4;
		strcpy(popTip, "当前未打开或新建文件");	
	}

}

//新建日期函数
void editNewDate(){
	char datex[10];
	int numx[10];
	int i;

	if (globalEdit && globalFile){
		//初始化输入数据
		for (i=0; i<=getKeyNumber(kp); i++){
			numx[i] = 0;
		}
		strcpy(datex, rpTail->date);
		addLinkRECORD(&rpTail, nextDate(datex), numx, getKeyNumber(kp));

		while(rpTailZoom!=rpTail){
			rpTailZoom = rpTailZoom->next;
			rpHeadZoom = rpHeadZoom->next;
		}
		rp=rpTail;


	}
	else {
		editTip();
	}
	if (globalFitIn == 0){
		peopleDelta = getTotalPeopleNumber(rpHead, rpTail, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
	else {
		peopleDelta = getTotalPeopleNumber(rpHeadZoom, rpTailZoom, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
}

//删除最后日期函数
void editDeleteLastDate(){
	if (globalEdit && globalFile){
		if (rpHead != rpTail){
			if (rpTail == rpTailZoom){
				rpTailZoom = rpTailZoom->prior;
				if (rpHeadZoom->prior != NULL){
					rpHeadZoom = rpHeadZoom->prior;
				}
			}
			if(rp==rpTail)rp=rp->prior;
			rpTail = rpTail->prior;
			deleteLinkRECORD(rpTail->next);
		}
		else {
			globalActive = 4;
			strcpy(popTip, "链表仅剩1个节点，无法删除");
		}

	}
	else {
		editTip();
	}
	if (globalFitIn == 0){
		peopleDelta = getTotalPeopleNumber(rpHead, rpTail, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
	else {
		peopleDelta = getTotalPeopleNumber(rpHeadZoom, rpTailZoom, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
}



//修改链表参数函数
void editChange(){
	char date[10];
	int num, fieldNum, i;
	RECORD *p;
	for (i=0; i<getKeyNumber(kpHead); i++){
		if (highLight[i][2] == 1){
			fieldNum = i;
			break;
		}
	}
	strcpy(date, rp->date);
	if (strspn(popInput, "0123456789") != strlen(popInput)){
		console("请输入正确格式的参数");
		return;
	}
	sscanf(popInput, "%d", &num);
	p = searchLinkRECORD(rpHead, date);
	if (p == NULL){
		console("未找到此结点");
		return;
	}
	else {
		p->number[fieldNum] = num;
	}
	strcpy(popInput,"");
	globalActive = 0;
	if (globalFitIn == 0){
		peopleDelta = getTotalPeopleNumber(rpHead, rpTail, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
	else {
		peopleDelta = getTotalPeopleNumber(rpHeadZoom, rpTailZoom, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
}

//新增线函数
void editNewLine(){
	RECORD *p0 = rpHead;
	KEY *p1 = kpTail;
	int fieldNum;
	fieldNum = getFieldNum(kpTail);
	if (fieldNum >= 9){
		console("线数量达到上限");
		return;
	}
	addLinkKEY(&kpTail, popInput);
	while (p0 != NULL){
		p0->number[fieldNum + 1] = 0;
		p0 = p0->next;
	}
	initHighLight(fieldNum+1);
	console("新增线成功");

	strcpy(popInput,"");
	//globalActive = 0;
	if (globalFitIn == 0){
		peopleDelta = getTotalPeopleNumber(rpHead, rpTail, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
	else {
		peopleDelta = getTotalPeopleNumber(rpHeadZoom, rpTailZoom, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
}

//删除线函数
void editDeleteLine(){
	RECORD *p0 = rpHead;
	KEY *p1 = kp;
	if (globalEdit && globalFile){
		int fieldNum, fieldNumTotal, i, j, numx[10];
		fieldNumTotal = getFieldNum(kpTail);
		if (fieldNumTotal <= 0){
			console("只剩一条线，无法继续删除");
			return;
		}
		for (i=0; i<getKeyNumber(kpHead); i++){
			if (highLight[i][2] == 1){
				fieldNum = i;
				break;
			}
		}
		while (p0 != NULL){
			for (i=fieldNum; i<fieldNumTotal; i++){
				p0->number[i] = p0->number[i + 1];
			}
			p0->number[fieldNumTotal] = 0;
			p0 = p0->next;
		}
		if (fieldNum == 0){						//删除头结点
			kpHead = kpHead->next;
			deleteLinkKEY(kp);	
			kp = kpHead;
		}
		else if (fieldNum == fieldNumTotal){		//删除尾结点
			kpTail = kpTail->prior;
			deleteLinkKEY(kp);	
			kp = kpHead;
		}
		else {									//删除中间结点
			deleteLinkKEY(kp);	
			kp = kpHead;
		}
		initHighLight(fieldNumTotal - 1);
		console("删除线成功");

	}
	else editTip();
	if (globalFitIn == 0){
		peopleDelta = getTotalPeopleNumber(rpHead, rpTail, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
	else {
		peopleDelta = getTotalPeopleNumber(rpHeadZoom, rpTailZoom, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}

}

//button响应函数
//点击button触发这些函数后会跳出输入框，等待用户输入内容后再执行edit函数
//修改链表参数button响应
void buttonChange(){
	if (globalEdit && globalFile){
		strcpy(popInputTip, "请输入修改后的值：");
		popInputStatus = 0;
		globalStatus = 1;
		globalActive = 3;
	}
	else {
		editTip();
	}
}
//新增线button响应
void buttonNewLine(){
	if (globalEdit && globalFile){
		strcpy(popInputTip, "请输入新的字段名：");
		popInputStatus = 1;
		globalStatus = 2;
		globalActive = 3;
	}else {
		editTip();
	}
}
