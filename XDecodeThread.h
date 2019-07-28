#pragma once

struct AVPacket;
class XDecode;
#include <list>
#include <mutex>
#include <QThread>
class XDecodeThread:public QThread
{
public:
	XDecodeThread();
	virtual ~XDecodeThread();
	virtual void Push(AVPacket *pkt);


	virtual void Clear();


	virtual void Close();

	virtual AVPacket *Pop();
	int maxList = 100;
	bool isExit = false;
protected:
	XDecode *decode = 0;
	std::list <AVPacket *> packs;
	std::mutex mux;
};

