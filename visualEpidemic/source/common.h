#ifndef _common_h
#define _common_h

#include "linklistMY.h"

//ȫ�ֱ���
extern int globalActive;		//ȫ�ֻ�Ծҳ����� ��ʾ��ǰ��Ծҳ�棨0123�ֱ��Ӧ��ҳ��/�Ƿ��/�Ƿ�ȡ����/�������ȣ�
extern int globalEdit;			//��ҳ��༭ģʽ���� ��ʾ��ҳ���Ƿ��ڱ༭ģʽ��0��1�ǣ�
extern int globalPredict;		//��ҳ��Ԥ��ģʽ���� ��ʾ��ҳ���Ƿ���Ԥ��ģʽ��0��1�ǣ�
extern int globalPredictMode;	//ȫ��Ԥ��ģʽ��������ʾ��ǰ������Ԥ��ģʽ��0ָ������1�����ߺ�����
extern int globalPredictDays;	//ȫ��Ԥ��������������ʾԤ�������
extern int globalStatus;		//ȫ��״̬��������ʾ��ǰ���ڽ��еĲ���
extern int globalFile;			//ȫ���ļ���������ʾ��ǰ���޴򿪵��ļ���0��1�У�
extern int globalLabel;			//ȫ�ֱ�ע��������ʾ�Ƿ���ʾ���ֱ�ע��0��1�ǣ�
extern int globalDate;			//ȫ�����ڱ�ע��������ʾ�Ƿ���ʾ���ڱ�ע��0��1�ǣ�
extern int globalHelp;			//ȫ�ְ���ҳ���л���������ʾ��ǰ���ĸ����棨0��ͼ��1use��2about��
extern int globalFitIn;			//ȫ�ֻ�ͼ�����ޱ�������ʾ��ǰ������ȷ��������0��Ӧ���ݣ�1��Ӧ����
extern char globalFileName[100];//ȫ���ļ�����������ʾ��ǰ�򿪵��ļ���(���ַ���Ϊ��)

extern char popTip[200];		//��ʾ�ڵ�������ʾ������
extern char popInputTip[200];	//��ʾ�����������ʾ������
extern char popInput[200];		//��ʾ������е��û�����
extern int popStatus2;			//��ʾprompt2����״̬
extern int popInputStatus;//��ʾinputBox����״̬   0�޸Ĳ�����1������ 2�½��ļ� 3���ļ�
//���ظ����õı�׼����

//int����1��0��0��1����
void commonTF(int *a);

//������һ�������ַ�������(�������ַ�����Ϊ������)
char *nextDate(char today[]);

//�õ�RECORD��������������
int getTotalDays( RECORD* p );

//��������
void console(char s[]);

#endif



