#include <windows.h>
#include <math.h>
#include <stdio.h>

#define ID_SWITCH_SQUARE 301
#define ID_SWITCH_CIRCLE 302
#define ID_SWITCH_BEZIER 303


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
           "Graphics",       /* Title Text */
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
{   PAINTSTRUCT ps;
    RECT rect;
    static BOOL drawing_circle=FALSE,drawing_square=FALSE,drawing_bezier=FALSE;
    static HDC hdc;
    static POINT bez[4];
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            GetClientRect(hwnd,&rect);
            CreateButtons(hwnd,rect);
            break;

        case WM_SIZE:
            GetClientRect(hwnd,&rect);
            MoveWindow(hwndSquareButton,rect.right*3/4,rect.bottom/32,rect.right/8,30,TRUE);
            MoveWindow(hwndCircleButton,rect.right*7/8,rect.bottom/32,rect.right/8,30,TRUE);
            MoveWindow(hwndBezierButton,rect.right*3/4,rect.bottom/8,rect.right/8,30,TRUE);
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

        case WM_MOUSEMOVE:
            break;

        case WM_LBUTTONDOWN:
            GetClientRect(hwnd,&rect);
            hdc=GetDC(hwnd);
            if(LOWORD(lParam)<rect.right*3/4-10 && LOWORD(lParam)>rect.right/4+10) {
                SetPixel(hdc,LOWORD(lParam),HIWORD(lParam),RGB(155,21,12));
            }
            ReleaseDC(hwnd,hdc);
            break;
        case WM_PAINT:
            hdc=BeginPaint(hwnd,&ps);
            GetClientRect(hwnd,&rect);
            //Create the gradients
            CreateGradient(hdc,1,1,rect.right/4+10,rect.bottom);
            CreateGradient(hdc,rect.right*3/4-10,1,rect.right,rect.bottom);
            //Creates the background for drawing
            DrawTheWorkingArea(hdc,rect);
            //Adds the lines to the drawing
            DrawTheLines(hdc,rect);
            //Adds figures to the mix
            DrawGeometry(hdc,rect);
            //Draw the bezier curve
            bez[0].x=1;
            bez[0].y=1;
            bez[1].x=rect.right/8;
            bez[1].y=rect.bottom/6;
            bez[2].x=1;
            bez[2].y=rect.bottom/4;
            bez[3].x=rect.right/8;
            bez[3].y=rect.bottom/4;
            PolyBezier(hdc,bez,4);
            TextOut(hdc,rect.right/16+2,rect.bottom/4,"Bezie Curve",11);
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
    hPen=CreatePen(PS_DOT,1,RGB(15,15,15));
    SelectObject(hdc,hPen);
    Rectangle(hdc,rect.right/4+12,2,rect.right*3/4-12,rect.bottom-2);
    Rectangle(hdc,rect.right/4+14,4,rect.right*3/4-14,rect.bottom-4);
    DeleteObject(hPen);
    SelectObject(hdc,GetStockObject(BLACK_PEN));
    SelectObject(hdc,GetStockObject(NULL_BRUSH));
    Rectangle(hdc,0,0,rect.right/4+10,rect.bottom);
    Rectangle(hdc,rect.right*3/4-10,0,rect.right,rect.bottom);
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
        hBrush=CreateSolidBrush(RGB(100,i,255));
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
                                      TEXT("Square"),WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
                                      rect.right*3/4,rect.bottom/32,rect.right/8,30,hwnd,(HMENU)ID_SWITCH_SQUARE,
                                      hInst,NULL);
    hwndCircleButton = CreateWindowEx((DWORD)NULL,TEXT("button"),
                                      TEXT("Circle"),WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
                                      rect.right*7/8,rect.bottom/32,rect.right/8,30,hwnd,(HMENU)ID_SWITCH_CIRCLE,
                                      hInst,NULL);
    hwndBezierButton = CreateWindowEx((DWORD)NULL,TEXT("button"),
                                      TEXT("Bezier"),WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
                                      rect.right*3/4,rect.bottom/8,rect.right/8,30,hwnd,(HMENU)ID_SWITCH_BEZIER,
                                      hInst,NULL);
}
