#pragma once
#include <QThread>
#include "IVideoCall.h"
#include <mutex>
class XDemux;
class XVideoThread;
class XAudioThread;
class XDemuxThread:public QThread
{
public:
	virtual bool Open(const char *url, IVideoCall *call);

	virtual void Start();

	virtual void Close();
	virtual void Clear();

	virtual void Seek(double pos);

	void run();
	XDemuxThread();
	virtual ~XDemuxThread();
	bool isExit = false;
	long long pts = 0;
	long long totalMs = 0;
	void SetPause(bool isPause);
	bool isPause = false;
protected:
	std::mutex mux;
	XDemux *demux = 0;
	XVideoThread *vt = 0;
	XAudioThread *at = 0;
};

