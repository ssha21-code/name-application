#include <windows.h>
#include <stdlib.h>

#define WS_NONRESIZEABLEWINDOW (WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)
#define WS_STATICSTYLENORMAL (WS_VISIBLE | WS_CHILD | SS_CENTER)
#define WS_STATICSTYLENORMALPLUS (WS_VISIBLE | WS_CHILD)

#define WS_EDITSTYLENORMAL (WS_VISIBLE | WS_CHILD)
#define WS_EDITSTYLEOUTPUT (WS_VISIBLE | WS_CHILD | ES_READONLY | ES_AUTOVSCROLL)

#define WS_BUTTONNORMAL (WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT)

#define STATIC_ONE 101
#define STATIC_FIRSTNAME 201
#define STATIC_LASTNAME 202

#define EDIT_FIRSTNAME 301
#define EDIT_LASTNAME 302
#define EDIT_OUTPUT 501

#define BUTTON_GENERATE 401

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

void AddControls(HWND hWnd, HINSTANCE hInst);

HFONT hFont1;
HFONT hFont2;
HFONT hFont3;

HMENU hMenu;

HWND hStatic1;
HWND hStaticFirstName;
HWND hStaticLastName;
HWND hStaticOutput;

HWND hEditFirstName;
HWND hEditLastName;

HWND hEditOutput;

HWND hButtonGenerate;

COLORREF background_color = RGB(0x12, 0x12, 0x12);
HBRUSH hBackgroundBrush = CreateSolidBrush(background_color);

COLORREF frame_color = RGB(0x0F, 0x29, 0x29);
HBRUSH hFrameBrush = CreateSolidBrush(frame_color);

COLORREF foreground_color = RGB(0x64, 0xFF, 0xDA);
HBRUSH hForegroundBrush = CreateSolidBrush(foreground_color);

COLORREF edit_background_color = RGB(0x8D, 0x00, 0xFF);
HBRUSH hEditBackgroundBrush = CreateSolidBrush(foreground_color);

constexpr int WINDOW_WIDTH = 700;
constexpr int WINDOW_HEIGHT = 700;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow) {
    SetProcessDPIAware();
    WNDCLASSW wc = {0};
    wc.hbrBackground = hBackgroundBrush;
    wc.hInstance = hInst;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = L"WINDOW";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassW(&wc)) {
        return -1;
    }

    HWND hWnd = CreateWindowW(L"WINDOW", L"Names Application", WS_NONRESIZEABLEWINDOW | WS_VISIBLE, 
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, NULL, NULL
    );

    AddControls(hWnd, hInst);

    MSG msg = {0};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_COMMAND: {
            if (HIWORD(wp) == BN_CLICKED) {
                HWND hButton = (HWND)lp;
                int buttonId = LOWORD(wp);

                switch (wp) {
                    case BUTTON_GENERATE: {
                        wchar_t first_name[100] = L"";
                        wchar_t last_name[100] = L"";
                        wchar_t full_name[300] = L"";

                        GetWindowTextW(hEditFirstName, first_name, 100);
                        GetWindowTextW(hEditLastName, last_name, 100);
                        
                        wcscat(full_name, first_name);
                        wcscat(full_name, L" ");
                        wcscat(full_name, last_name);
                        
                        SetWindowTextW(hEditOutput, (LPCWSTR)full_name);
                        return 0;
                    }break;
                }
            }
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            RECT frame_rect = {50, 150, 640, 600};
            FillRect(hdc, &frame_rect, hFrameBrush);
            
            EndPaint(hWnd, &ps);
            return 0;
        }
        case WM_CREATE:
            
            return 0;
        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wp;
            HWND hWindow = (HWND)lp;

            int ctrlID = GetDlgCtrlID(hWindow);

            switch (ctrlID) {
                case STATIC_ONE: {
                    SetBkMode(hdcStatic, TRANSPARENT);
                    SetTextColor(hdcStatic, foreground_color);
                    return (INT_PTR)hBackgroundBrush;
                }
                case STATIC_FIRSTNAME: {
                    SetBkMode(hdcStatic, TRANSPARENT);
                    SetTextColor(hdcStatic, foreground_color);
                    return (INT_PTR)hFrameBrush;
                }
                case STATIC_LASTNAME: {
                    SetBkMode(hdcStatic, TRANSPARENT);
                    SetTextColor(hdcStatic, foreground_color);
                    return (INT_PTR)hFrameBrush;
                }
                case EDIT_OUTPUT: {
                    SetBkMode(hdcStatic, OPAQUE);
                    SetBkColor(hdcStatic, edit_background_color);
                    SetTextColor(hdcStatic, background_color);
                    return (INT_PTR)hEditBackgroundBrush;
                }
            }
            return 0;
        }
        case WM_CTLCOLOREDIT: {
            HDC hdcEdit = (HDC)wp;
            HWND hWindow = (HWND)lp;

            int ctrlId = GetDlgCtrlID(hWindow);
            switch (ctrlId) {
                case EDIT_FIRSTNAME: {
                    SetBkMode(hdcEdit, OPAQUE);
                    SetBkColor(hdcEdit, edit_background_color);
                    SetTextColor(hdcEdit, background_color);
                    return (INT_PTR)hEditBackgroundBrush;
                }
                case EDIT_LASTNAME: {
                    SetBkMode(hdcEdit, OPAQUE);
                    SetBkColor(hdcEdit, edit_background_color);
                    SetTextColor(hdcEdit, background_color);
                    return (INT_PTR)hEditBackgroundBrush;
                }
                
            }
            return 0;
        }
        

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(hWnd, msg, wp, lp);
    }
}

