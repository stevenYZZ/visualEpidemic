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

RECORD* newLinkRECORD();
RECORD* addLinkRECORD(RECORD* head, RECORD* tail,char *datex,int num1,int num2);
KEY* newLinkKEY();
KEY* addLinkKEY(KEY* head, KEY* tail,char *namex);

#endif