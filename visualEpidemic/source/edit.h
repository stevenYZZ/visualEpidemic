
#ifndef _edit_h
#define _edit_h


//全局变量
extern int globalEdit;

//杂七杂八函数
void empty();
void err();
void editTip();

//以下函数按button顺序排列
//编辑模式On/Off函数
void editOnOff();
//新建日期函数
void editNewDate();
//删除最后日期函数
void editDeleteLastDate();
//修改链表参数函数
void editChange();
//新增线函数
void editNewLine();
//删除线函数
void editDeleteLine();


//button响应函数
//点击button触发这些函数后会跳出输入框，等待用户输入内容后再执行edit函数
//修改链表参数button响应
void buttonChange();
//新增线button响应
void buttonNewLine();

#endif