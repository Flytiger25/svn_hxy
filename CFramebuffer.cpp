#include "stdafx.h"
//#include <cv.h>
//#include <highgui.h>
//#include<opencv2/core/core.hpp>
//#include<opencv2/imgproc/imgproc.hpp>
//#include<opencv2/highgui/highgui.hpp>
#include<stdio.h>
#include <string>
#include "CFramebuffer.h"
#include "SDIViewSwitch.h"
#include <iostream>
#include "MyTreeView.h"
#define texturewidth 512
#define textureheight 512
//#pragma comment(lib, "glew32.lib")
CFramebuffer::CFramebuffer()
{
	m_FboID = 0;
	m_RboID = 0;
	m_tex = 0;
	m_bIsBegined = false;
	m_width = 0;
	m_height = 0;
	m_curbuff = 0;
}


CFramebuffer::~CFramebuffer()
{
	if (m_bIsBegined)
	{
		end();
		m_bIsBegined = false;
	}
	glDeleteTextures(1, &m_tex);
	//glDeleteRenderbuffersEXT(1, &m_RboID);
	glDeleteRenderbuffers(1, &m_RboID);
	//glDeleteFramebuffersEXT(1, &m_FboID);
	glDeleteFramebuffers(1, &m_FboID);
}
static GLubyte checkImage[textureheight][texturewidth][4];
void CFramebuffer::init(int width, int height)
{
	glewInit();
	m_width = width;
	m_height = height;

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);

	for (int i = 0; i < textureheight; i++) {
		for (int j = 0; j < texturewidth; j++) {
			
			checkImage[i][j][0] = (GLubyte)255;
			checkImage[i][j][1] = (GLubyte)255;
			checkImage[i][j][2] = (GLubyte)255;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_RENDERBUFFER_EXT);
	glGenRenderbuffersEXT(1, &m_RboID);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_RboID);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_width, m_height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	glDisable(GL_RENDERBUFFER_EXT);

	glEnable(GL_FRAMEBUFFER_EXT);
	glGenFramebuffersEXT(1, &m_FboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FboID);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_tex, 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_RboID);


	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		switch (status)
		{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			std::cout << "Framebuffer complete." << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			std::cout << "[ERROR] Unsupported by FBO implementation." << std::endl;
			break;

		default:
			std::cout << "[ERROR] Unknow error." << std::endl;
			break;
		}
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDisable(GL_FRAMEBUFFER_EXT);

	m_curbuff = 0;
}
extern CMyTreeView* pTreeView;
bool CFramebuffer::begin()
{
	if (m_bIsBegined)
	{
		return false;
	}
	else
	{
		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		//glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT,&m_curbuff);
		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_FboID);

		//CRect rect;
		double cx, cy;
		//pTreeView->GetClientRect(&rect);
		//cx = m_width = rect.Width();
		//cy = m_height = rect.Height();
		cx = texturewidth;
		cy = textureheight;

		init(texturewidth, textureheight);

		glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &m_curbuff);
		//glPushAttrib(GL_VIEWPORT_BIT);
		//glMatrixMode(GL_PROJECTION);
		//glPushMatrix();
		//glMatrixMode(GL_MODELVIEW);
		//glPushMatrix();
		/*double aspect = (cy == 0) ? cx : (double)cx / (double)cy;

		glViewport(0, 0, cx, cy);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		gluPerspective(pTreeView->FOVY, aspect, 0.1, 2000);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();*/

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FboID);
		//cout<<"begin after : GL_FRAMEBUFFER_BINDING_EXT = "<<FboId<<endl;
		/*glViewport(0, 0, m_width, m_height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, m_width, 0, m_height, 0.1, 2000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		::glDepthFunc(GL_ALWAYS);
		::glColor3f(1, 0, 0);
		::glPointSize(5.0);
		::glBegin(GL_LINES);
		::glVertex3f(-1,-1, 0);
		::glVertex3f(1, 1, 0);
		::glEnd();
		::glDepthFunc(GL_LESS);*/

		m_bIsBegined = true;
		return true;
	}
}

bool CFramebuffer::end()
{
	if (m_bIsBegined)
	{
		//glPopAttrib();
		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_curbuff);
		//glMatrixMode(GL_MODELVIEW);
		//glPopMatrix();
		//glMatrixMode(GL_PROJECTION);
		//glPopMatrix();
		//glPopAttrib();

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_curbuff);
		m_bIsBegined = false;
		return true;
	}
	else
	{
		return false;
	}
}
using namespace cv;
void CFramebuffer::saveFrameBuff(const char* fileName)
{
	IplImage* pImage = cvCreateImage(cvSize(m_width, m_height), 8, 3);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, pImage->imageData);
	glDisable(GL_TEXTURE_2D);

	cvFlip(pImage, NULL, 0);
	//cvSaveImage(fileName, pImage);
	Mat Img;
	Img = cvarrToMat(pImage);
	cv::imwrite(fileName, Img);
	cvReleaseImage(&pImage);
}