#include <windows.h>
#include <stdio.h>
#include <tchar.h>


//Defining the ID values of the windows
#define ID_FONT_BUTTON  10
#define ID_COLOR_BUTTON 20
#define ID_LIST_BOX     30
#define ID_TEXT_INPUT   40
#define ID_INPUT_BUTTON 50
#define ID_CLEAR_BUTTON 60
#define TIMES_NEW_ROMAN       100
#define ARIAL                 200
#define CALIBRI               300

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
    wincl.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(136,152,213));

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
{
    static HWND hwndFontButton, hwndColorButton,hwndListBox,hwndTextInput,hwndInputButton,hwndClearButton;  //all the handles to different controls
    HDC hdc;                                                                                                //device context handle
    HFONT textFont,hFontOld;                                                                                //Font handles
    static char colorBit=2;                                                                                 //used to set colors of text on button press
    PAINTSTRUCT ps;
    static RECT rect,resizeRect;
    static char* inputText=new char[200];                                                                   //Text buffer
    static int cyChar,cxChar,textSize;                                                                      //characters ave width ,height and size of string
    static int color_button_pressed,font_button_pressed,textHeight,fontNumber=100;                          //Button press flags,fontNumber for circling fonts
    HBRUSH color;
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
                                          rect.right-cxChar*40-20,rect.bottom-50,cxChar*30,cyChar*1.5,hwnd,(HMENU)ID_FONT_BUTTON,
                                          hInst,NULL);

            hwndClearButton=CreateWindowEx((DWORD)NULL,TEXT("button"),
                                          TEXT("C"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                                          rect.left+21+cxChar*30+5,rect.bottom-49,cxChar*5,cyChar*1.5,hwnd,(HMENU)ID_CLEAR_BUTTON,
                                          hInst,NULL);

            hwndInputButton=CreateWindowEx((DWORD)NULL,TEXT("button"),
                                          TEXT("Press to input"),WS_CHILD|WS_VISIBLE,
                                          rect.left+21,rect.bottom-49,cxChar*30,cyChar*1.5,hwnd,(HMENU)ID_INPUT_BUTTON,
                                          hInst,NULL);

            hwndColorButton=CreateWindowEx((DWORD)NULL,TEXT("button"),
                                          TEXT("COLOR"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                                          rect.right-cxChar*15-20,rect.bottom-50-cyChar*1.5,cxChar*15,cyChar*1.5,hwnd,(HMENU)ID_COLOR_BUTTON,
                                          hInst,NULL);

            hwndListBox=CreateWindowEx((DWORD)NULL,TEXT("listbox"),
                                          TEXT("LISTBOX"),WS_VISIBLE | WS_CHILD | WS_VSCROLL |LBS_NOINTEGRALHEIGHT |ES_READONLY |WS_HSCROLL,
                                          rect.left+21,rect.top+41,rect.right-rect.left-41,rect.bottom-rect.top-120,hwnd,(HMENU)ID_LIST_BOX,
                                          hInst,NULL);

            hwndTextInput=CreateWindowEx((DWORD)NULL,"EDIT",
                                          TEXT(""),WS_VISIBLE | WS_CHILD | WS_BORDER |LBS_NOINTEGRALHEIGHT ,
                                          0,0,0,0,hwnd,(HMENU)ID_TEXT_INPUT,
                                          hInst,NULL);
            //Default message to list box
            SendDlgItemMessage(hwnd, ID_LIST_BOX, LB_ADDSTRING, 0, (LPARAM)"Hi there!");
            break;

        case WM_SIZE:

            //Setting up the display window size
            GetClientRect(hwnd,&rect);
            rect.left  =20;
            rect.top   =40;
            rect.bottom-=80;
            rect.right -=20;
            //Moving the buttons and other windows according to the resize
            MoveWindow(hwndFontButton,rect.right-cxChar*30,rect.bottom+31,cxChar*30,cyChar*1.5,FALSE);

            MoveWindow(hwndClearButton,rect.left+cxChar*30+6,rect.bottom+31,cxChar*5,cyChar*1.5,FALSE);

            MoveWindow(hwndInputButton,rect.left,rect.bottom+31,cxChar*30,cyChar*1.5,FALSE);

            MoveWindow(hwndColorButton,rect.right-cxChar*15,rect.bottom+30-cyChar*1.5,cxChar*15,cyChar*1.5,FALSE);

            MoveWindow(hwndListBox,rect.left+1,1+rect.top,rect.right-rect.left-1,rect.bottom-rect.top-1,FALSE);

            MoveWindow(hwndTextInput,rect.left,rect.bottom+2,rect.right-rect.left-cxChar*16,cyChar*1.5,FALSE);

            //Getting a handle to the whole window and drawing the rectangle around the list box
            hdc=GetDC(hwnd);
            MoveToEx(hdc,rect.left,rect.top,NULL);
            LineTo(hdc,rect.right,rect.top);
            LineTo(hdc,rect.right,rect.bottom);
            LineTo(hdc,rect.left,rect.bottom);
            LineTo(hdc,rect.left,rect.top);
            ReleaseDC(hwnd,hdc);

            return 0;

        case WM_GETMINMAXINFO:
            //Setting the minimal size for the window
            MINMAXINFO *ptMinMax;
            ptMinMax=(MINMAXINFO*)lParam;

            ptMinMax->ptMinTrackSize.x=600;
            ptMinMax->ptMinTrackSize.y=200;
            break;


        case WM_COMMAND:

            //Handling button presses
            switch(LOWORD(wParam))
            {
                case ID_CLEAR_BUTTON:
                    //Clearing list box
                    SendMessage(hwndListBox,LB_RESETCONTENT,0,0);
                    break;
                case ID_FONT_BUTTON:
                    //Incrementing font number in modular arithmetic
                    fontNumber=((fontNumber/100)%3)*100+100;
                    font_button_pressed=1;
                    RedrawWindow(hwndListBox,NULL,NULL,RDW_INVALIDATE);
                    break;
                case ID_COLOR_BUTTON:
                    //Incrementing color number in modular arithmetic
                    if(colorBit) colorBit=(colorBit%8)*2;
                    else colorBit=2;
                    color_button_pressed=1;
                    RedrawWindow(hwndListBox,NULL,NULL,RDW_INVALIDATE);
                    break;
                case ID_INPUT_BUTTON:
                    {   //sending the message from the edit box to the list box
                        textSize=SendMessage(hwndTextInput,WM_GETTEXT,200,(LONG)inputText);
                        inputText[textSize]=_T('\0');
                        SendDlgItemMessage(hwnd, ID_LIST_BOX, LB_ADDSTRING, 0, (LPARAM)inputText);
                        SendMessage(hwndTextInput,WM_SETTEXT,NULL,(LPARAM)"");

                    }
                default: break;
            }
            return 0;

        case WM_CTLCOLORLISTBOX:
            color=CreateSolidBrush(RGB(255,255,255));
            hdc=(HDC)wParam;
            SetTextColor(hdc,RGB(((colorBit>>3)&0x01)*255,((colorBit>>2)&0x01)*255,((colorBit>>1)&0x01)*255));
            if(GetDlgCtrlID((HWND)lParam)==ID_LIST_BOX&&color_button_pressed)
            {
                color_button_pressed=0;
            }

            if(GetDlgCtrlID((HWND)lParam)==ID_LIST_BOX&&font_button_pressed)
            {
                textHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);
                switch(fontNumber)
                {
                    case TIMES_NEW_ROMAN:
                        textFont = CreateFont(textHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
                        break;
                    case ARIAL:
                        textFont = CreateFont(textHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Arial");
                        break;
                    case CALIBRI:
                        textFont = CreateFont(textHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Calibri");
                        break;
                }
                if(textFont)
                {
                    SendMessage(hwndListBox,WM_SETFONT,WPARAM(textFont),TRUE);
                }
                font_button_pressed=0;
            }
            return (LONG)color;

        case WM_CTLCOLOREDIT:
            //Setting custom color for the edit box
             if(ID_TEXT_INPUT==GetDlgCtrlID((HWND)lParam))
             {
                hdc=(HDC)wParam;
                color = CreateSolidBrush(RGB(25, 50, 75));
                SetTextColor(hdc, RGB(250, 150, 150));  //setting text color
                SetBkMode(hdc, TRANSPARENT);            //setting background to transp
                SetBkColor(hdc,(LONG)color);            //setting background color
                return (LONG)color;
             }

        case WM_PAINT:

            //Getting a handle on the window
            hdc=BeginPaint(hwnd,&ps);
            GetClientRect(hwnd,&rect);
            //Set the font,color and background for the title of the window
            textHeight= -MulDiv(20, GetDeviceCaps(hdc, LOGPIXELSY), 72);
            textFont  = CreateFont(textHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
            hFontOld  = (HFONT)SelectObject(hdc,textFont);
            SetTextColor(hdc,RGB(255,125,63));
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc,"EDIT~ME",7,&rect,DT_TOP|DT_CENTER);
            //Destroy font object
            DeleteObject(SelectObject(hdc,hFontOld));
            //Set the font and color for the normal text
            textHeight= -MulDiv(10, GetDeviceCaps(hdc, LOGPIXELSY), 72);
            textFont  = CreateFont(textHeight, 0, 0, 0, 0, 0,TRUE,0, 0, 0, 0, 0, 0, NULL);
            hFontOld  = (HFONT)SelectObject(hdc,textFont);
            SetTextColor(hdc,RGB(0,0,0));
            DrawText(hdc,"Gheorghe Reazantev",18,&rect,DT_TOP|DT_LEFT);
            //Destroy font object
            DeleteObject(SelectObject(hdc,hFontOld));
            //Set font of a button
            textHeight= -MulDiv(16, GetDeviceCaps(hdc, LOGPIXELSY), 72);
            textFont  = CreateFont(textHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Arial");
            hFontOld  = (HFONT)SelectObject(hdc,textFont);
            SendMessage(hwndInputButton,WM_SETFONT,WPARAM(textFont),TRUE);
            EndPaint(hwnd,&ps);
            return 0;

        case WM_CLOSE:
            //Make window jump to position 0,0 measured from TOP left screen corner
            SetWindowPos(hwnd,HWND_TOP,0,0,700,700,SWP_SHOWWINDOW);
            break;

        case WM_SYSCOMMAND:
            //Setting up the minimize and maximize default buttons to custom actions
            switch(wParam)
            {
                case SC_MINIMIZE:
                DestroyWindow(hwnd);
                break;

                case SC_MAXIMIZE:
                MessageBox(hwnd,TEXT("....maybe not"),szClassName,MB_ICONWARNING);
                break;
                default: DefWindowProc (hwnd, message, wParam, lParam);
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}


