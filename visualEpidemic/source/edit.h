
#ifndef _edit_h
#define _edit_h


//ȫ�ֱ���
extern int globalEdit;

//�����Ӱ˺���
void empty();
void err();
void editTip();

//���º�����button˳������
//�༭ģʽOn/Off����
void editOnOff();
//�½����ں���
void editNewDate();
//ɾ��������ں���
void editDeleteLastDate();
//�޸������������
void editChange();
//�����ߺ���
void editNewLine();
//ɾ���ߺ���
void editDeleteLine();


//button��Ӧ����
//���button������Щ���������������򣬵ȴ��û��������ݺ���ִ��edit����
//�޸��������button��Ӧ
void buttonChange();
//������button��Ӧ
void buttonNewLine();

#endif