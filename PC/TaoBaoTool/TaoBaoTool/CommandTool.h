#pragma once
#define Window_Path_Android " /data/winDump.xml "
#define Window_Path_PC " c:\\winDump.xml "
class CommandTool
{
public:
	CommandTool(void);
	~CommandTool(void);

	void setAndroidDefaultInputMethod(CString m);
	void simulateKey(int keyValue);
	void simulateClik(int paramInt1, int paramInt2, int paramInt3, int paramInt4);
	int startApp(CString package, CString classname);
	int stopApp(CString p);
	void swipe(int paramInt1, int paramInt2, int paramInt3, int paramInt4);//scroll
	void typeRawText(CString value);
	void refreshView();
	void clearPackageData(CString pck);
	bool packageisInstalled(CString pck);
	CString getAndroidSysInfo(CString key);
private:
	CString getAndroidInputMethod();
	CString execCmd(CString cmd);
	CString GetWorkDir();
	TCHAR* StringToChar(CString& str);
};

