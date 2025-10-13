#include "win_wrapper.h"
#include <Windows.h>
#include <iostream>
#include <string>

typedef struct Image
{
	void* data;
	int width;
	int height;
	int mipmaps;
	int format;
} Image;

typedef struct Color
{
	unsigned char r, g, b, a;
} Color;

static WNDPROC g_OldWndProc = nullptr;

static PFN_LoadImgFromScrn pLoadImgFromScrn = nullptr;
static PFN_GetImgClr       pGetImgClr = nullptr;
static PFN_UnloadImg       pUnloadImg = nullptr;

// Cached screen frame (updated once per frame)
static Image g_CachedFrame = { 0 };

// Update the cached frame from the main loop
void UpdateCachedFrame(PFN_LoadImgFromScrn LoadImg, PFN_UnloadImg UnloadImg)
{
	if (g_CachedFrame.data)
	{
		UnloadImg(g_CachedFrame);
		g_CachedFrame.data = nullptr;
	}
	g_CachedFrame = LoadImg();
}

LRESULT CALLBACK CustomWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void SetWndProc(void* hwnd,
	PFN_LoadImgFromScrn LoadImgFromScrn,
	PFN_GetImgClr GetImgClr,
	PFN_UnloadImg UnloadImg)
{
	pLoadImgFromScrn = LoadImgFromScrn;
	pGetImgClr = GetImgClr;
	pUnloadImg = UnloadImg;

	g_OldWndProc = (WNDPROC)SetWindowLongPtr((HWND)hwnd, GWLP_WNDPROC, (LONG_PTR)CustomWndProc);
}

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

void Win_EnableLayered(void* hwnd)
{
	HWND h = (HWND)hwnd;
	LONG exStyle = GetWindowLong(h, GWL_EXSTYLE);
	SetWindowLong(h, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);
	SetLayeredWindowAttributes(h, 0, 0, LWA_COLORKEY);
}

LRESULT CALLBACK CustomWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCHITTEST && pGetImgClr)
	{
		if (!g_CachedFrame.data)
			return HTCLIENT; // fallback

		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		ScreenToClient(hwnd, &pt);

		if (pt.x >= 0 && pt.y >= 0 &&
			pt.x < g_CachedFrame.width && pt.y < g_CachedFrame.height)
		{
			Color c = pGetImgClr(g_CachedFrame, pt.x, pt.y);
			if (c.a == 0)
			{
				return HTTRANSPARENT; // let click pass through
			}
		}
		return HTCLIENT;
	}

	return CallWindowProc(g_OldWndProc, hwnd, msg, wParam, lParam);
}

void CustomSetWindowSize(void* hwnd, int width, int height)
{
	RECT rect;
	GetWindowRect((HWND)hwnd, &rect);

	int oldSizeX = rect.right - rect.left;
	int oldSizeY = rect.bottom - rect.top;

	int deltaX = (width - oldSizeX) / 2.0f;
	int deltaY = (height - oldSizeY) / 2.0f;

	int newX = rect.left - deltaX;
	int newY = rect.top - deltaY;

	SendMessage((HWND)hwnd, WM_SETREDRAW, FALSE, 0);

	SetWindowPos((HWND)hwnd, HWND_TOP, newX, newY, width, height, SWP_NOZORDER | SWP_NOACTIVATE);

	SendMessage((HWND)hwnd, WM_SETREDRAW, TRUE, 0);
	RedrawWindow((HWND)hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
}

static std::string GetOwnExePath()
{
	char path[MAX_PATH];
	DWORD len = GetModuleFileNameA(nullptr, path, MAX_PATH);
	return (len > 0) ? std::string(path) : "";
}

static const char* STARTUP_KEY = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
bool IsAppInStartup(const std::string& appName)
{
	// TODO: check if the value of the key matches the exe path

	HKEY hKey;
	bool exists = false;

	if (RegOpenKeyExA(HKEY_CURRENT_USER, STARTUP_KEY, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		char value[512];
		DWORD value_length = sizeof(value);
		DWORD type = REG_SZ;

		if (RegQueryValueExA(hKey,
			appName.c_str(),
			nullptr, &type,
			reinterpret_cast<BYTE*>(value),
			&value_length
		) == ERROR_SUCCESS)
			exists = true;

		RegCloseKey(hKey);
	}

	return exists;
}

void AddAppToStartup(const std::string& appName)
{
	std::string exePath = GetOwnExePath();
	if (exePath.empty()) return;

	HKEY hKey;
	RegOpenKeyExA(HKEY_CURRENT_USER, STARTUP_KEY, 0, KEY_SET_VALUE, &hKey);
	RegSetValueExA(hKey, appName.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(exePath.c_str()), static_cast<DWORD>(exePath.size() + 1));
	RegCloseKey(hKey);
}

void RemoveAppFromStartup(const std::string& appName)
{
	HKEY hKey;
	RegOpenKeyExA(HKEY_CURRENT_USER, STARTUP_KEY, 0, KEY_SET_VALUE, &hKey);
	RegDeleteValueA(hKey, appName.c_str());
	RegCloseKey(hKey);
}
