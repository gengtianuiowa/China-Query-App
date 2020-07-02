#ifndef CSPPOINT_H
#define CSPPOINT_H
#include <iostream>
using namespace std;

class CSpPoint
{
public:
    CSpPoint();
    int X;
    int Y;
    bool IsEnd();
    void ShowPoint();
};

#endif // CSPPOINT_H
