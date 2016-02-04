#include "KinectCam.h"
#include <shlobj.h>
#include "NuiApi.h"

KinectCam::KinectCam()
{
	m_hNextVideoFrameEvent = nullptr;
	m_pVideoStreamHandle = nullptr;
}

HRESULT KinectCam::Nui_Init()
{
	m_hNextVideoFrameEvent = nullptr;
	m_pVideoStreamHandle = nullptr;
	m_hNextVideoFrameEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

	auto hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR);
	if (FAILED(hr))
	{
		return hr;
	}
	DebugBreak();
	NuiCameraElevationSetAngle(0);

	auto resolution = NUI_IMAGE_RESOLUTION_640x480;

	hr = NuiImageStreamOpen(
		NUI_IMAGE_TYPE_COLOR,
		resolution,
		0,
		2,
		m_hNextVideoFrameEvent,
		&m_pVideoStreamHandle);

	return hr;
}

void KinectCam::Nui_UnInit()
{
	NuiCameraElevationSetAngle(0);

	NuiShutdown();

	if (m_hNextVideoFrameEvent && (m_hNextVideoFrameEvent != INVALID_HANDLE_VALUE))
	{
		CloseHandle(m_hNextVideoFrameEvent);
		m_hNextVideoFrameEvent = nullptr;
	}
}

void KinectCam::Nui_GetCamFrame(BYTE *frameBuffer, int frameSize) const
{
	const NUI_IMAGE_FRAME *pImageFrame = nullptr;

	WaitForSingleObject(m_hNextVideoFrameEvent, INFINITE);

	auto hr = NuiImageStreamGetNextFrame(
		m_pVideoStreamHandle,
		0,
		&pImageFrame);
	if (FAILED(hr))
	{
		return;
	}

	auto pTexture = pImageFrame->pFrameTexture;
	NUI_LOCKED_RECT LockedRect;
	pTexture->LockRect(0, &LockedRect, nullptr, 0);
	if (LockedRect.Pitch != 0)
	{
		auto pBuffer = static_cast<BYTE*>(LockedRect.pBits);
		memcpy(frameBuffer, pBuffer, frameSize);
	}

	NuiImageStreamReleaseFrame(m_pVideoStreamHandle, pImageFrame);
}
