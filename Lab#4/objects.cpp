#include "objects.h"

Objects::Objects(POINT vertixes[2],const int &speed) {
    this->vertixes=vertixes;
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
    Ellipse(hdc,vertixes[0].x+=xSpeed,vertixes[0].y+=ySpeed,vertixes[1].x+=xSpeed,vertixes[1].y+=ySpeed);

    SelectObject(hdc,GetStockObject(WHITE_BRUSH));
    DeleteObject(hBrush);
    return TRUE;

}

bool Objects::Collision(const RECT &rect) {

    if(vertixes[1].x>rect.right-10)  {xSpeed=-abs(xSpeed);}
    if(vertixes[0].x<rect.left+10)   {xSpeed=abs(xSpeed);}
    if(vertixes[1].y>rect.bottom-10) {ySpeed=-abs(ySpeed);}
    if(vertixes[0].y<rect.top+10)    {ySpeed=abs(ySpeed);}

    return TRUE;
}

bool Interaction(Objects &alfa,Objects &beta) {
    float distance;
    float angle;
    POINT centerAlfa;
    POINT centerBeta;

    centerAlfa.x=alfa.vertixes[0].x+(alfa.vertixes[1].x-alfa.vertixes[0].x)/2;
    centerAlfa.y=alfa.vertixes[0].y+(alfa.vertixes[1].y-alfa.vertixes[0].y)/2;

    centerBeta.x=beta.vertixes[0].x+(beta.vertixes[1].x-beta.vertixes[0].x)/2;
    centerBeta.y=beta.vertixes[0].y+(beta.vertixes[1].y-beta.vertixes[0].y)/2;

    distance=sqrt( pow(centerAlfa.x-centerBeta.x,2)+ pow(centerAlfa.y-centerBeta.y,2) );

    if ( distance-1 < (alfa.vertixes[1].x-alfa.vertixes[0].x+beta.vertixes[1].x-beta.vertixes[0].x)/2.0 ) {
        angle=(centerBeta.y-centerAlfa.y)/(centerBeta.x-centerAlfa.x+0.0001);
    {

    alfa.Accelerate(-2,-5);
    beta.Accelerate(2,5);
    }
    return TRUE;
}
}
