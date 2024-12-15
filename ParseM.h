#pragma once

class CParseM
{
public:
	CParseM(void);
	~CParseM(void);
public:
	void Free(void);
	int Run(char *filename,CSceneGraph3d *pSceneGraph);
	CString m_FileName;
};
