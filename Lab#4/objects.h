#ifndef _Objects.h_
#define _Objects.h_
#include <iostream>
#include <windows.h>
using namespace std;

class Objects {

protected:
    POINT *vertixes;
    float angle;
    int xSpeed,ySpeed;
    COLORREF clr;
public:
    Objects(POINT vertixes[2],const float &angle,const int &speed);
    bool Accelerate(const int& deltaSpeedX,const int& deltaSpeedY);
    bool Color(const COLORREF &clr);
    bool Collision(const RECT &rect);
    virtual bool Move()=0;
};

class Circle : private Objects {

public:

    Circle(POINT vertixes[2],const float &angle,const int &speed): Objects(vertixes,angle,speed)
    {
        cout<<"creation success";
    }

    bool Move(const HDC &hdc,const RECT& rect,HBRUSH &hBrush);
};

#endif // _Objects
