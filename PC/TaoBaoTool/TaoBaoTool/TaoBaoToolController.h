#pragma once
class TaoBaoToolController
{
public:
	TaoBaoToolController(void);
	~TaoBaoToolController(void);
private:
	CList<CString>  m_DevicesName;
	CommandTool m_CommandTool;
};

