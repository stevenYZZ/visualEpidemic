#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include <time.h>
#include "imgui.h"

//�������Դ�ͷ�ļ�
#include "common.h"

//ȫ�ֱ���



//���º�����button˳������
//�༭ģʽOn/Off����

void editOnOff(){
	commonTF(&globalEdit);
}

//�½����ں���
void editNewDate();

//ɾ��������ں���
void editDeleteLastDate();

//�޸Ĳ�������
void editChangeData();

//�����ߺ���
void editNewLine();

//ɾ���ߺ���
void editDeleteLine();
