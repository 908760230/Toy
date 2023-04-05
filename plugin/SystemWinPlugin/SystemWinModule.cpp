#include "SystemWinModule.h"

namespace Toy
{
bool SystemWinModule::Awake()
{
    return true;
}
bool SystemWinModule::Init()
{
    winclass.cbSize        = sizeof(WNDCLASSEX);
    winclass.style         = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    winclass.lpfnWndProc   = WindowProc;
    winclass.cbClsExtra    = 0;
    winclass.cbWndExtra    = 0;
    winclass.hInstance     = hinstance ? hinstance : GetModuleHandle(nullptr);
    winclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    winclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winclass.lpszMenuName  = NULL;
    winclass.lpszClassName = "WINCLASS";
    winclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    if (!RegisterClassEx(&winclass)) return false;
    return true;
}
bool SystemWinModule::AfterInit()
{
    hwnd = CreateWindowEx(NULL,                                   // extended style
                          "WINCLASS",                            // class
                          "windows",                             // title
                          WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, // initial x,y
                          m_width, m_height,                     // initial width, height
                          NULL,                                   // handle to parent
                          NULL,                                   // handle to menu
                          hinstance,                              // instance of this application
                          NULL);

    if (!hwnd) return false;
}
bool SystemWinModule::Execute()
{
    return pollEvent();
}
void SystemWinModule::run()
{
}
bool SystemWinModule::pollEvent()
{
    bool flag = GetMessage(&msg, NULL, 0, 0);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (msg.message == WM_QUIT)
        flag = false;
    return flag;
}
void SystemWinModule::resize(int w, int h)
{
}
LRESULT SystemWinModule::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    PAINTSTRUCT ps;  // used in WM_PAINT
    HDC         hdc; // handle to a device context
    // what is the message
    switch (msg)
    {
    case WM_CREATE: {
        // 窗体居中显示
        int  scrWidth, scrHeight;
        RECT rect;
        // 获得屏幕尺寸
        scrWidth  = GetSystemMetrics(SM_CXSCREEN);
        scrHeight = GetSystemMetrics(SM_CYSCREEN);
        // 取得窗口尺寸
        GetWindowRect(hwnd, &rect);
        // 重新设置rect里的值
        rect.left = (scrWidth - rect.right) / 2;
        rect.top  = (scrHeight - rect.bottom) / 2;
        // 移动窗口到指定的位置
        SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
    }
    break;
    case WM_KEYDOWN:
        break;
    case WM_KEYUP:
        break;
    case WM_CHAR:
        break;
    case WM_PAINT: {
        // simply validate the window
        hdc = BeginPaint(hwnd, &ps);
        // you would do all your painting here
        EndPaint(hwnd, &ps);

        // return success
        return (0);
    }
    break;
    case WM_MOUSEMOVE: {
        int    scrWidth  = GetSystemMetrics(SM_CXSCREEN);
        int    scrHeight = GetSystemMetrics(SM_CYSCREEN);
        POINTS pt        = MAKEPOINTS(lparam);
        
        break;
    }
    case WM_LBUTTONDOWN: {
        const POINTS pt = MAKEPOINTS(lparam);
        break;
    }
    case WM_LBUTTONUP: {
        break;
    }
    case WM_RBUTTONDOWN: {
        const POINTS pt = MAKEPOINTS(lparam);
        break;
    }
    case WM_RBUTTONUP: {
        const POINTS pt = MAKEPOINTS(lparam);
        break;
    }
    case WM_MOUSEWHEEL: {
        const POINTS pt = MAKEPOINTS(lparam);
        if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
        {
        }
        else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
        {
        }
        break;
    }
    case WM_DESTROY: {
        // kill the application, this sends a WM_QUIT message
        PostQuitMessage(0);

        // return success
        return (0);
    }

    default:
        break;

    } // end switch

    // process any messages that we didn't take care of
    return (DefWindowProc(hwnd, msg, wparam, lparam));
}
} // namespace Toy
