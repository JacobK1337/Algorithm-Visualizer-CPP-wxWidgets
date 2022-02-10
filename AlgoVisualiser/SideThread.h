#pragma once
#include<wx/wx.h>
using namespace std;

//wxDECLARE_EVENT(wxEVT_THREAD_END, wxCommandEvent);
DECLARE_EVENT_TYPE(wxEVT_THREAD_END, -1)
class SideThread : public wxThread
{
public:
	SideThread(wxEvtHandler* parent, function<void()> functionEvent);
	void* Entry();

protected:
	wxEvtHandler* pParent;
	function<void()> functionEvent;

private:
	const int END_THREAD_ID = 5555;
};

