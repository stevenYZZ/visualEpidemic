#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include <time.h>
#include "imgui.h"

#include "visualization.h"
//ȫ�ֱ���
int globalActive = 0;		//ȫ�ֻ�Ծҳ����� ��ʾ��ǰ��Ծҳ�棨0123�ֱ��Ӧ��ҳ��/�Ƿ��/�Ƿ�ȡ����/�������ȣ�
int globalEdit = 0;			//��ҳ��༭ģʽ���� ��ʾ��ҳ���Ƿ��ڱ༭ģʽ��0��1�ǣ�
int globalPredict = 0;		//��ҳ��Ԥ��ģʽ���� ��ʾ��ҳ���Ƿ���Ԥ��ģʽ��0��1�ǣ�
int globalStatus = 0;		//ȫ��״̬��������ʾ��ǰ���ڽ��еĲ���
int globalFile = 1;			//ȫ���ļ���������ʾ��ǰ���޴򿪵��ļ���0��1�У�

char popTip[200];		//��ʾ����ʾ������ʾ������
char popInputTip[200];	//��ʾ�����������ʾ������
char popInput[200];		//��ʾ������е��û�����
int popStatus2 = -1;	//��ʾprompt2����״̬
int popInputStatus = -1;//��ʾinputBox����״̬   0�޸Ĳ�����1������


//���ļ����������Ӱ˺���


//�ж��Ƿ�Ϊ���꺯��
int ifRun(int n){
	if((n%4==0) && (n%100!=0) || (n%400==0))return 1;
	else return 0;
}

//���ظ����õı�׼����
//int����1��0��0��1����
void commonTF(int *a){
	if(*a==0)*a=1;
	else *a=0;

}

//�򿪵�ǰ�ļ������ļ�
void OpenFiles(char *p){
	FILE*fp;
	if((fp=fopen(p,"a+"))==NULL){
		exit(0);
	}
} 






//״̬������      ����ȫ�ֱ���������ʾʲô״̬
void Status(int n){
	double fH = GetFontHeight();
	if(n==1) drawLabel(0,fH,"�༭ģʽ������");

}


//������һ�������ַ�������(�������ַ�����Ϊ������)
char *nextDate(char today[]){
	char year[4];char month[2];char day[2]; //char����������
	int y,m,d;			//int����������
	int mAdd=0;     //�·��Ƿ��λ
	
	//�õ�int����������
	strncpy(year, today, 4);y=atoi(year);
	strncpy(month, today+4, 2);m=atoi(month);
	strncpy(day, today+6, 2);d=atoi(day);

	//����
	if(d==28 && !ifRun(y) && m==2){
			d=0;
			mAdd++;
	}else if(d==29 && ifRun(y) && m==2){
			d=0;
			mAdd++;
	}else if(d==30 && (m==4 || m==6 || m==9 || m==11)){
			d=0;
			mAdd++;
	}else if(d==31 && m!=2 && m!=4 && m!=6 && m!=9 && m!=11){
			d=0;
			mAdd++;
	}else;
	d=d+1;
	if(m==12 && mAdd==1){
		y++;
		m=1;
	}else m=m+mAdd;

	//������ƴ�ӳ�char����
	sprintf(today,"%d%02d%02d",y,m,d);
	return today;
}