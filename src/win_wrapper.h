#pragma once

#include <string>

struct Image;
struct Color;

typedef Image(*PFN_LoadImgFromScrn)();
typedef Color(*PFN_GetImgClr)(Image, int, int);
typedef void  (*PFN_UnloadImg)(Image);

void Win_EnableLayered(void* hwnd);

void SetWndProc(void* hwnd, PFN_LoadImgFromScrn LoadImgFromScrn, PFN_GetImgClr GetImgClr, PFN_UnloadImg UnloadImg);

void UpdateCachedFrame(PFN_LoadImgFromScrn LoadImg, PFN_UnloadImg UnloadImg);

void CustomSetWindowSize(void* hwnd, int x, int y);

bool IsAppInStartup(const std::string& appName);

void AddAppToStartup(const std::string& appName);

void RemoveAppFromStartup(const std::string& appName);
