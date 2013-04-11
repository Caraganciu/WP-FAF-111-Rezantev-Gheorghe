#include "objects.h"

//The constructor for the objects class instances
Objects::Objects(POINT center,const int &speed) {

    this->center=center;
    this->xSpeed=speed;
    this->ySpeed=speed;
    clr=RGB(0,0,0);

}

//The function what will change the speed of the balls when needed
bool Objects::Accelerate(const int& deltaSpeedX,const int& deltaSpeedY) {
    xSpeed=deltaSpeedX;
    ySpeed=deltaSpeedY;
    return TRUE;
}

//The function that sets the color
bool Objects::Color(const COLORREF &clr) {
    this->clr=clr;
    return TRUE;
}

//The function that redraws the circle in a new position
bool Circle::Move(const HDC &hdc,const RECT& rect,HBRUSH &hBrush) {
    //The selection of the brush
    hBrush=CreateSolidBrush(clr);
    SelectObject(hdc,hBrush);
    //The check for collision
    Collision(rect);
    //The increment of the position
    center.x+=xSpeed; center.y+=ySpeed;
    //The drawing
    Ellipse(hdc,center.x-20,center.y-20,center.x+20,center.y+20);
    //Reseting the brush
    SelectObject(hdc,GetStockObject(WHITE_BRUSH));
    DeleteObject(hBrush);
    return TRUE;

}
//The function that checks for collision  with the wall and changes the direction in case it happened
bool Objects::Collision(const RECT &rect) {

    if(center.x+20>rect.right-1)  {xSpeed=-abs(xSpeed);}
    if(center.x-20<rect.left+1)   {xSpeed=abs(xSpeed);}
    if(center.y+20>rect.bottom-1) {ySpeed=-abs(ySpeed);}
    if(center.y-20<rect.top+1)    {ySpeed=abs(ySpeed);}

    return TRUE;
}

//Function that checks if there was a interaction between 2 balls
bool Interaction(Objects &alfa,Objects &beta) {

    float distance;
    POINT totalSpeed;
    //Checks the distance between the balls
    distance=sqrt( pow(alfa.center.x-beta.center.x,2)+ pow(alfa.center.y-beta.center.y,2) );
    //If the distance is smaller than twice the diameter changes direction of the balls
    if ( distance < 45 ) {
    //Calculates total speed and divides it equaly between the balls
    totalSpeed.x=(abs(alfa.xSpeed)+abs(beta.xSpeed))/2;
    totalSpeed.y=(abs(alfa.ySpeed)+abs(beta.ySpeed))/2;
    //Gives the balls a new direction and speed
    alfa.Accelerate(-alfa.xSpeed*totalSpeed.x/abs(alfa.xSpeed),-alfa.ySpeed*totalSpeed.y/abs(alfa.ySpeed));
    beta.Accelerate(-beta.xSpeed*totalSpeed.x/abs(beta.xSpeed),-beta.ySpeed*totalSpeed.y/abs(beta.ySpeed));
    //Changes the color of one of the balls on interaction
    alfa.Color(RGB(totalSpeed.x*2%255,totalSpeed.x*3%255,totalSpeed.x%255));
    }

    return TRUE;
}
