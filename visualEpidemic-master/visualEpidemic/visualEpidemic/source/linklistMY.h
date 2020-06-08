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

RECORD* newLinkRECORD();
RECORD* addLinkRECORD(RECORD* head, RECORD* tail,char *datex,int num1,int num2);
KEY* newLinkKEY();
KEY* addLinkKEY(KEY* head, KEY* tail,char *namex);

#endif