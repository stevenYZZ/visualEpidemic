#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <time.h>
#include "imgui.h"

//�������Դ�ͷ�ļ�
#include "common.h"
#include "visualization.h"


void fileNew(){
	FILE *fp;
	rpHead = newLinkRECORD();
	rpTail = rpHead;
	kpHead = newLinkKEY();
	kpTail = kpHead;
	strcpy(kpHead->name, "first");
	strcpy(rpHead->date, "20191201");
	rpHead->number[0] = 0;
	globalStatus = 3;
	if ((fp = fopen(popInput, "w")) == NULL){
		console("�����ļ�����");
		return;
	}
	fprintf(fp, "1 first 1\n20191201 0");
	if (fclose(fp)){
		console("�ļ�δ��ȷ�ر�");
	}
	strcpy(globalFileName, popInput);
	console("�½��ɹ�");
	//����Ϊ��bug����
	rpTailZoom = rpTail;
	rpHeadZoom = rpHead;
	kp = kpHead;
	rp = rpHead;

	strcpy(popInput, "");
	globalFile = 1;
	if (globalFitIn == 0){
		peopleDelta = getTotalPeopleNumber(rpHead, rpTail, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
	else {
		peopleDelta = getTotalPeopleNumber(rpHeadZoom, rpTailZoom, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
	initHighLight(1);

}

void fileOpen(){
	FILE *fp;
	int i, j, num[10], n;
	int totalField, totalDays;
	char namex[20], datex[10];
	RECORD *temp = NULL;

	rpHead = newLinkRECORD();
	rpTail = rpHead;
	kpHead = newLinkKEY();
	kpTail = kpHead;
	globalStatus = 4;
	if ((fp = fopen(popInput, "r")) == NULL){
		console("δ�ҵ�ԭ�ļ����ļ�������");
		return;
	}
	fscanf(fp, "%d", &totalField);
	for (i=0; i<totalField; i++){
		fscanf(fp, "%s ", namex);
		if (i == 0){
			strcpy(kpHead->name, namex);
		}
		else {
			addLinkKEY(&kpTail, namex);
		}
	}
	fscanf(fp, "%d",&totalDays);
	for (i=0; i<totalDays; i++){
		if (i == 0){
			fscanf(fp, "%s", datex);
			strcpy(rpHead->date, datex);
			for (j=0; j<totalField; j++){
				fscanf(fp, "%d", &num[j]);
				rpHead->number[j] = num[j];
			}
		}
		else {
			fscanf(fp, "%s", datex);
			for (j=0; j<totalField; j++){
				fscanf(fp, "%d", &num[j]);
			}
			addLinkRECORD(&rpTail, datex, num, totalField);
		}
	}
	if (fclose(fp)){
		console("�ļ�δ��ȷ�ر�");
	}
	strcpy(globalFileName, popInput);
	console("�򿪳ɹ�");

	//����Ϊ��bug����
	temp = rpHead;
	rpTailZoom = rpTail;
	rpHeadZoom = rpHead;
	if (getDateNumber(rpHeadZoom, rpTailZoom) > MAX_DATENUMBER_SHOW)
	{
		for (i=0; i<(MAX_DATENUMBER_SHOW-1); i++){
			temp = temp->next;
		}
		rpTailZoom = temp;
	}
	else{;}
	kp = kpHead;
	rp = rpHead;
	n = getKeyNumber(kp);
	if (globalFitIn == 0){
		peopleDelta = getTotalPeopleNumber(rpHead, rpTail, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
	else {
		peopleDelta = getTotalPeopleNumber(rpHeadZoom, rpTailZoom, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}

	strcpy(popInput, "");
	globalFile = 1;
	initHighLight(totalField);
}

void fileSave(){
	FILE * fp;
	RECORD *p0 = rpHead;
	KEY *p1 = kpHead;
	int i;
	int totalField;
	globalStatus = 5;
	if (!globalFile){
		console("��ǰδ�򿪻��½��ļ�");
		return;
	}
	totalField = getKeyNumber(kpHead);
	if ((fp = fopen(globalFileName, "r+")) == NULL){
		console("δ�ҵ�ԭ�ļ�");
		return;
	}
	fprintf(fp, "%d ", totalField);
	while (p1 != NULL){
		fprintf(fp, "%s ", p1->name);
		p1 = p1->next;
	}
	fprintf(fp, "%d\n", getTotalDays(rpHead));
	while (p0->next != NULL){
		fprintf(fp, "%s ", p0->date);
		for (i=0; i<totalField; i++){
			fprintf(fp, "%d", p0->number[i]);
			if (i != totalField){
				fprintf(fp, " ");
			}
		}
		fprintf(fp, "\n");
		p0 = p0->next;
	}//while
	fprintf(fp, "%s ", p0->date);
	for (i=0; i<totalField; i++){
		fprintf(fp, "%d", p0->number[i]);
		if (i != totalField){
			fprintf(fp, " ");
		}
	}
	if (fclose(fp)){
		console("�ļ�δ��ȷ�ر�");
	}
	console("����ɹ�");

}
