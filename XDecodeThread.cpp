#include "XDecodeThread.h"
#include "XDecode.h"
void XDecodeThread::Close()
{
	Clear();
	
	isExit = true;
	wait();
	decode->Close();

	mux.lock();
	delete decode;
	decode = NULL;
	mux.unlock();
}
void XDecodeThread::Clear()
{
	mux.lock();
	decode->Clear();
	while (!packs.empty())
	{
		AVPacket *pkt = packs.front();
		XFreePacket(&pkt);
		packs.pop_front();
	}

	mux.unlock();
}

AVPacket *XDecodeThread::Pop()
{
	mux.lock();
	if (packs.empty())
	{
		mux.unlock();
		return NULL;
	}
	AVPacket *pkt = packs.front();
	packs.pop_front();
	mux.unlock();
	return pkt;
}
void XDecodeThread::Push(AVPacket *pkt)
{
	if (!pkt)return;
	
	while (!isExit)
	{
		mux.lock();
		if (packs.size() < maxList)
		{
			packs.push_back(pkt);
			mux.unlock();
			break;
		}
		mux.unlock();
		msleep(1);
	}
}


XDecodeThread::XDecodeThread()
{
	if (!decode) decode = new XDecode();
}


XDecodeThread::~XDecodeThread()
{	
	isExit = true;
	wait();
}
