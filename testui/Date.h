/***头文件。
只进行了一些定义 Date.h
author:wallwind  time 2011/8/29
**/
#pragma once
#include "duilib.h"

#include <string>
#include <iostream>

static TCHAR Zodiac[12]={_T('鼠'),_T('牛'),_T('虎'),_T('兔'),_T('龙'),_T('蛇'),_T('马'),_T('羊'),_T('猴'),_T('鸡'),_T('狗'),_T('猪')};


class CDate  //创建一个Date类，
{
	int year,month,day;//
public:
	static int DayOfMonth(int y,int m);//返回一个月的天数。
	int ToInt();//
public:
	CDate() //构造函数，初始化默认的日期
		:year(1900),month(1),day(1)
	{
	}
	CDate(int y,int m,int d)
		:year(y),month(m),day(d)//对日期赋值，判断有效的日期。
	{
		if((y<=0)||(m<=0 || m>12)||(d<=0 || d>DayOfMonth(y,m)))
		{
			year = 1900;
			month = day = 1;
		}
	}
	virtual ~CDate()//虚析构函数。
	{
	}
	int GetYear()const//返回年份
	{
		return year;
	}

	int GetMonth()const   //返回月份
	{
		return month;
	}
	int GetDay()const   //返回日期
	{
		return day;
	}
	bool IsLeapyear() const    //判断是否为平年。
	{
		return year%400?(year%100?(year%4?(false):(true)):(false)):true;
	}
	static bool IsLeapyear(const int y)  //判断是否为平年。
	{
		return y%400?(y%100?(y%4?false:true):false):true;
	}

	void Display() const  //输出日期值
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
		CString c[12][2]={  {_T("魔蝎座"),_T("水瓶座")}, {_T("水瓶座"),_T("双鱼座")}, {_T("双鱼座"),_T("白羊座")},  
		{_T("白羊座"),_T("金牛座")}, {_T("金牛座"),_T("双子座")}, {_T("双子座"),_T("巨蟹座")}, 
		{_T("巨蟹座"),_T("狮子座")}, {_T("狮子座"),_T("处女座")}, {_T("处女座"),_T("天秤座")}, 
		{_T("天秤座"),_T("天蝎座")}, {_T("天蝎座"),_T("射手座")}, {_T("射手座"),_T("魔蝎座")}} ;

		int x=month-1; 
		int y=day/21; 
		return c[x][y]; 
	} 

};
