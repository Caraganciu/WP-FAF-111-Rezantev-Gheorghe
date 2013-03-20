#include <windows.h>
#include <stdio.h>
#include "resources.h"

/*  Declare Windows procedure and other subclassed procedures  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ListProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK    InfoDlgProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK ScrollProc(HWND, UINT, WPARAM, LPARAM);
// Create global variables that will be used in functions
static int iIndex;
static char buffer[100];
WNDPROC oldList,oldScroll;
// Create an child initialization function
void CreateChildren(const HINSTANCE&,const HWND&);
/*  Make the class name into a global variable  */
char szClassName[ ] = "Lab#2";
/*Create a global instance variable    */
HINSTANCE hInst;
// Create global handle to menu
HMENU hMenu;
// Create an array of five people
People employees[5]={"Steve",25,"Bill",38,"Don",15,"Garry",20,"Gena",19};
// Create handles that will be easier to manage on a global scale
HWND hwndListPeople,hwndScrollColor,hwndScrollVPOS,hwndScrollHPOS;
HWND hwndStaticButton;
// Create a variable that will save the color atm of the text (alfa) and to position variable that will help modify the window position
int alfa=0,posY=0,posX=0;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    hInst=hThisInstance;
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    hMenu=LoadMenu(hInst,MAKEINTRESOURCE(IDI_MENU));
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use personalized icon and pointer  */
    wincl.hIcon = LoadIcon (GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON));
    wincl.hIconSm = LoadIcon (GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON));
    wincl.hCursor = LoadCursor (GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_CURSOR));
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Lab#2",             /* Title Text */
           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, /* make window non-resizeable*/
           0,                   /* Put window in upper left corner or the screen  */
           2,                   /* same as above */
           512,                 /* The programs width */
           512,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           hMenu,               /* menu */
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
{   //Create the variables needed for changing the background color of the listbox
    static int listRedColor=255,listBlueColor=255,listGreenColor=255;
    // Create brushes that will be used to modify text and background colors
    static HBRUSH hBrushStatic,hBrushList;

    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            CreateChildren(hInst,hwnd);
            for(int i=0;i<5;i++) {
            SendMessage(hwndListPeople,LB_ADDSTRING,0,(LPARAM)employees[i].name);
            }
            SetScrollRange(hwndScrollColor,SB_CTL,0,255,FALSE);
            SetScrollPos(hwndScrollColor,SB_CTL,0,FALSE);

            SetScrollRange(hwndScrollVPOS,SB_CTL,0,200,FALSE);
            SetScrollPos(hwndScrollVPOS,SB_CTL,200,FALSE);

            SetScrollRange(hwndScrollHPOS,SB_CTL,0,200,FALSE);
            SetScrollPos(hwndScrollHPOS,SB_CTL,0,FALSE);
            //Create exit points for the subclassed functions to use when returning command to main proc
            oldScroll=(WNDPROC) SetWindowLong(hwndScrollColor,GWL_WNDPROC,(LPARAM)ScrollProc);
            oldList=(WNDPROC) SetWindowLong (hwndListPeople,GWL_WNDPROC,(LPARAM)ListProc);
            break;
        //Handle scroll messages
        case WM_VSCROLL:
            if((HWND)lParam==hwndScrollColor)
            {
                switch(LOWORD(wParam))
                {
                case SB_PAGEDOWN:
                    alfa+=DELTA_COLOR;
                case SB_LINEDOWN:
                    alfa=min(255,alfa+1);
                    break;
                case SB_PAGEUP:
                    alfa-=DELTA_COLOR;
                case SB_LINEUP:
                    alfa=max(0,alfa-1);
                    break;
                case SB_TOP:
                    alfa=0;
                    break;
                case SB_BOTTOM:
                    alfa=255;
                    break;
                case SB_THUMBPOSITION:
                case SB_THUMBTRACK:
                    alfa=HIWORD(wParam);
                    break;
                default: break;
                }

                SetScrollPos(hwndScrollColor,SB_CTL,alfa,TRUE);
                InvalidateRect(hwndStaticButton,NULL,TRUE);
            }
            if((HWND)lParam==hwndScrollVPOS)
            {
                switch(LOWORD(wParam))
                {
                case SB_PAGEDOWN:
                    posY-=Y_PAGE_MOVE;
                case SB_LINEDOWN:
                    posY=max(0,posY-Y_LINE_MOVE);
                    break;
                case SB_PAGEUP:
                    posY+=Y_PAGE_MOVE;
                case SB_LINEUP:
                    posY=min(200,posY+Y_LINE_MOVE);
                    break;
                case SB_TOP:
                    posY=200;
                    break;
                case SB_BOTTOM:
                    posY=0;
                    break;
                case SB_THUMBPOSITION:
                    posY=HIWORD(wParam);
                    break;
                default: break;
                }

                SetScrollPos(hwndScrollVPOS,SB_CTL,200-posY,TRUE);
                SetWindowPos(hwnd,HWND_TOP,posX,posY,0,0,SWP_SHOWWINDOW|SWP_NOSIZE);
            }
            return 0;
        case WM_HSCROLL:
            switch(LOWORD(wParam))
                {
                case SB_PAGELEFT:
                    posX-=X_PAGE_MOVE;
                case SB_LINELEFT:
                    posX=max(0,posX-X_LINE_MOVE);
                    break;
                case SB_PAGERIGHT:
                    posX+=X_PAGE_MOVE;
                case SB_LINERIGHT:
                    posX=min(200,posX+X_LINE_MOVE);
                    break;
                case SB_RIGHT:
                    posX=200;
                    break;
                case SB_LEFT:
                    posX=0;
                    break;
                case SB_THUMBPOSITION:
                    posX=HIWORD(wParam);
                    break;
                default: break;
                }
                SetScrollPos(hwndScrollHPOS,SB_CTL,posX,TRUE);
                SetWindowPos(hwnd,HWND_TOP,posX,posY,0,0,SWP_SHOWWINDOW|SWP_NOSIZE);
            return 0;
        //Handle the menu commands and the listbox interactions
        case WM_COMMAND:
            if(LOWORD(wParam)==ID_LIST_PEOPLE && HIWORD(wParam)==LBN_DBLCLK)
            {
                iIndex=SendMessage(hwndListPeople,LB_GETCURSEL,0,0);
                sprintf(buffer,"Name:: %s\nAge::%i",employees[iIndex].name,employees[iIndex].age);
                MessageBox(hwnd,buffer,TEXT("Employee information"),MB_ICONASTERISK|MB_OK);
                SetFocus(hwndListPeople);
            }
            switch(LOWORD(wParam))
            {
            case IDI_EXIT:
                if(MessageBox(hwnd,"Are you sure?","See Ya",MB_ICONQUESTION|MB_YESNO)==IDYES) DestroyWindow(hwnd);
                break;
            case IDI_RED:
                listRedColor=255;
                listBlueColor=0;
                listGreenColor=0;
                InvalidateRect(hwndListPeople,NULL,TRUE);
                break;
            case IDI_WHITE:
                listRedColor=255;
                listBlueColor=255;
                listGreenColor=255;
                InvalidateRect(hwndListPeople,NULL,TRUE);
                break;
            case IDI_INFO:
                DialogBox(hInst,MAKEINTRESOURCE(IDI_DIALOG),hwnd,InfoDlgProc);
                break;
            }
            break;
        //Handle the combinations from the keyboard input
        case WM_KEYDOWN:

            switch(wParam)
            {
               case KEY_L:
                   if(GetAsyncKeyState(VK_LSHIFT)) SetFocus(hwndListPeople);
               break;

               case KEY_S:
                   if(GetAsyncKeyState(VK_LCONTROL)) {
                   SetFocus(hwndScrollColor);
                   }
               break;
            }
        break;
        // Define how the listbox and the static button should be colored
        case WM_CTLCOLORLISTBOX:
            SetBkMode((HDC)wParam,TRANSPARENT);
            hBrushList=(HBRUSH) CreateSolidBrush(RGB(listRedColor,listGreenColor,listBlueColor));
            return(LRESULT) hBrushList;

        case WM_CTLCOLORSTATIC:
            SetTextColor((HDC)wParam,RGB(alfa,0,255-alfa));
            SetBkMode((HDC)wParam,TRANSPARENT);
            hBrushStatic=(HBRUSH)GetStockObject(NULL_BRUSH);
            return(LRESULT) hBrushStatic;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}


