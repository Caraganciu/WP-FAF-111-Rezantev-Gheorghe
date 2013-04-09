#include "objects.h"

Objects::Objects(POINT vertixes[2],const float &angle,const int &speed) {
    this->vertixes=vertixes;
    this->angle=angle;
    this->xSpeed=speed;
    this->ySpeed=speed;
}

bool Objects::Accelerate(const int& deltaSpeedX,const int& deltaSpeedY) {
    xSpeed+=deltaSpeedX;
    ySpeed+=deltaSpeedY;
    return TRUE;
}

bool Objects::Color(const COLORREF &clr) {
    this->clr=clr;
    return TRUE;
}

bool Circle::Move(const HDC &hdc,const RECT& rect,HBRUSH &hBrush) {

    hBrush=CreateSolidBrush(clr);
    SelectObject(hdc,hBrush);
    Collision(rect);
    Ellipse(hdc,vertixes[0].x+xSpeed,vertixes[0].y+ySpeed,vertixes[1].x+xSpeed,vertixes[1].y+ySpeed);
    SelectObject(hdc,GetStockObject(WHITE_BRUSH));
    DeleteObject(hBrush);
    return TRUE;

}

bool Objects::Collision(const RECT &rect) {

    if(vertixes[1].x>rect.right)  {xSpeed=-xSpeed;}
    if(vertixes[0].x<rect.left)   {xSpeed=-xSpeed;}
    if(vertixes[1].y>rect.bottom) {ySpeed=-ySpeed;}
    if(vertixes[0].y<rect.top)    {ySpeed=-ySpeed;}

    return TRUE;
}

bool Objects::Interaction(const Objects &alfa,const Objects &beta) {

}
