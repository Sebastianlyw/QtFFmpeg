#pragma once

struct AVPacket;
struct AVCodecParameters;
class XDecode;
#include <list>
#include <mutex>
#include <QThread>
#include "IVideoCall.h"
#include "XDecodeThread.h"
class XVideoThread:public XDecodeThread
{
public:

	virtual bool RepaintPts(AVPacket *pkt, long long seekpts);
	virtual bool Open(AVCodecParameters *para,IVideoCall *call,int width,int height);
	void run();
	XVideoThread();
	virtual ~XVideoThread();
	long long synpts = 0;

	void SetPause(bool isPause);
	bool isPause = false;
protected:
	IVideoCall *call = 0;
	std::mutex vmux;


};

