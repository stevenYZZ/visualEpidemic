#ifndef _linklist_h
#define _linklist_h

//生成链表所需
struct record{
    char date[10];                  //日期
    int number[10];                 //各组数据人数
    struct record *prior, *next;    //链表指针
};
typedef struct record RECORD;

struct key{
    char name[20];                  //每组数据字段名
    struct key *prior, *next;    //链表指针
};
typedef struct key KEY;

RECORD* newLinkRECORD();													//新建一个头结点为空的RECORD链表
RECORD* searchLinkRECORD(RECORD* head, char datex[]);						//找出给定RECORD链表给定日期所在结点并返回
void addLinkRECORD(RECORD* tail,char datex[],int num[],int totalField);		//在RECORD链表尾部增加一个结点
void deleteLinkRECORD(RECORD* tail);										//在RECORD链表尾部删除一个节点
void writeLinkRECORD(RECORD* p0,int fieldNum, int numx);					//在给定RECORD链表结点中给定字段写入新值

KEY* newLinkKEY();															//新建一个头结点为空的KEY链表
void addLinkKEY(KEY* tail,char* namex);										//在KEY链表尾部增加一个结点
void deleteLinkKEY(KEY* tail);												//在KEY链表尾部删除一个节点
int getFieldNum(KEY *p0);													//得到给定结点在链表中位置													

#endif