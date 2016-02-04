#pragma once

#include "KinectCam.h"

#define DECLARE_PTR(type, ptr, expr) type* ptr = (type*)(expr);

EXTERN_C const GUID CLSID_VirtualCam;

class CVCamStream;
class CVCam : public CSource
{
public:
	//////////////////////////////////////////////////////////////////////////
	//  IUnknown
	//////////////////////////////////////////////////////////////////////////
	static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);
	STDMETHODIMP QueryInterface(REFIID riid, void **ppv);

	IFilterGraph *GetGraph() const	{ return m_pGraph; }

	KinectCam m_kinectCam;
	bool m_kinected;
	BYTE *m_pBuffer;
	int m_pBufferSize;

private:
	CVCam(LPUNKNOWN lpunk, HRESULT *phr);
	~CVCam();
};

class CVCamStream : public CSourceStream, public IAMStreamConfig, public IKsPropertySet
{
public:

	//////////////////////////////////////////////////////////////////////////
	//  IUnknown
	//////////////////////////////////////////////////////////////////////////
	STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP_(ULONG) AddRef() { return GetOwner()->AddRef(); }                                                          \
		STDMETHODIMP_(ULONG) Release() { return GetOwner()->Release(); }

	//////////////////////////////////////////////////////////////////////////
	//  IQualityControl
	//////////////////////////////////////////////////////////////////////////
	STDMETHODIMP Notify(IBaseFilter * pSender, Quality q);

	//////////////////////////////////////////////////////////////////////////
	//  IAMStreamConfig
	//////////////////////////////////////////////////////////////////////////
	HRESULT STDMETHODCALLTYPE SetFormat(AM_MEDIA_TYPE *pmt) override;
	HRESULT STDMETHODCALLTYPE GetFormat(AM_MEDIA_TYPE **ppmt) override;
	HRESULT STDMETHODCALLTYPE GetNumberOfCapabilities(int *piCount, int *piSize) override;
	HRESULT STDMETHODCALLTYPE GetStreamCaps(int iIndex, AM_MEDIA_TYPE **pmt, BYTE *pSCC) override;

	//////////////////////////////////////////////////////////////////////////
	//  IKsPropertySet
	//////////////////////////////////////////////////////////////////////////
	HRESULT STDMETHODCALLTYPE Set(REFGUID guidPropSet, DWORD dwID, void *pInstanceData, DWORD cbInstanceData, void *pPropData, DWORD cbPropData) override;
	HRESULT STDMETHODCALLTYPE Get(REFGUID guidPropSet, DWORD dwPropID, void *pInstanceData, DWORD cbInstanceData, void *pPropData, DWORD cbPropData, DWORD *pcbReturned) override;
	HRESULT STDMETHODCALLTYPE QuerySupported(REFGUID guidPropSet, DWORD dwPropID, DWORD *pTypeSupport) override;

	//////////////////////////////////////////////////////////////////////////
	//  CSourceStream
	//////////////////////////////////////////////////////////////////////////
	CVCamStream(HRESULT *phr, CVCam *pParent, LPCWSTR pPinName);

	HRESULT FillBuffer(IMediaSample *pms) override;
	HRESULT DecideBufferSize(IMemAllocator *pIMemAlloc, ALLOCATOR_PROPERTIES *pProperties) override;
	HRESULT CheckMediaType(const CMediaType *pMediaType) override;
	HRESULT GetMediaType(int iPosition, CMediaType *pmt) override;
	HRESULT SetMediaType(const CMediaType *pmt) override;
	HRESULT OnThreadCreate(void) override;

private:
	CVCam *m_pParent;
	REFERENCE_TIME m_rtLastTime;
	HBITMAP m_hLogoBmp;
	CCritSec m_cSharedState;
	IReferenceClock *m_pClock;
};