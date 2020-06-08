#ifndef _linklist_h
#define _linklist_h

//������������
struct record{
    char date[10];                  //����
    int number[10];                 //������������
    struct record *prior, *next;    //����ָ��
};
typedef struct record RECORD;

struct key{
    char name[20];                  //ÿ�������ֶ���
    struct key *prior, *next;    //����ָ��
};
typedef struct key KEY;

RECORD* newLinkRECORD();													//�½�һ��ͷ���Ϊ�յ�RECORD����
RECORD* searchLinkRECORD(RECORD* head, char datex[]);						//�ҳ�����RECORD��������������ڽ�㲢����
void addLinkRECORD(RECORD* tail,char datex[],int num[],int totalField);		//��RECORD����β������һ�����
void deleteLinkRECORD(RECORD* tail);										//��RECORD����β��ɾ��һ���ڵ�
void writeLinkRECORD(RECORD* p0,int fieldNum, int numx);					//�ڸ���RECORD�������и����ֶ�д����ֵ

KEY* newLinkKEY();															//�½�һ��ͷ���Ϊ�յ�KEY����
void addLinkKEY(KEY* tail,char* namex);										//��KEY����β������һ�����
void deleteLinkKEY(KEY* tail);												//��KEY����β��ɾ��һ���ڵ�
int getFieldNum(KEY *p0);													//�õ����������������λ��													

#endif