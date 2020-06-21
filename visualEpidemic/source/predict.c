#include "common.h"
#include "linklistMY.h"
#include "visualization.h"
#include "edit.h"
#include "files.h"

//求数学期望
double E(int num[],int n){
	int i;
	double sum=0;
	for(i=0;i<n;i++){
		sum+=num[i];
	}
	return sum/n;
}
//求方差
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
	int totalDays;			//原RECORD链表总天数
	int totalField;			//KEY链表结点总数
	int i,j;
	int nump[10];			//新的预测链表某个结点的number
	int numy[10][200];		//样本y坐标
	int numx[10][200];		//样本x坐标
	int x;				//预测函数自变量的值
	char lastDate[10];
	double a[10],b[10],k[10];		//求得的预测函数参数

	strcpy(lastDate,rpTail->date);
	totalDays=getTotalDays(rpHead);
	totalField=getFieldNum(kpTail)+1;
	
	if(totalDays<=5){
		console("天数过少，无法预测");
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

	if(globalPredictMode==0){				//线性函数暴力拟合  y=kx+b
		for(i=0;i<totalField;i++){
			k[i]= E(numy[i],totalDays)/E(numx[i],totalDays);	//暴力拟合参数
		}
		x=totalDays+2;	
	}
	pHead=newLinkRECORD();
	strcpy(pHead->date,nextDate(lastDate));

	if(globalPredictMode==0){
		for(j=0;j<totalField;j++){
			pHead->number[j]=x*k[j];//暴力拟合参数
		}
	}
	x++;
	pTail=pHead;
	for(i=0;i<globalPredictDays-1;i++){
		if(globalPredictMode==0){
			for(j=0;j<totalField;j++){
				//nump[j]=(int)a[j]*exp(b[j]*x);
				nump[j]=x*k[j];//暴力拟合参数
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

//预测button响应
void buttonPredict(){
	globalPredict=1;
	connnect(rpTail,predict());
}

//修改预测参数button响应
void buttonPredictChange(){
	if (globalEdit && globalFile){
		strcpy(popInputTip, "请输入要预测的天数：");
		sprintf(popInput,"%d",globalPredictDays);
		popInputStatus = 4;
		globalStatus = 6;
		globalActive = 3;
	}
	else {
		editTip();
	}
}

//修改预测参数
void predictChange(){
	int num;
	if (strspn(popInput, "0123456789") != strlen(popInput)){
		console("请输入正确格式的参数");
		return;
	}
	sscanf(popInput,"%d",&num);
	globalPredictDays = num;
	strcpy(popInput,"");
	console("修改成功，点击预测按钮即可");
}

