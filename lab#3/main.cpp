#include <windows.h>
#include <math.h>
#include <stdio.h>

#include "resources.h"


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void DrawTheLines(const HDC &,const RECT&);
void DrawTheWorkingArea(const HDC& ,const RECT& );
void DrawGeometry(const HDC& ,const RECT& );
void VertexSetup(const RECT&);
void CreateGradient(const HDC &,const int ,const int ,const int ,const int );
void CreateButtons(const HWND& ,const RECT&);

/*  Make the class name into a global variable  */
HINSTANCE hInst;
HWND hwndSquareButton,hwndCircleButton,hwndBezierButton;
char szClassName[ ] = "Lab#3";

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    hInst=hThisInstance;
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Graphics",          /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           800,                 /* The programs width */
           500,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    static RECT rect,oldRect;
    static BOOL drawing_circle=FALSE,drawing_square=FALSE,drawing_bezier=TRUE,        //Checks what is being drawn at the moment
    first_point=TRUE,                                                                 //Checks if the first point of the bezier is being drawn
    button_pressed_in_area=FALSE;                                                     //Checks if the button was pressed and if it happened inside the drawing area

    static HDC hdc,
    hdcMem;
    static HBITMAP hBitmap;
    BITMAP bitmap;

    static int figureCount=0;                                                         //Counts the number of figures in the drawing area
    static POINT arrayPoints[100][5];                                                 //The array that holds the data of the figures

    static float xDisp,yDisp;

    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:

            GetClientRect(hwnd,&rect);
            oldRect=rect;
            CreateButtons(hwnd,rect);
            hBitmap = LoadBitmap(hInst,MAKEINTRESOURCE(LOGO));
            GetObject(hBitmap,sizeof(BITMAP),&bitmap);
            break;

        case WM_SIZE:

            GetClientRect(hwnd,&rect);
            //Resizes all buttons based on the window size
            MoveWindow(hwndSquareButton,rect.right*3/4,rect.bottom/32,rect.right/8,30,TRUE);
            MoveWindow(hwndCircleButton,rect.right*7/8,rect.bottom/32,rect.right/8,30,TRUE);
            MoveWindow(hwndBezierButton,rect.right*3/4,rect.bottom/8,rect.right/8,30,TRUE);
            //Set the displacement of the drawings inside the window to new , moved coordinates
            xDisp=(float)rect.right/oldRect.right;
            yDisp=(float)rect.bottom/oldRect.bottom;
            oldRect.right=rect.right;
            oldRect.bottom=rect.bottom;
            for(int i=0;i<figureCount;i++) {
                for(int j=0;j<4;j++) {
                    arrayPoints[i][j].x*=xDisp;
                    arrayPoints[i][j].y*=yDisp;
                }
            }

            InvalidateRect(hwnd,NULL,TRUE);
            break;

        case WM_COMMAND:

            switch(wParam)
            {
                case ID_SWITCH_CIRCLE:
                SendMessage(hwndCircleButton,BM_SETCHECK,1,0);
                SendMessage(hwndBezierButton,BM_SETCHECK,0,0);
                SendMessage(hwndSquareButton,BM_SETCHECK,0,0);
                drawing_bezier=drawing_square=FALSE;
                drawing_circle=TRUE;
                break;

                case ID_SWITCH_SQUARE:
                SendMessage(hwndCircleButton,BM_SETCHECK,0,0);
                SendMessage(hwndBezierButton,BM_SETCHECK,0,0);
                SendMessage(hwndSquareButton,BM_SETCHECK,1,0);
                drawing_bezier=drawing_circle=FALSE;
                drawing_square=TRUE;
                break;

                case ID_SWITCH_BEZIER:
                SendMessage(hwndCircleButton,BM_SETCHECK,0,0);
                SendMessage(hwndBezierButton,BM_SETCHECK,1,0);
                SendMessage(hwndSquareButton,BM_SETCHECK,0,0);
                drawing_square=drawing_circle=FALSE;
                drawing_bezier=TRUE;
                break;

            }
            break;

        case WM_RBUTTONDOWN:

            figureCount--;
            if(figureCount<0) figureCount=0;
            InvalidateRect(hwnd,NULL,TRUE);

            break;

        case WM_MOUSEMOVE:

            if(!button_pressed_in_area) break;

            //Checks if the moving is being done in the drawing area
            GetClientRect(hwnd,&rect);
            if ((LOWORD(lParam)<rect.right/4+14) || (LOWORD(lParam)>rect.right*3/4-14) ||
                (HIWORD(lParam)<125) || (HIWORD(lParam)>rect.bottom-6)) break;


            if (drawing_square) {
            hdc=GetDC(hwnd);
            SelectObject(hdc,GetStockObject(NULL_BRUSH));
            Rectangle(hdc,arrayPoints[figureCount][0].x,arrayPoints[figureCount][0].y,LOWORD(lParam),HIWORD(lParam));
            ReleaseDC(hwnd,hdc);
            }

            break;

        case WM_LBUTTONDOWN:

            //Checks if the button clicking is being done in the drawing area
            GetClientRect(hwnd,&rect);
            if ((LOWORD(lParam)<rect.right/4+14) || (LOWORD(lParam)>rect.right*3/4-14) ||
                (HIWORD(lParam)<125) || (HIWORD(lParam)>rect.bottom-6)) break;

            button_pressed_in_area=TRUE;
            hdc=GetDC(hwnd);

            if (drawing_square) {
                arrayPoints[figureCount][4].x=ID_SWITCH_SQUARE;
                arrayPoints[figureCount][0].x=LOWORD(lParam);
                arrayPoints[figureCount][0].y=HIWORD(lParam);
            }

            if (drawing_bezier && first_point) {
                arrayPoints[figureCount][4].x=ID_SWITCH_BEZIER;
                arrayPoints[figureCount][0].x=LOWORD(lParam);
                arrayPoints[figureCount][0].y=HIWORD(lParam);
                first_point=FALSE;
            } else if (drawing_bezier) {
                first_point=TRUE;
                figureCount--;
                arrayPoints[figureCount][2].x=LOWORD(lParam);
                arrayPoints[figureCount][2].y=HIWORD(lParam);
            }

            if (drawing_circle) {
                arrayPoints[figureCount][4].x=ID_SWITCH_CIRCLE;
                arrayPoints[figureCount][0].x=LOWORD(lParam);
                arrayPoints[figureCount][0].y=HIWORD(lParam);
            }
            ReleaseDC(hwnd,hdc);
            break;

        case WM_LBUTTONUP:
            if(!button_pressed_in_area) break;
            //Check if drawing in the needed area
            GetClientRect(hwnd,&rect);
            if ((LOWORD(lParam)<rect.right/4+14) || (LOWORD(lParam)>rect.right*3/4-14) ||
                (HIWORD(lParam)<125) || (HIWORD(lParam)>rect.bottom-6)) break;



            hdc=GetDC(hwnd);

            if (drawing_square) {
                arrayPoints[figureCount][1].x=LOWORD(lParam);
                arrayPoints[figureCount][1].y=HIWORD(lParam);
            }

            if (drawing_bezier && !first_point) {
                arrayPoints[figureCount][1].x=LOWORD(lParam);
                arrayPoints[figureCount][1].y=HIWORD(lParam);
                MoveToEx(hdc,arrayPoints[figureCount][0].x,arrayPoints[figureCount][0].y,NULL);
                LineTo(hdc,arrayPoints[figureCount][1].x,arrayPoints[figureCount][1].y);
            } else if (drawing_bezier) {
                arrayPoints[figureCount][3].x=LOWORD(lParam);
                arrayPoints[figureCount][3].y=HIWORD(lParam);
                MoveToEx(hdc,arrayPoints[figureCount][2].x,arrayPoints[figureCount][2].y,NULL);
                LineTo(hdc,arrayPoints[figureCount][3].x,arrayPoints[figureCount][3].y);
                InvalidateRect(hwnd,NULL,TRUE);
            }

            if (drawing_circle) {
                arrayPoints[figureCount][1].x=LOWORD(lParam);
                arrayPoints[figureCount][1].y=HIWORD(lParam);
            }
            figureCount++;
            ReleaseDC(hwnd,hdc);
            if (!drawing_bezier) {
            InvalidateRect(hwnd,NULL,TRUE);
            }

            button_pressed_in_area=FALSE;
            break;

        case WM_PAINT:
            hdc=BeginPaint(hwnd,&ps);
            GetClientRect(hwnd,&rect);
            //Create figures
            SelectObject(hdc,GetStockObject(NULL_BRUSH));
            for (int i=0;i<figureCount;i++) {
                switch(arrayPoints[i][4].x){
                case ID_SWITCH_SQUARE:
                Rectangle(hdc,arrayPoints[i][0].x,arrayPoints[i][0].y,arrayPoints[i][1].x,arrayPoints[i][1].y);
                break;
                case ID_SWITCH_CIRCLE:
                Ellipse(hdc,arrayPoints[i][0].x,arrayPoints[i][0].y,arrayPoints[i][1].x,arrayPoints[i][1].y);
                break;
                case ID_SWITCH_BEZIER:
                PolyBezier(hdc,arrayPoints[i],4);
                break;
                }
            }
            SelectObject(hdc,GetStockObject(WHITE_BRUSH));
            //Create the gradients
            CreateGradient(hdc,1,1,rect.right/4+10,rect.bottom);
            CreateGradient(hdc,rect.right*3/4-10,1,rect.right,rect.bottom);
            //Creates the background for drawing
            DrawTheWorkingArea(hdc,rect);
            //Adds the lines to the drawing
            DrawTheLines(hdc,rect);
            //Adds figures to the mix
            DrawGeometry(hdc,rect);
            //Make Logo
            hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem,hBitmap);
            BitBlt(hdc,rect.right/4+16,6,185,120,hdcMem,0,0,SRCCOPY);
            DeleteDC(hdcMem);
            EndPaint(hwnd,&ps);
            break;

        case WM_CTLCOLORSTATIC:
            SetBkMode((HDC)wParam,TRANSPARENT);
            return (LRESULT)GetStockObject(NULL_BRUSH);

        case WM_GETMINMAXINFO:
            //Setting the minimal size for the window
            MINMAXINFO *ptMinMax;
            ptMinMax=(MINMAXINFO*)lParam;

            ptMinMax->ptMinTrackSize.x=450;
            ptMinMax->ptMinTrackSize.y=300;
            break;

        case WM_DESTROY:

            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}



