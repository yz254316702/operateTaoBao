#include "StdAfx.h"
#include "CommandTool.h"


CommandTool::CommandTool(void)
{
}


CommandTool::~CommandTool(void)
{
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
	CString cmd("adb shell ime set ");
	cmd += m;
	execCmd(cmd	);
}

void CommandTool::simulateKey( int keyValue )
{
	CString cmd("adb shell input keyevent  ");
	CString format("%d ");
	cmd.AppendFormat(format,keyValue);
	execCmd(cmd);
}

void CommandTool::simulateClik( int paramInt1, int paramInt2, int paramInt3, int paramInt4 )
{

}

int CommandTool::startApp( CString package, CString classname )
{
	CString cmd("adb shell am start -n  ");
	cmd += package;
	cmd += classname;
	execCmd(cmd);
}

int CommandTool::stopApp( CString p )
{
	CString cmd("adb shell am force-stop  ");
	cmd += p;
	execCmd(cmd);
}

void CommandTool::swipe( int paramInt1, int paramInt2, int paramInt3, int paramInt4 )
{
	CString cmd("adb shell input swipe ");
	CString format("%d %d %d %d");
	cmd.AppendFormat(format,paramInt1,paramInt2,paramInt3,paramInt4);
	execCmd(cmd);
}

void CommandTool::typeRawText( CString value )
{
	CString cmd("adb shell input text ");
	cmd += value;
	execCmd(cmd);
}

void CommandTool::refreshView()
{
	CString cmd;
	cmd = "adb shell rm ";
	cmd += Window_Path_Android;
	execCmd(cmd);//delete xml file

	cmd = "adb shell uiautomator dump ";
	cmd += Window_Path_Android;
	execCmd(cmd);//create xml file

	cmd = "adb pull ";
	cmd += Window_Path_Android;
	cmd += Window_Path_PC;
	execCmd(cmd);

}

void CommandTool::clearPackageData(CString pck)
{
	CString cmd("adb shell pm clear ");
	cmd +=pck;
	execCmd(cmd);
}

CString CommandTool::getAndroidInputMethod()
{
	CString cmd("adb shell ime list -s");
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
	CString cmd("adb shell pm path ");
	cmd += pck;
	CString ret=execCmd(cmd);
	if (ret.IsEmpty() || ret.GetLength() == 0)
	{
		return false;
	}
	return true;
}

CString CommandTool::getAndroidSysInfo( CString key )
{
	CString cmd("adb shell getprop ");
	cmd += key;
	return execCmd(cmd);
}
