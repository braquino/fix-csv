#ifndef WIN32_DX11_UI_H_
#define WIN32_DX11_UI_H_

#include "baseui.h"
#include "backend/imgui_impl_win32.h"
#include "backend/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <memory>

// Data
static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


class Win32Dx11UI : public BaseUI
{
public:
    Win32Dx11UI(int _width, int _height, std::string _title);
    ~Win32Dx11UI();
    void start_loop(std::function<void()> func) override;

private:

};

#endif // WIN32_DX11_UI_H_