//The function that will draw the basic static lines .
void
DrawTheLines(const HDC & hdc,const RECT& rect) {
    HPEN hPen;
    SetBkMode(hdc,TRANSPARENT);
    //Creating the pen and drawing the line . If the line style isn't solid a weight bigger than 1 will force it to solid
    hPen = CreatePen(PS_DASHDOT,0,RGB(5,5,20));
    SelectObject(hdc,hPen);
    MoveToEx(hdc,10,rect.bottom*15/16,NULL);
    LineTo(hdc,rect.right/4,rect.bottom*15/16);
    //Deleting the pen
    DeleteObject(hPen);

    hPen = CreatePen(PS_DASH,1,RGB(255,0,200));
    SelectObject(hdc,hPen);
    MoveToEx(hdc,10,rect.bottom*14/16,NULL);
    LineTo(hdc,rect.right/4,rect.bottom*14/16);
    DeleteObject(hPen);

    hPen = CreatePen(PS_DOT,0,RGB(0,0,255));
    SelectObject(hdc,hPen);
    MoveToEx(hdc,10,rect.bottom*13/16,NULL);
    LineTo(hdc,rect.right/4,rect.bottom*13/16);
    DeleteObject(hPen);

    hPen = CreatePen(PS_DASHDOTDOT,0,RGB(200,0,100));
    SelectObject(hdc,hPen);
    MoveToEx(hdc,10,rect.bottom*12/16,NULL);
    LineTo(hdc,rect.right/4,rect.bottom*12/16);
    DeleteObject(hPen);

    hPen = CreatePen(PS_SOLID,4,RGB(5,255,20));
    SelectObject(hdc,hPen);
    MoveToEx(hdc,10,rect.bottom*11/16,NULL);
    LineTo(hdc,rect.right/4,rect.bottom*11/16);
    DeleteObject(hPen);

    SelectObject(hdc,GetStockObject(BLACK_PEN));

}


