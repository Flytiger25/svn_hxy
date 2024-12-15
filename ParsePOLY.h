#pragma once

class CParsePOLY
{
public:
	CParsePOLY(void);
	~CParsePOLY(void);

	void Free(void);

	//Parse PSLG
	int Run_PSLG(char *filename,CSceneGraph3d *pSceneGraph);

	//Parse Elements
	int Run_Element(char *filename,CSceneGraph3d *pSceneGraph);
	CString m_FileName;
};
