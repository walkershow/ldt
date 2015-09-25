
/***
    Date 日期类实现。//Date.cpp  2011/8/29
author:wallwind  time 2011/8/29
***/

#include "Date.h"
//得到每个月的天数。。。。
int CDate::DayOfMonth(int y,int m)
{
    int d = 0;
    switch(m)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        d = 31;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        d = 30;
        break;
    case 2:
        d = 28 + IsLeapyear(y);
        break;
    }
    return d;
}
int CDate::ToInt()
{
    int sum =0;
    for(int y=1;y<year;++y)
    {
        sum += 365+IsLeapyear(y);
    }
    for(int m=1;m<month;++m)
        sum += DayOfMonth(year,m);
    sum += day;
    return sum;
}
