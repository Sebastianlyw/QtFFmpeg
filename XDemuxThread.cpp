#include "XDemuxThread.h"
#include "XDemux.h"
#include "XVideoThread.h"
#include "XAudioThread.h"
#include <iostream>
extern "C"
{
#include <libavformat/avformat.h>
}
#include "XDecode.h"

using namespace std;
void XDemuxThread::Clear()
{
	mux.lock();
	if (demux)demux->Clear();
	if (vt) vt->Clear();
	if (at) at->Clear();
	mux.unlock();
}

void XDemuxThread::Seek(double pos)
{
	Clear();

	mux.lock();
	bool status = this->isPause;
	mux.unlock();
	
	SetPause(true);

	mux.lock();
	if (demux)
		demux->Seek(pos);
	long long seekPts = pos*demux->totalMs;
	while (!isExit)
	{
		AVPacket *pkt = demux->ReadVideo();
		if (!pkt) break;
		if (vt->RepaintPts(pkt, seekPts))
		{
			this->pts = seekPts;
			break;
		}
	}

	mux.unlock();

	if(!status)
		SetPause(false);
}

void XDemuxThread::SetPause(bool isPause)
{
	mux.lock();
	this->isPause = isPause;
	if (at) at->SetPause(isPause);
	if (vt) vt->SetPause(isPause);
	mux.unlock();
}

void XDemuxThread::run()
{
	while (!isExit)
	{
		mux.lock();
		if (isPause)
		{
			mux.unlock();
			msleep(5);
			continue;
		}
		if (!demux)
		{
			mux.unlock();
			msleep(5);
			continue;
		}

		if (vt && at)
		{
			pts = at->pts;
			vt->synpts = at->pts;
		}


		AVPacket *pkt = demux->Read();
		if (!pkt) 
		{
			mux.unlock();
			msleep(5);
			continue;
		}
		if (demux->IsAudio(pkt))
		{
			//while (at->IsFull())
			{
			//	vt->synpts = at->pts;
			}
			if(at)at->Push(pkt);
		}
		else 
		{
			//while (vt->IsFull())
			//{
			//	vt->synpts = at->pts;
			//}
			if (vt)vt->Push(pkt);
		}
		mux.unlock();
		msleep(1);
	}
}


bool XDemuxThread::Open(const char *url, IVideoCall *call)
{
	if (url == 0 || url[0] == '\0')
		return false;

	mux.lock();
	if (!demux) demux = new XDemux();
	if (!vt) vt = new XVideoThread();
	if (!at) at = new XAudioThread();

	bool re = demux->Open(url);
	if (!re)
	{
		mux.unlock();
		cout << "demux->Open(url) failed!" << endl;
		return false;
	}
	if (!vt->Open(demux->CopyVPara(), call, demux->width, demux->height))
	{
		re = false;
		cout << "vt->Open failed!" << endl;
	}
	if (!at->Open(demux->CopyAPara(), demux->sampleRate, demux->channels))
	{
		re = false;
		cout << "at->Open failed!" << endl;
	}
	totalMs = demux->totalMs;
	mux.unlock();

	cout << "XDemuxThread::Open " << re << endl;
	return re;
}

void XDemuxThread::Close()
{
	isExit = true;
	wait();
	if (vt) vt->Close();
	if (at) at->Close();
	mux.lock();
	delete vt;
	delete at;
	vt = NULL;
	at = NULL;
	mux.unlock();
}

void XDemuxThread::Start()
{
	mux.lock();
	if (!demux) demux = new XDemux();
	if (!vt) vt = new XVideoThread();
	if (!at) at = new XAudioThread();
	QThread::start();
	if (vt)vt->start();
	if (at)at->start();
	mux.unlock();
}
XDemuxThread::XDemuxThread()
{
}


XDemuxThread::~XDemuxThread()
{
	isExit = true;
	wait();
}