void
DrawTheWorkingArea(const HDC& hdc,const RECT& rect) {
    HPEN hPen;
    hPen=CreatePen(PS_SOLID,5,RGB(255,255,255));
    SelectObject(hdc,hPen);
    SelectObject(hdc,GetStockObject(NULL_BRUSH));
    Rectangle(hdc,rect.right/4+11,125,rect.right*3/4-11,rect.bottom);
    DeleteObject(hPen);

    hPen=CreatePen(PS_DASHDOTDOT,1,RGB(15,15,15));
    SelectObject(hdc,hPen);
    Rectangle(hdc,rect.right/4+12,126,rect.right*3/4-12,rect.bottom-2);
    Rectangle(hdc,rect.right/4+14,128,rect.right*3/4-14,rect.bottom-4);
    DeleteObject(hPen);

    SelectObject(hdc,GetStockObject(NULL_PEN));
    SelectObject(hdc,GetStockObject(WHITE_BRUSH));
    Rectangle(hdc,rect.right/4+11,0,rect.right*3/4-11,124);


}

void
DrawGeometry(const HDC& hdc,const RECT& rect) {
    HPEN hPen;
    HBRUSH hBrush;

    hPen=CreatePen(PS_SOLID,2,RGB(30,30,30));
    SelectObject(hdc,hPen);
    hBrush=CreateSolidBrush(RGB(0,180,180));
    SelectObject(hdc,hBrush);
    RoundRect(hdc,rect.right*15/16-10,rect.bottom*7/8,rect.right*15/16+rect.right*1/16-10,rect.bottom*7/8+rect.right*1/16,15,15);
    DeleteObject(hPen);
    DeleteObject(hBrush);

    hPen=CreatePen(PS_DASH,1,RGB(255,0,0));
    SelectObject(hdc,hPen);
    hBrush=CreateSolidBrush(RGB(130,0,130));
    SelectObject(hdc,hBrush);
    Ellipse(hdc,rect.right*3/4,rect.bottom*7/8,rect.right*3/4+rect.right*1/16,rect.bottom*7/8+rect.right*1/16);
    DeleteObject(hPen);
    DeleteObject(hBrush);

    hPen=(HPEN)GetStockObject(NULL_PEN);
    SelectObject(hdc,hPen);
    hBrush=CreateSolidBrush(RGB(80,80,0));
    SelectObject(hdc,hBrush);
    Chord(hdc,rect.right*3/4,rect.bottom*3/4,rect.right*3/4+rect.right*1/16,rect.bottom*3/4+rect.right*1/16,5000,1500,100,155);
    DeleteObject(hPen);
    DeleteObject(hBrush);

    hPen=CreatePen(PS_DOT,1,RGB(255,255,255));
    SelectObject(hdc,hPen);
    hBrush=CreateSolidBrush(RGB(30,30,30));
    SelectObject(hdc,hBrush);
    Pie(hdc,rect.right*15/16-10,rect.bottom*3/4,rect.right*15/16+rect.right*1/16-10,rect.bottom*3/4+rect.right*1/16,10,500,100,15);
    DeleteObject(hPen);
    DeleteObject(hBrush);

    SelectObject(hdc,GetStockObject(BLACK_PEN));
    SelectObject(hdc,GetStockObject(NULL_BRUSH));
}

