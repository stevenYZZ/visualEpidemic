#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

//�������Դ�ͷ�ļ�
#include "visualization.h"
#include "linklistMY.h"

//ȫ�ֱ���
int globalActive = 0;		//ȫ�ֻ�Ծҳ����� ��ʾ��ǰ��Ծҳ�棨0123�ֱ��Ӧ��ҳ��/�Ƿ��/�Ƿ�ȡ����/�������ȣ�
int globalEdit = 0;			//��ҳ��༭ģʽ���� ��ʾ��ҳ���Ƿ��ڱ༭ģʽ��0��1�ǣ�
int globalPredict = 0;		//��ҳ��Ԥ��ģʽ���� ��ʾ��ҳ���Ƿ���Ԥ��ģʽ��0��1�ǣ�
int globalPredictMode = 0;	//ȫ��Ԥ��ģʽ��������ʾ��ǰ������Ԥ��ģʽ��0ָ������1�����ߺ�����
int globalPredictDays = 10;	//ȫ��Ԥ��������������ʾԤ�������
int globalStatus = 0;		//ȫ��״̬��������ʾ��ǰ���ڽ��еĲ���
int globalFile = 0;			//ȫ���ļ���������ʾ��ǰ���޴򿪵��ļ���0��1�У�
int globalLabel = 0;		//ȫ�����ֱ�ע��������ʾ�Ƿ���ʾ���ֱ�ע��0��1�ǣ�
int globalDate = 1;			//ȫ�����ڱ�ע��������ʾ�Ƿ���ʾ���ڱ�ע��0��1�ǣ�
int globalHelp = 0;			//ȫ�ְ���ҳ���л���������ʾ��ǰ���ĸ����棨0��ͼ��1use��2about��
int globalFitIn = 0;		//ȫ�ֻ�ͼ�����ޱ�������ʾ��ǰ������ȷ��������0��Ӧ���ݣ�1��Ӧ����
char globalFileName[100] = "";	//ȫ���ļ�����������ʾ��ǰ�򿪵��ļ���(���ַ���Ϊ��)

char popTip[200];		//��ʾ����ʾ������ʾ������
char popInputTip[200];	//��ʾ�����������ʾ������
char popInput[200];		//��ʾ������е��û�����
int popStatus2 = -1;	//��ʾprompt2����״̬
int popInputStatus = -1;//��ʾinputBox����״̬   0�޸Ĳ�����1�����ߣ�2�½��ļ���3�����ļ�

//���ļ����������Ӱ˺���

//�ж��Ƿ�Ϊ���꺯��
int ifRun(int n){
	if ((n % 4 == 0) && (n % 100 != 0) || (n % 400 == 0)){
		return 1;
	}
	else {
		return 0;
	}
}

//���ظ����õı�׼����
//int����1��0��0��1����
void commonTF(int *a){
	if (*a == 0){
		*a = 1;
	}
	else{
		*a = 0;
	}

}

//�õ�RECORD��������������
int getTotalDays(RECORD *p){
	int count = 0;
	while (p->next != NULL){
		count++;
		p = p->next;
	}
	count++;
	return count;
}

//������һ�������ַ�������(�������ַ�����Ϊ������)
char *nextDate(char today[]){
	char year[4]; char month[2]; char day[2]; //char����������
	int y, m, d;			//int����������
	int mAdd = 0;     //�·��Ƿ��λ

	//�õ�int����������
	strncpy(year, today, 4); y = atoi(year);
	strncpy(month, today + 4, 2); m = atoi(month);
	strncpy(day, today + 6, 2); d = atoi(day);

	//����
	if (d == 28 && !ifRun(y) && m == 2){
		d = 0;
		mAdd++;
	}
	else if (d == 29 && ifRun(y) && m == 2){
		d = 0;
		mAdd++;
	}
	else if (d == 30 && (m == 4 || m == 6 || m == 9 || m == 11)){
		d = 0;
		mAdd++;
	}
	else if (d == 31 && m != 2 && m != 4 && m != 6 && m != 9 && m != 11){
		d = 0;
		mAdd++;
	}
	else;
	d = d+1;
	if (m == 12 && mAdd == 1){
		y++;
		m = 1;
	}
	else {
		m = m + mAdd;
	}

	//������ƴ�ӳ�char����
	sprintf(today, "%d%02d%02d", y, m, d);
	return today;
}

void console(char s[]){
	globalActive = 4;
	strcpy(popTip, s);
}