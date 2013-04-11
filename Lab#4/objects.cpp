#include "objects.h"

Objects::Objects(POINT center,const int &speed) {
    this->center=center;
    this->xSpeed=speed;
    this->ySpeed=speed;
    clr=RGB(0,0,0);
}

bool Objects::Accelerate(const int& deltaSpeedX,const int& deltaSpeedY) {
    xSpeed=deltaSpeedX;
    ySpeed=deltaSpeedY;
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
    center.x+=xSpeed; center.y+=ySpeed;
    Ellipse(hdc,center.x-30,center.y-30,center.x+30,center.y+30);

    SelectObject(hdc,GetStockObject(WHITE_BRUSH));
    DeleteObject(hBrush);
    return TRUE;

}

bool Objects::Collision(const RECT &rect) {

    if(center.x+30>rect.right-1)  {xSpeed=-abs(xSpeed);}
    if(center.x-30<rect.left+1)   {xSpeed=abs(xSpeed);}
    if(center.y+30>rect.bottom-1) {ySpeed=-abs(ySpeed);}
    if(center.y-30<rect.top+1)    {ySpeed=abs(ySpeed);}

    return TRUE;
}

bool Interaction(Objects &alfa,Objects &beta) {
    float distance;
    float angle;

    distance=sqrt( pow(alfa.center.x-beta.center.x,2)+ pow(alfa.center.y-beta.center.y,2) );

    if ( distance-1 < 60) {
        angle=(beta.center.y-alfa.center.y)/(beta.center.x-alfa.center.x+0.0001);
    {
    totalSpeed=(alfa.xSpeed/cos(arctg(angle))+beta.xSpeed/cos(arctg(angle)))/2;
    alfa.Accelerate(-alfa.xSpeed*totalSpeed*cos(arctg(angle))/afla.xSpeed,-alfa.ySpeed*totalSpeed*sin(arctg(angle))/alfa.ySpeed);
    beta.Accelerate(-beta.xSpeed*totalSpeed*cos(arctg(angle))/beta.xSpeed,-beta.ySpeed*totalSpeed*sin(arctg(angle))/beta.ySpeed);
    }
    return TRUE;
}
}
