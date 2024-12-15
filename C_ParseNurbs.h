
#ifndef _PARSE_NURBS_
#define _PARSE_NURBS_
//C_ParseNurbs.h
class CParseNurbs  
{
public:
	void Free(void);
	int Run(char *filename,CSceneGraph3d *pSceneGraph);
	CParseNurbs();
	virtual ~CParseNurbs();
	CString m_FileName;

};
#endif