/*������ ���������, ���������� �� ����*/
#pragma once
#include "Const.h"
#include "Predef.h"

#include "TEventQue.h"
#include "TEvent.h"
class TNetIOParser
{
	/*��������� ��������� ����� 1, ������ ��������� ����� 2, ������������ ��������� �������������*/
public:
	TNetIOParser(int CLID) 
		:bufer("") 
		, ClientID(CLID)
	{};
	void AddData(char* buf);//���������� ������

	TEventQue q;//������� 

	bool HaveDataToRead() { return _haveDataToRead; };
	shared_ptr<IEvent> GetNextEvent();	//��������� ����� ����������
private:
	string bufer;//�����
	bool _haveDataToRead;//����� ����� ������ ���� ��� ���������
	int _datast;//������ ���������
	int _dataen;// ����� ���������
	int ClientID;//�� ������� ��� ��������� ���������

	shared_ptr<IEvent> CreateIncomingEvent(vector<string> &v);//������� �� ������ - ���������
	void checkData();
};

