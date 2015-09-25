/***ͷ�ļ���
ֻ������һЩ���� Date.h
author:wallwind  time 2011/8/29
**/
#pragma once
#include "duilib.h"

#include <string>
#include <iostream>

static TCHAR Zodiac[12]={_T('��'),_T('ţ'),_T('��'),_T('��'),_T('��'),_T('��'),_T('��'),_T('��'),_T('��'),_T('��'),_T('��'),_T('��')};


class CDate  //����һ��Date�࣬
{
	int year,month,day;//
public:
	static int DayOfMonth(int y,int m);//����һ���µ�������
	int ToInt();//
public:
	CDate() //���캯������ʼ��Ĭ�ϵ�����
		:year(1900),month(1),day(1)
	{
	}
	CDate(int y,int m,int d)
		:year(y),month(m),day(d)//�����ڸ�ֵ���ж���Ч�����ڡ�
	{
		if((y<=0)||(m<=0 || m>12)||(d<=0 || d>DayOfMonth(y,m)))
		{
			year = 1900;
			month = day = 1;
		}
	}
	virtual ~CDate()//������������
	{
	}
	int GetYear()const//�������
	{
		return year;
	}

	int GetMonth()const   //�����·�
	{
		return month;
	}
	int GetDay()const   //��������
	{
		return day;
	}
	bool IsLeapyear() const    //�ж��Ƿ�Ϊƽ�ꡣ
	{
		return year%400?(year%100?(year%4?(false):(true)):(false)):true;
	}
	static bool IsLeapyear(const int y)  //�ж��Ƿ�Ϊƽ�ꡣ
	{
		return y%400?(y%100?(y%4?false:true):false):true;
	}

	void Display() const  //�������ֵ
	{
		std::cout<<year<<"-"<<month<<"-"<<day<<std::endl;
	}

	TCHAR GetZodiac()
	{
		int n = year%12;
		return Zodiac[n];
	}

	int GetAge()
	{
		SYSTEMTIME sys;

		GetLocalTime( &sys );
		int nowYear = sys.wYear;
		int nowMonth = sys.wMonth;
		int _age = nowYear - year-(nowMonth < month);
		return _age;
	}
	static CDate GetCurDate()
	{
		SYSTEMTIME sys;

		GetLocalTime( &sys );
		return CDate(sys.wYear, sys.wMonth, sys.wDay);
	}
	CString constell() 
	{ 
		CString c[12][2]={  {_T("ħЫ��"),_T("ˮƿ��")}, {_T("ˮƿ��"),_T("˫����")}, {_T("˫����"),_T("������")},  
		{_T("������"),_T("��ţ��")}, {_T("��ţ��"),_T("˫����")}, {_T("˫����"),_T("��з��")}, 
		{_T("��з��"),_T("ʨ����")}, {_T("ʨ����"),_T("��Ů��")}, {_T("��Ů��"),_T("�����")}, 
		{_T("�����"),_T("��Ы��")}, {_T("��Ы��"),_T("������")}, {_T("������"),_T("ħЫ��")}} ;

		int x=month-1; 
		int y=day/21; 
		return c[x][y]; 
	} 

};
