#include <windows.h>

//Defining the ID values of the windows
#define ID_FONT_BUTTON  10
#define ID_COLOR_BUTTON 20

//  Declare Windows procedure
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
//  Make the class name into a global variable
char szClassName[ ] = "Lab_1.WindowsApp";
//  Create a global handle to an instance variable to be used across different functions
HINSTANCE hInst;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    hInst=hThisInstance;
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_HREDRAW | CS_VREDRAW ;
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's white colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
    {
        MessageBox(NULL,"Error: Something went wrong when registering window",szClassName,MB_ICONERROR);
        return 0;
    }


    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Lab_1 Window's program",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           600,                 /* The programs width */
           450,                 /* and height in pixels */
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
{   static HWND hwndFontButton, hwndColorButton, hwndTitleBar,hwndInputBar;
    HDC hdc;
    PAINTSTRUCT ps;
    static RECT rect;
    static int cyChar,cxChar;
    static int jCount;
    switch (message)                  /* handle the messages */
    {   case WM_CREATE:
            //Getting the dimensions of the client area
            GetClientRect(hwnd,&rect);
            //Getting information about the average height and width of a letter
            cyChar = HIWORD(GetDialogBaseUnits());
            cxChar = LOWORD(GetDialogBaseUnits());
            //Creating the windows for the buttons and text boxes
            hwndFontButton=CreateWindowEx((DWORD)NULL,TEXT("button"),
                                          TEXT("FONT"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                                          rect.right-cxChar*40-20,rect.bottom-50,cxChar*40,cyChar*1.5,hwnd,(HMENU)ID_FONT_BUTTON,
                                          hInst,NULL);

            hwndColorButton=CreateWindowEx((DWORD)NULL,TEXT("button"),
                                          TEXT("COLOR"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                                          rect.right-cxChar*15-20,rect.bottom-50-cyChar*1.5,cxChar*15,cyChar*1.5,hwnd,(HMENU)ID_COLOR_BUTTON,
                                          hInst,NULL);

        case WM_SIZE:

            //Setting up the display window size
            GetClientRect(hwnd,&rect);
            rect.left  =20;
            rect.top   =40;
            rect.bottom-=80;
            rect.right -=20;
            //Resetting the count of lines of text on window resizing so that the next output won't be out of bounds
            jCount=0;
            //Getting a handle to the whole window and drawing the rectangle with an extra set of lines around it
            hdc=GetDC(hwnd);
            MoveToEx(hdc,rect.left,rect.top,NULL);
            LineTo(hdc,rect.right,rect.top);
            LineTo(hdc,rect.right,rect.bottom);
            LineTo(hdc,rect.left,rect.bottom);
            LineTo(hdc,rect.left,rect.top);
            Rectangle(hdc,rect.left+1,rect.top+1,rect.right,rect.bottom);
            ReleaseDC(hwnd,hdc);
            //Moving the buttons and other windows according to the resize
            MoveWindow(hwndFontButton,rect.right-cxChar*40,rect.bottom+30,cxChar*40,cyChar*1.5,FALSE);
            MoveWindow(hwndColorButton,rect.right-cxChar*15,rect.bottom+30-cyChar*1.5,cxChar*15,cyChar*1.5,FALSE);
            return 0;

        case WM_PAINT:
            //Getting a handle on the window
            hdc=BeginPaint(hwnd,&ps);
            SetTextColor(hdc,RGB(015,100,105));
            TextOut(hdc,25,rect.top+2+cyChar*jCount++,">>Hello World!",14);
            TextOut(hdc,25,rect.top+2+cyChar*jCount++,">>Hello World!",14);
            TextOut(hdc,25,rect.top+2+cyChar*jCount++,">>Hello World!",14);
            EndPaint(hwnd,&ps);
            return 0;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
