#include "XPlay2.h"
#include <QtWidgets/QApplication>
#include <iostream>
using namespace std;
#include "XDemux.h"
#include "XDecode.h"
#include "XResample.h"
#include <QThread>
#include "XAudioPlay.h"
#include "XAudioThread.h"
#include "XVideoThread.h"
class TestThread :public QThread
{
public:
	XAudioThread at;
	XVideoThread vt;
	void Init()
	{
		char *url = "rtmp://live.hkstv.hk.lxdns.com/live/hks";
		cout << "demux.Open = " << demux.Open(url);
		demux.Read();
		demux.Clear();
		demux.Close();
		url = "v1080.mp4";
		cout << "demux.Open = " << demux.Open(url);
		
		cout << "at.Open = "<<at.Open(demux.CopyAPara(), demux.sampleRate, demux.channels);
		vt.Open(demux.CopyVPara(), video, demux.width, demux.height);
		at.start();
		vt.start();
	}
	unsigned char *pcm = new unsigned char[1024 * 1024];
	void run()
	{
		for (;;)
		{
			AVPacket *pkt = demux.Read();
			if (demux.IsAudio(pkt))
			{
				at.Push(pkt);
			}
			else
			{
				vt.Push(pkt);
			}
			if (!pkt)
			{
				demux.Seek(0);
				//break;
			}
		}
	}
	
	XDemux demux;

	XVideoWidget *video = 0;

};
#include "XDemuxThread.h"
//v1080.mp4
int main(int argc, char *argv[])
{

	
	QApplication a(argc, argv);
	XPlay2 w;
	w.show();

	return a.exec();
}
