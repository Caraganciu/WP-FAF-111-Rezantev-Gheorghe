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
           544,                 /* The programs width */
           375,                 /* and height in pixels */
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
{   static HDC hdc;
    static PAINTSTRUCT ps;
    static RECT rect;
    static HBRUSH hBrush;
    switch (message)                  /* handle the messages */
    {
          case WM_CREATE:

            static POINT x[2];
            x[0].y=50;
            x[0].x=50;
            x[1].y=80;
            x[1].x=80;
            objects[0]=new Circle(x,15);
            static POINT y[2];
            y[0].y=100;
            y[0].x=100;
            y[1].y=130;
            y[1].x=130;
            static Circle beta(y,10);
            beta.Color(RGB(200,15,200));
            static POINT z[2];
            z[0].y=300;
            z[0].x=300;
            z[1].y=330;
            z[1].x=330;
            static Circle gama(z,10);
            gama.Color(RGB(100,15,200));
            SetTimer(hwnd,ID_TIMER,100,NULL);
            break;

        case WM_PAINT:

            hdc=BeginPaint(hwnd,&ps);
            GetClientRect(hwnd,&rect);
            Interaction(*objects[0],beta);
            Interaction(*objects[0],gama);
            Interaction(gama,beta);
            objects[0]->Move(hdc,rect,hBrush);
            beta.Move(hdc,rect,hBrush);
            gama.Move(hdc,rect,hBrush);
            EndPaint(hwnd,&ps);

            break;

        case WM_TIMER:
            InvalidateRect(hwnd,NULL,TRUE);
            break;
        case WM_DESTROY:
            KillTimer(hwnd,ID_TIMER);
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
