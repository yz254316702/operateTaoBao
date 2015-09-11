#include "StdAfx.h"
#include "TaoBaoToolController.h"
#include "CommandTool.h"

TaoBaoToolController::TaoBaoToolController(void)
{
	m_DevicesName = CommandTool::getDevicesName();
	m_CommandTool.setAndroidDevice(m_DevicesName.GetHead());
}


TaoBaoToolController::~TaoBaoToolController(void)
{
}
