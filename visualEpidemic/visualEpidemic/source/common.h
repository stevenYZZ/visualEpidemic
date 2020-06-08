#ifndef _common_h
#define _common_h

//全局变量
extern int globalEdit;

//可重复利用的标准函数

//int类型1变0，0变1函数
void commonTF(int *a);

//生成下一天日期字符串函数(将输入字符串改为新日期)
char *nextDate(char today[]);


#endif



