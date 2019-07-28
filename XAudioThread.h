#pragma once
#include <QThread>
#include <mutex>
#include <list>
struct AVCodecParameters;
class XAudioPlay;
class XResample;
#include "XDecodeThread.h"
class XAudioThread:public XDecodeThread
{
public:
	long long pts = 0;
	virtual bool Open(AVCodecParameters *para,int sampleRate,int channels);

	virtual void Close();

	virtual void Clear();
	void run();
	XAudioThread();
	virtual ~XAudioThread();
	void SetPause(bool isPause);
	bool isPause = false;
protected:
	std::mutex amux;
	XAudioPlay *ap = 0;
	XResample *res = 0;

};