void
CreateChildren(const HINSTANCE& hInst,const HWND& hwnd){
    RECT rect;
    GetClientRect(hwnd,&rect);
    hwndListPeople=CreateWindowEx((DWORD)NULL,"listbox",TEXT("Hello"),WS_VISIBLE|WS_CHILD|WS_VSCROLL|LBS_NOINTEGRALHEIGHT|WS_BORDER|LBS_NOTIFY,
                                  5,5,200,rect.bottom-10,hwnd,(HMENU)ID_LIST_PEOPLE,hInst,NULL);
    hwndScrollColor=CreateWindowEx((DWORD)NULL,"scrollbar",TEXT(""),WS_VISIBLE|WS_CHILD|SBS_VERT,
                                    rect.right-30,5,20,rect.bottom-10,hwnd,(HMENU)ID_SCROLL_COLOR,hInst,NULL);
    hwndScrollVPOS=CreateWindowEx((DWORD)NULL,"scrollbar",TEXT(""),WS_VISIBLE|WS_CHILD|SBS_VERT,
                                    rect.right-60,5,20,rect.bottom/2-5,hwnd,(HMENU)ID_SCROLL_VPOS,hInst,NULL);
    hwndScrollHPOS=CreateWindowEx((DWORD)NULL,"scrollbar",TEXT(""),WS_VISIBLE|WS_CHILD|SBS_HORZ,
                                    rect.right-200,5,130,20,hwnd,(HMENU)ID_SCROLL_HPOS,hInst,NULL);
    hwndStaticButton=CreateWindowEx((DWORD)NULL,"static",TEXT("Use the scroll above and to the right to move the main window"),WS_VISIBLE|WS_CHILD,
                                    rect.right-200,35,130,100,hwnd,(HMENU)ID_STATIC_BUTTON,hInst,NULL);
    SetClassLong(hwndListPeople,GCL_HCURSOR,(LONG)LoadCursor(hInst,MAKEINTRESOURCE(IDI_CURSOR)));
    SetClassLong(hwndScrollColor,GCL_HCURSOR,(LONG)LoadCursor(hInst,MAKEINTRESOURCE(IDI_CURSOR)));
    SetClassLong(hwndScrollHPOS,GCL_HCURSOR,(LONG)LoadCursor(hInst,MAKEINTRESOURCE(IDI_CURSOR)));
    SetClassLong(hwndScrollVPOS,GCL_HCURSOR,(LONG)LoadCursor(hInst,MAKEINTRESOURCE(IDI_CURSOR)));
}