void AddControls(HWND hWnd, HINSTANCE hInst) {
    hFont1 = CreateFontW(40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, L"Helvetica");
    hFont2 = CreateFontW(25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, L"Helvetica");
    hFont3 = CreateFontW(22, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, L"Helvetica");

    hStatic1 = CreateWindowW(L"STATIC", L"Names Application", WS_STATICSTYLENORMAL, 
        0, 50, WINDOW_WIDTH, 50, hWnd, (HMENU)STATIC_ONE, hInst, NULL
    );
    SendMessageW(hStatic1, WM_SETFONT, (WPARAM)hFont1, TRUE);
    hStaticFirstName = CreateWindowW(L"STATIC", L"Enter your first name: ", WS_STATICSTYLENORMALPLUS, 
        90, 200, 250, 50, hWnd, (HMENU)STATIC_FIRSTNAME, hInst, NULL
    );
    SendMessageW(hStaticFirstName, WM_SETFONT, (WPARAM)hFont2, TRUE);
    hStaticLastName = CreateWindowW(L"STATIC", L"Enter your second name: ", WS_STATICSTYLENORMALPLUS, 
        90, 265, 250, 50, hWnd, (HMENU)STATIC_LASTNAME, hInst, NULL
    );
    SendMessageW(hStaticLastName, WM_SETFONT, (WPARAM)hFont2, TRUE);
    hEditFirstName = CreateWindowW(L"EDIT", L"", WS_EDITSTYLENORMAL, 
        350, 200, 250, 25, hWnd, (HMENU)EDIT_FIRSTNAME, hInst, NULL
    );
    SendMessageW(hEditFirstName, WM_SETFONT, (WPARAM)hFont3, TRUE);
    hEditLastName = CreateWindowW(L"EDIT", L"", WS_EDITSTYLENORMAL, 
        350, 265, 250, 25, hWnd, (HMENU)EDIT_LASTNAME, hInst, NULL
    );
    SendMessageW(hEditLastName, WM_SETFONT, (WPARAM)hFont3, TRUE);
    hButtonGenerate = CreateWindowW(L"BUTTON", L"Generate!", WS_BUTTONNORMAL, 
        WINDOW_WIDTH / 2 - 75, 350, 150, 50, hWnd, (HMENU)BUTTON_GENERATE, hInst, NULL
    );
    SendMessageW(hButtonGenerate, WM_SETFONT, (WPARAM)hFont2, TRUE);
    hStaticOutput = CreateWindowW(L"STATIC", L"Output: ", WS_STATICSTYLENORMAL, 
        50, 425, 590, 50, hWnd, (HMENU)STATIC_LASTNAME, hInst, NULL
    );
    SendMessageW(hStaticOutput, WM_SETFONT, (WPARAM)hFont2, TRUE);
    hEditOutput = CreateWindowW(L"EDIT", L"", WS_EDITSTYLEOUTPUT, 
        100, 470, 520, 110, hWnd, (HMENU)EDIT_OUTPUT, hInst, NULL
    );
    SendMessageW(hEditOutput, WM_SETFONT, (WPARAM)hFont3, TRUE);
    
    SendMessageW(hWnd, WM_CHANGEUISTATE, MAKEWPARAM(UIS_SET, UISF_HIDEFOCUS), 0);
    

}
