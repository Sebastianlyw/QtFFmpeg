#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>
#include <mutex>
#include "IVideoCall.h"
struct AVFrame;
class XVideoWidget : public QOpenGLWidget, protected QOpenGLFunctions,public IVideoCall
{
	Q_OBJECT

public:
	virtual void Init(int width, int height);

	virtual void Repaint(AVFrame *frame);

	XVideoWidget(QWidget *parent);
	~XVideoWidget();
protected:
	void paintGL();

	void initializeGL();

	void resizeGL(int width, int height);
private:
	std::mutex mux;
	QGLShaderProgram program;

	GLuint unis[3] = { 0 };
	GLuint texs[3] = { 0 };
	unsigned char *datas[3] = { 0 };

	int width = 240;
	int height = 128;

};