LRESULT CALLBACK ListProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    if(message==WM_KEYDOWN && wParam==KEY_S)
    {
        if(GetAsyncKeyState(VK_LCONTROL)) SetFocus(hwndScrollColor);
        else SetFocus(hwndListPeople);
    }
    if(message==WM_KEYDOWN && wParam==VK_SPACE)
    {
        iIndex=SendMessage(hwndListPeople,LB_GETCURSEL,0,0);
        sprintf(buffer,"Name:: %s\nAge::%i",employees[iIndex].name,employees[iIndex].age);
        MessageBox(hwnd,buffer,TEXT("Employee information"),MB_ICONASTERISK|MB_OK);
        SetFocus(hwndListPeople);
    }
    return CallWindowProc(oldList,hwnd,message,wParam,lParam);
}


BOOL CALLBACK    InfoDlgProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam) {
    switch(message)
    {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDOK:
            EndDialog(hwnd,0);
            return TRUE;

        }
    break;
    }
    return FALSE;
}

LRESULT CALLBACK ScrollProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if(message==WM_KEYDOWN && wParam==KEY_L)
    {
        if(GetAsyncKeyState(VK_LSHIFT)) SetFocus(hwndListPeople);
        else SetFocus(hwndScrollColor);
    }
    return CallWindowProc(oldScroll,hwnd,message,wParam,lParam);
}
