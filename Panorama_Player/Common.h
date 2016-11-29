#ifndef __COMMON_H__
#define __COMMON_H__

#define LOG_FILE "./Log/my_Panorama_Player.log" 

#define OutputLogLen 1024


char * CString_to_charX(CString str);
void OutputLog(char * msg);


/*
class ShowLogToEdit{
public:
	ShowLogToEdit(void);
	~ShowLogToEdit(void);

	void SetEdit(CEdit* p)
	{
		m_editFont.CreatePointFont(100, (LPCTSTR)"ºÚÌå");
		pLogEdit = p;
		pLogEdit->SetFont(&m_editFont);
	}
	int ShowTimeToEdit();
	int ShowStrToEdit(char * str,int len);
	int LogEditEnt();
	int StopShowLog;

private:
	CFont m_editFont;
	CCriticalSection ShowLoglock;
	CEdit* pLogEdit;
};
*/
//ShowLogToEdit GetLogEdit();
#endif