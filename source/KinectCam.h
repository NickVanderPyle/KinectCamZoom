#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class KinectCam
{
public:
	KinectCam();
	HRESULT Nui_Init();
	void Nui_UnInit();
	void Nui_GetCamFrame(BYTE *frameBuffer, int frameSize) const;

private:
	HANDLE m_hNextVideoFrameEvent;
	HANDLE m_pVideoStreamHandle;
};