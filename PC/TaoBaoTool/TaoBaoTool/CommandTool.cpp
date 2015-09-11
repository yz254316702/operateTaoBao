#include "StdAfx.h"
#include "CommandTool.h"


CommandTool::CommandTool(char * name)
{
	if (name == NULL)
	{
		return;
	}
	m_adb_device_name = new char[strlen(name)+1];
	if (m_adb_device_name != NULL)
	{
		memcpy(m_adb_device_name,name,strlen(name)+1);
	}
}

CommandTool::CommandTool()
{
	m_adb_device_name = NULL;
}

CommandTool::~CommandTool(void)
{
	if (m_adb_device_name!=NULL)
	{
		delete[] m_adb_device_name;
	}
}

CString CommandTool::GetWorkDir()
{
	WCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory( MAX_PATH, pFileName);

	CString csFullPath(pFileName);  
	if( nPos < 0 )
	{
		return CString("");
	}
	else
	{
	return csFullPath;
	}

}

void CommandTool::setAndroidDefaultInputMethod( CString m )
{

	CString cmd("adb -s %s shell ime set %s");
	cmd.Format(cmd,m_adb_device_name,m);
	execCmd(cmd	);
}

void CommandTool::simulateKey( int keyValue )
{
	CString cmd("adb -s %s shell input keyevent  %d");
	cmd.Format(cmd,m_adb_device_name,keyValue);
	execCmd(cmd);
}

void CommandTool::simulateClik( int paramInt1, int paramInt2)
{
	CString cmd("adb -s %s shell input tap %d  %d");
	cmd.Format(cmd,m_adb_device_name,paramInt1,paramInt2);
}

int CommandTool::startApp( CString package, CString classname )
{
	CString cmd("adb -s %s shell am start -n %s%s");
	cmd.Format(cmd,m_adb_device_name,package,classname);
	execCmd(cmd);
}

int CommandTool::stopApp( CString p )
{
	CString cmd("adb -s %s shell am force-stop %s ");
	cmd.Format(cmd,m_adb_device_name,p);
	execCmd(cmd);
}

void CommandTool::swipe( int paramInt1, int paramInt2, int paramInt3, int paramInt4 )
{
	CString cmd("adb -s %s shell input swipe %d %d %d %d");
	cmd.Format(cmd,m_adb_device_name,paramInt1,paramInt2,paramInt3,paramInt4);
	execCmd(cmd);
}

void CommandTool::typeRawText( CString value )
{
	CString cmd("adb  -s %s shell input text %s");
	cmd.Format(cmd,m_adb_device_name,value);
	execCmd(cmd);
}

void CommandTool::refreshView()
{
	CString cmd;
	cmd = "adb -s %s shell rm %s";
	cmd.Format(cmd,m_adb_device_name,Window_Path_Android);
	execCmd(cmd);//delete xml file

	cmd = "adb -s %s shell uiautomator dump %s";
	cmd.Format(cmd,m_adb_device_name,Window_Path_Android);
	execCmd(cmd);//create xml file

	cmd = "adb -s %s pull %s %s";
	cmd.Format(cmd,m_adb_device_name,Window_Path_Android,Window_Path_PC);
	execCmd(cmd);

}

void CommandTool::clearPackageData(CString pck)
{
	CString cmd("adb -s %s shell pm clear %s");
	cmd.Format(cmd,m_adb_device_name,pck);
	execCmd(cmd);
}

CString CommandTool::getAndroidInputMethod()
{
	CString cmd("adb -s %s shell ime list -s");
	cmd.Format(cmd,m_adb_device_name);
	return execCmd(cmd);
}

CString CommandTool::execCmd( CString str )
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead,&hWrite,&sa,0))
	{
		MessageBox("Error on CreatePipe()!");
		return NULL;
	}
	STARTUPINFO si={sizeof(si)};
	PROCESS_INFORMATION pi;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	TCHAR* cmdline=StringToChar(str);
	if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))
	{
		MessageBox("Error on CreateProcess()!");
		return NULL;
	}
	CloseHandle(hWrite);

	char buffer[4096];
	memset(buffer,0,4096);
	CString output;
	DWORD byteRead;
	while(true)
	{
		if (ReadFile(hRead,buffer,4095,&byteRead,NULL) == NULL)
		{
			break;
		}
		output += buffer;
	}
	return output;
}

TCHAR* CommandTool::StringToChar(CString& str)
{
	int len=str.GetLength();
	TCHAR* tr=str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr; 
}

bool CommandTool::packageisInstalled( CString pck )
{
	CString cmd("adb -s %s shell pm path %s");
	cmd.Format(cmd,m_adb_device_name,pck);
	CString ret=execCmd(cmd);
	if (ret.IsEmpty() || ret.GetLength() == 0)
	{
		return false;
	}
	return true;
}

CString CommandTool::getAndroidSysInfo( CString key )
{
	CString cmd("adb -s %s shell getprop %s");
	cmd.Format(cmd,m_adb_device_name,key);
	return execCmd(cmd);
}

CString CommandTool::sendBroadcase2AndroidDevices( CString action,CString stringV, CString boolV )
{
	CString result;
	if (action.IsEmpty()||action.GetLength()==0)
	{
		result="action is empty";
		return result;
	}
	CString cmd("adb -s %s shell am broadcast -a %s ");
	cmd.Format(cmd,m_adb_device_name,action);
	if (false == (stringV.IsEmpty()||stringV.GetLength()==0))
	{
		cmd +=" --es string ";
		cmd +=stringV;
	}
	if (false == (boolV.IsEmpty()||boolV.GetLength()==0))
	{
		cmd +=" --ez boolean ";
		cmd +=boolV;
	}
	result = execCmd(cmd);
	return result;
}

CString CommandTool::installAPK(CString path)
{
	CString cmd("adb -s %s shell install -r %s");
	cmd.Format(cmd,m_adb_device_name,path);
	return execCmd(cmd);
}

void CommandTool::setAndroidDevice( char* name )
{
	if (m_adb_device_name!=NULL)
	{
		delete[] m_adb_device_name;
	}
	m_adb_device_name = new char[strlen(name)+1];
	if (m_adb_device_name != NULL)
	{
		memcpy(m_adb_device_name,name,strlen(name)+1);
	}
	
}

CString CommandTool::getDevicesName()
{

}

int CommandTool::getDevicesCount()
{

}