void
CreateGradient(const HDC & hdc,const int xInit,const int yInit,const int xFin,const int yFin) {
    HBRUSH hBrush;
    for(int i=0;i<256;i++){
        hBrush=CreateSolidBrush(RGB(i,i,255));
        SelectObject(hdc,hBrush);
        SelectObject(hdc,GetStockObject(NULL_PEN));
        Rectangle(hdc,xInit,yInit,xFin,yFin-i*(yFin)/255);
        DeleteObject(hBrush);

    }
    SelectObject(hdc,GetStockObject(NULL_BRUSH));
    SelectObject(hdc,GetStockObject(BLACK_PEN));
}

void
CreateButtons(const HWND& hwnd,const RECT& rect){
    hwndSquareButton = CreateWindowEx((DWORD)NULL,TEXT("button"),
                                      TEXT("Rectangle"),WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
                                      rect.right*3/4,rect.bottom/32,rect.right/8,30,hwnd,(HMENU)ID_SWITCH_SQUARE,
                                      hInst,NULL);
    hwndCircleButton = CreateWindowEx((DWORD)NULL,TEXT("button"),
                                      TEXT("Ellipse"),WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
                                      rect.right*7/8,rect.bottom/32,rect.right/8,30,hwnd,(HMENU)ID_SWITCH_CIRCLE,
                                      hInst,NULL);
    hwndBezierButton = CreateWindowEx((DWORD)NULL,TEXT("button"),
                                      TEXT("Bezier"),WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
                                      rect.right*3/4,rect.bottom/8,rect.right/8,30,hwnd,(HMENU)ID_SWITCH_BEZIER,
                                      hInst,NULL);
    SendMessage(hwndBezierButton,BM_SETCHECK,1,0);
}
