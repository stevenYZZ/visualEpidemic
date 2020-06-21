#include "common.h"
#include "linklistMY.h"
#include "visualization.h"
#include "edit.h"
#include "files.h"

//����ѧ����
double E(int num[],int n){
	int i;
	double sum=0;
	for(i=0;i<n;i++){
		sum+=num[i];
	}
	return sum/n;
}
//�󷽲�
double Var(int num[],int n){
	int i;
	double sum=0;
	double e=E(num,n);
	for(i=0;i<n;i++){
		sum+=pow(num[i]-e,2);
	}
	return sum/n;
}

RECORD * predict(){
	RECORD *pHead,*pTail,*p;
	int totalDays;			//ԭRECORD����������
	int totalField;			//KEY����������
	int i,j;
	int nump[10];			//�µ�Ԥ������ĳ������number
	int numy[10][200];		//����y����
	int numx[10][200];		//����x����
	int x;				//Ԥ�⺯���Ա�����ֵ
	char lastDate[10];
	double a[10],b[10],k[10];		//��õ�Ԥ�⺯������

	strcpy(lastDate,rpTail->date);
	totalDays=getTotalDays(rpHead);
	totalField=getFieldNum(kpTail)+1;
	
	if(totalDays<=5){
		console("�������٣��޷�Ԥ��");
		return NULL;
	}
	p=rpHead;
	j=0;
	while(p->next!=NULL){
		for(i=0;i<totalField;i++){
			numy[i][j]=p->number[i];
			numx[i][j]=j;
		}
		j++;
		p=p->next;
	}
		for(i=0;i<totalField;i++){
			numy[i][j]=p->number[i];
			numx[i][j]=j;
		}

	if(globalPredictMode==0){				//���Ժ����������  y=kx+b
		for(i=0;i<totalField;i++){
			k[i]= E(numy[i],totalDays)/E(numx[i],totalDays);	//������ϲ���
		}
		x=totalDays+2;	
	}
	pHead=newLinkRECORD();
	strcpy(pHead->date,nextDate(lastDate));

	if(globalPredictMode==0){
		for(j=0;j<totalField;j++){
			pHead->number[j]=x*k[j];//������ϲ���
		}
	}
	x++;
	pTail=pHead;
	for(i=0;i<globalPredictDays-1;i++){
		if(globalPredictMode==0){
			for(j=0;j<totalField;j++){
				//nump[j]=(int)a[j]*exp(b[j]*x);
				nump[j]=x*k[j];//������ϲ���
			}
		}else;
		addLinkRECORD(&pTail,nextDate(lastDate),nump,totalField);
		x++;
	}
	if (globalFitIn == 0){
		peopleDelta = getTotalPeopleNumber(rpHead, rpTail, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
	else {
		peopleDelta = getTotalPeopleNumber(rpHeadZoom, rpTailZoom, getKeyNumber(kpHead), &peopleMax, &peopleMin);
	}
	//shiyan
	return pHead;

}

//Ԥ��button��Ӧ
void buttonPredict(){
	globalPredict=1;
	connnect(rpTail,predict());
}

//�޸�Ԥ�����button��Ӧ
void buttonPredictChange(){
	if (globalEdit && globalFile){
		strcpy(popInputTip, "������ҪԤ���������");
		sprintf(popInput,"%d",globalPredictDays);
		popInputStatus = 4;
		globalStatus = 6;
		globalActive = 3;
	}
	else {
		editTip();
	}
}

//�޸�Ԥ�����
void predictChange(){
	int num;
	if (strspn(popInput, "0123456789") != strlen(popInput)){
		console("��������ȷ��ʽ�Ĳ���");
		return;
	}
	sscanf(popInput,"%d",&num);
	globalPredictDays = num;
	strcpy(popInput,"");
	console("�޸ĳɹ������Ԥ�ⰴť����");
}

