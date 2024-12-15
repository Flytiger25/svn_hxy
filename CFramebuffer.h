#pragma once
#include <glut.h>
//#include <cv.h>
//#include <highgui.h>
class CFramebuffer
{
public:
	CFramebuffer();
	~CFramebuffer();
private:
	unsigned int m_FboID;
	unsigned int m_RboID;
	unsigned int m_tex;
	bool     m_bIsBegined;
	int      m_curbuff;

public:
	int     m_width;
	int		m_height;

public:
	void init(int width, int height);
	bool begin();
	bool end();
	void saveFrameBuff(const char* fileName);

	unsigned int getTex() { return m_tex; }
};

