#ifndef _Objects.h_
#define _Objects.h_
#include <iostream>
#include <windows.h>
#include <cmath>

#define ID_TIMER 333
using namespace std;


class Objects {

protected:


    COLORREF clr;
public:
    POINT *vertixes;
    int xSpeed,ySpeed;
    Objects(POINT vertixes[2],const int &speed);
    bool Accelerate(const int& deltaSpeedX,const int& deltaSpeedY);
    bool Color(const COLORREF &clr);
    bool Collision(const RECT &rect);
    virtual bool Move(const HDC &hdc,const RECT& rect,HBRUSH &hBrush)=0;
};

class Circle : public Objects {

public:

    Circle(POINT vertixes[2],const int &speed): Objects(vertixes,speed)
    {
        cout<<"creation success";
    }

    bool Move(const HDC &hdc,const RECT& rect,HBRUSH &hBrush);
};

bool Interaction(Objects &alfa,Objects &beta);
#endif // _Objects
