#ifndef CSPFILE_H
#define CSPFILE_H
#include <cspdatabase.h>
#include <cspindex.h>
#include <cspcities.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

class CSpFile
{
public:
    CSpFile();
    CSpDatabase Database;
    int PropertyCapacity;
    CSpProperty* Properties;
    CSpIndex index;
    int CityCapacity;
    CSpCities* Cities;
    bool LoadFile();
    bool LoadDat();
    bool LoadOpt();
    bool LoadTxt();
    void ShowProperties();
    void ShowCities();
    const char* DAT;
    const char* OPT;
    const char* TXT;
    bool datState;
    bool optState;
    bool txtState;
    void createIndex();
    bool findInindex(int x,int y);//输入一个点，在索引中实现其点到属性的检索
    bool cursorPositiontoPoint(int x,int y);//在索引中找到鼠标现在的位置对应查询哪一个点
    CSpCities cityFromindex;//从索引中检索到的城市
};

#endif // CSPFILE_H
