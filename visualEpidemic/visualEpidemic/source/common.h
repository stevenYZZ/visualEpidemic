#ifndef _common_h
#define _common_h

//ȫ�ֱ���
extern int globalActive;		//ȫ�ֻ�Ծҳ����� ��ʾ��ǰ��Ծҳ�棨0123�ֱ��Ӧ��ҳ��/�Ƿ��/�Ƿ�ȡ����/�������ȣ�
extern int globalEdit;			//��ҳ��༭ģʽ���� ��ʾ��ҳ���Ƿ��ڱ༭ģʽ��0��1�ǣ�
extern int globalPredict;		//��ҳ��Ԥ��ģʽ���� ��ʾ��ҳ���Ƿ���Ԥ��ģʽ��0��1�ǣ�
extern int globalStatus;		//ȫ��״̬��������ʾ��ǰ���ڽ��еĲ���
extern int globalFile;			//ȫ���ļ���������ʾ��ǰ���޴򿪵��ļ���0��1�У�
	
extern char popTip[200];		//��ʾ�ڵ�������ʾ������
extern char popInputTip[200];	//��ʾ�����������ʾ������
extern char popInput[200];		//��ʾ������е��û�����
extern int popStatus2;				//��ʾ����״̬
//���ظ����õı�׼����

//int����1��0��0��1����
void commonTF(int *a);

//������һ�������ַ�������(�������ַ�����Ϊ������)
char *nextDate(char today[]);
void OpenFiles(char *p);

//��������
void callPrompt2(int n);
void callInputBox(int n);




#endif



