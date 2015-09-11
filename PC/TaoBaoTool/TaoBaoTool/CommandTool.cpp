#include "StdAfx.h"
#include "CommandTool.h"
#include "Base64.h"

CommandTool::CommandTool()
{
	m_adb_device_name="";
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
	//CString cmd("adb  -s %s shell input text %s");
	unsigned char * tmp = (unsigned char*)value.GetBuffer();
	std::string stmp = base64_encode(tmp,value.GetLength()*2);
	value.ReleaseBuffer();
	//cmd.Format(cmd,m_adb_device_name,stmp.c_str());
	//execCmd(cmd);
	CString action("com.owenyi.input.INPUTSTRING");
	CString stringV(stmp.c_str());
	CString boolV("false");
	sendBroadcase2AndroidDevices(action,stringV,boolV);
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

void CommandTool::setAndroidDevice( CString name )
{
	m_adb_device_name = name;
}

CList<CString> CommandTool::getDevicesName()
{
	CList<CString> results;
	CList<CString> results1;
	CString cmd("adb devices");
	CString result = execCmd(cmd);
	CString split("\n");
	CString split1(" ");
	splitString(result,split,&results,false);
	if (results.IsEmpty() || results.GetCount() <= 1)
	{
		return results1;
	}
	int i = 1;
	while (i < results.GetCount())
	{
		 POSITION pos = results.FindIndex(i);
		splitString(results.GetAt(pos),split1,&results1,true); 
	}
	return results1;
}


CString CommandTool::getAppInfo()
{

}

 void CommandTool::splitString(CString s, CString subs ,CList<CString> *result,bool bAddOne)
{
	if (result == NULL)
	{
		return ;
	}
	int n = s.Find(subs);
	if (n<=0)
	{
		result->AddTail(s);
		return ;
	}
	result->AddTail(s.Left(n));
	if (bAddOne)
	{
		return;
	}
	int length=s.Delete(0,n); //找出","右边的字符串位数
	if (length <= 0)
	{
		return;
	}
	s = s.Right(length);
	splitString(s,subs,result);
}
