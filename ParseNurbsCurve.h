#pragma once
class CSceneGraph3d;;
class CParseNurbsCurve
{
public:
	void Free(void);
	int Run(char *filename,CSceneGraph3d *pSceneGraph);
	CParseNurbsCurve();
	virtual ~CParseNurbsCurve();
	CString m_FileName;
};
