#pragma once
struct AVCodecParameters;
struct AVCodecContext;
struct AVFrame;
struct AVPacket;
#include <mutex>
extern void XFreePacket(AVPacket **pkt);
extern void XFreeFrame(AVFrame **frame);
class XDecode
{
public:
	bool isAudio = false;


	long long pts = 0;
	
	virtual bool Open(AVCodecParameters *para);

	virtual bool Send(AVPacket *pkt);
	
	virtual AVFrame* Recv();

	virtual void Close();
	virtual void Clear();

	XDecode();
	virtual ~XDecode();
protected:
	AVCodecContext *codec = 0;
	std::mutex mux;
};

