#include "common.h"
#include "visualization.h"

//ȫ�ֱ���

//�����Ӱ˺���
void empty(){
	;
}

void err(){
	exit(-1);
}

void editTip(){
	if (!globalFile){
		globalActive = 4;
		strcpy(popTip, "��ǰδ�򿪻��½��ļ�");
	}
	else if (!globalEdit){
		globalActive = 4;
		strcpy(popTip, "��ǰδ�����༭ģʽ");
	}
}

//���º�����button˳������
//�༭ģʽOn/Off����

void editOnOff(){
	if (globalFile){
		commonTF(&globalEdit);
	}
	else {
		globalActive = 4;
		strcpy(popTip, "��ǰδ�򿪻��½��ļ�");	
	}

}

//�½����ں���
void editNewDate(){
	char datex[10];
	int numx[10];
	int i;

	if (globalEdit && globalFile){
		//��ʼ����������
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

//ɾ��������ں���
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
			strcpy(popTip, "�����ʣ1���ڵ㣬�޷�ɾ��");
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



//�޸������������
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
		console("��������ȷ��ʽ�Ĳ���");
		return;
	}
	sscanf(popInput, "%d", &num);
	p = searchLinkRECORD(rpHead, date);
	if (p == NULL){
		console("δ�ҵ��˽��");
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

//�����ߺ���
void editNewLine(){
	RECORD *p0 = rpHead;
	KEY *p1 = kpTail;
	int fieldNum;
	fieldNum = getFieldNum(kpTail);
	if (fieldNum >= 9){
		console("�������ﵽ����");
		return;
	}
	addLinkKEY(&kpTail, popInput);
	while (p0 != NULL){
		p0->number[fieldNum + 1] = 0;
		p0 = p0->next;
	}
	initHighLight(fieldNum+1);
	console("�����߳ɹ�");

	strcpy(popInput,"");
	//globalActive = 0;
	if (globalFitIn == 0){
		peopleDelta = getTotalPeopleNumber(rpHead, rpTail, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
	else {
		peopleDelta = getTotalPeopleNumber(rpHeadZoom, rpTailZoom, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
}

//ɾ���ߺ���
void editDeleteLine(){
	RECORD *p0 = rpHead;
	KEY *p1 = kp;
	if (globalEdit && globalFile){
		int fieldNum, fieldNumTotal, i, j, numx[10];
		fieldNumTotal = getFieldNum(kpTail);
		if (fieldNumTotal <= 0){
			console("ֻʣһ���ߣ��޷�����ɾ��");
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
		if (fieldNum == 0){						//ɾ��ͷ���
			kpHead = kpHead->next;
			deleteLinkKEY(kp);	
			kp = kpHead;
		}
		else if (fieldNum == fieldNumTotal){		//ɾ��β���
			kpTail = kpTail->prior;
			deleteLinkKEY(kp);	
			kp = kpHead;
		}
		else {									//ɾ���м���
			deleteLinkKEY(kp);	
			kp = kpHead;
		}
		initHighLight(fieldNumTotal - 1);
		console("ɾ���߳ɹ�");

	}
	else editTip();
	if (globalFitIn == 0){
		peopleDelta = getTotalPeopleNumber(rpHead, rpTail, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
	else {
		peopleDelta = getTotalPeopleNumber(rpHeadZoom, rpTailZoom, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}

}

//button��Ӧ����
//���button������Щ���������������򣬵ȴ��û��������ݺ���ִ��edit����
//�޸��������button��Ӧ
void buttonChange(){
	if (globalEdit && globalFile){
		strcpy(popInputTip, "�������޸ĺ��ֵ��");
		popInputStatus = 0;
		globalStatus = 1;
		globalActive = 3;
	}
	else {
		editTip();
	}
}
//������button��Ӧ
void buttonNewLine(){
	if (globalEdit && globalFile){
		strcpy(popInputTip, "�������µ��ֶ�����");
		popInputStatus = 1;
		globalStatus = 2;
		globalActive = 3;
	}else {
		editTip();
	}
}
