#include "objects.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
static Objects *objects[100];
/*  Make the class name into a global variable  */
char szClassName[ ] = "CodeBlocksWindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

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
           "Animations All Over",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           800,                 /* The programs width */
           600,                 /* and height in pixels */
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
{   static HDC hdc,hdcMem;
    static PAINTSTRUCT ps;
    static RECT rect;
    static HBRUSH hBrush;
    static HBITMAP hbmMem;
    static HANDLE hOld;
    static int timerSpeed=50,numberObjects=0;

    switch (message)                  /* handle the messages */
    {
          case WM_CREATE:
            //Gets device context of the window and the rectangle of the client area
            hdc=GetDC(hwnd);
            GetClientRect(hwnd,&rect);
            //Generate the buffer memory using a bitmap akin memory data
            hdcMem=CreateCompatibleDC(hdc);
            hbmMem=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
            hOld = SelectObject(hdcMem,hbmMem);
            //Sets the initial timer
            SetTimer(hwnd,ID_TIMER,timerSpeed,NULL);
            break;

        case WM_SIZE:
            //Destroy the double buffer memory and handle
            SelectObject(hdcMem,hOld);
            DeleteObject(hbmMem);
            DeleteDC(hdcMem);
            //Gets device context of the window and the rectangle of the client area
            hdc=GetDC(hwnd);
            GetClientRect(hwnd,&rect);
            //Generate the buffer memory using a bitmap akin memory data
            hdcMem=CreateCompatibleDC(hdc);
            hbmMem=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
            hOld = SelectObject(hdcMem,hbmMem);
            break;
        case WM_LBUTTONDOWN:
            //Reads the center of the circle from the mouse click data
            POINT newCenter;
            newCenter.x=LOWORD(lParam);
            newCenter.y=HIWORD(lParam);
            //Creates a new circle based on the mouse position data
            objects[numberObjects]=new Circle(newCenter,5+newCenter.x%5);
            objects[numberObjects]->Color(RGB(newCenter.x%255,newCenter.x%125+newCenter.y%125,newCenter.y%255));
            //Increments number of objects
            numberObjects++;
            break;


        case WM_MOUSEWHEEL:
            //Checks if the mousewheel was scrolled down and decreases the speed and increases otherwise
            if((short)HIWORD(wParam)<0) {
                timerSpeed+=10;
            }else {
                timerSpeed-=10;
            //Checks to see that the delay for the timer isn't negative
                if (timerSpeed<0) timerSpeed=1;
            }
            //Resets the timer
            KillTimer(hwnd,ID_TIMER);
            SetTimer(hwnd,ID_TIMER,timerSpeed,NULL);
            break;

        case WM_PAINT:
            //Gets the device contexd handle and the rectangle area of the client
            hdc=BeginPaint(hwnd,&ps);
            GetClientRect(hwnd,&rect);
            //Checks for interaction between all the objects in the array
            for(int i=0;i<numberObjects-1;i++) {
                for(int j=i+1;j<numberObjects;j++) {
                    Interaction(*objects[i],*objects[j]);
                }
            }
            //Fills the buffer memory background image with white
            FillRect(hdcMem,&rect,(HBRUSH)GetStockObject(WHITE_BRUSH));
            //Redraws all the objects in the array
            for(int i=0;i<numberObjects;i++) {
                objects[i]->Move(hdcMem,rect,hBrush);
            }
            //Switches the loaded memory buffer with the display context
            BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);

            EndPaint(hwnd,&ps);
            break;

        case WM_TIMER:
            //Invalidates rect area without calling the erase background message
            InvalidateRect(hwnd,NULL,FALSE);
            break;



        case WM_DESTROY:
            //Destroy the double buffer memory and handle
            SelectObject(hdcMem,hOld);
            DeleteObject(hbmMem);
            DeleteDC(hdcMem);
            //Kill the timer
            KillTimer(hwnd,ID_TIMER);

            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
