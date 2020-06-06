
#ifndef _edit_h
#define _edit_h


//全局变量
extern int globalEdit;

//杂七杂八函数
void empty();
void err();


//以下函数按button顺序排列
//编辑模式On/Off函数
void editOnOff();
//新建日期函数
void editNewDate();
//删除最后日期函数
void editDeleteLastDate();
//修改参数函数
void editChangeData();
//新增线函数
void editNewLine();
//删除线函数
void editDeleteLine();


#endif