#pragma once
#define Window_Path_Android " /data/winDump.xml "
#define Window_Path_PC " c:\\winDump.xml "
class CommandTool
{
public:
	CommandTool(char * deviceName);
	CommandTool();
	~CommandTool(void);
	void setAndroidDevice(char* name);
	void setAndroidDefaultInputMethod(CString m);
	void simulateKey(int keyValue);
	void simulateClik(int paramInt1, int paramInt2);
	int startApp(CString package, CString classname);
	int stopApp(CString p);
	void swipe(int paramInt1, int paramInt2, int paramInt3, int paramInt4);//scroll
	void typeRawText(CString value);
	void refreshView();
	void clearPackageData(CString pck);
	bool packageisInstalled(CString pck);
	CString getAndroidSysInfo(CString key);
	CString sendBroadcase2AndroidDevices(CString action,CString stringV, CString boolV);
	static CString getDevicesName();
	static int getDevicesCount();
	CString installAPK(CString path);
private:
	char* m_adb_device_name;
	CString getAndroidInputMethod();
	CString execCmd(CString cmd);
	CString GetWorkDir();
	TCHAR* StringToChar(CString& str);
};

