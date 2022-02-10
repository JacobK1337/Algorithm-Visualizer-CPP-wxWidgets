#pragma once
#include<wx/wx.h>
using namespace std;

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(wxEVT_MYTHREAD, -1)
END_DECLARE_EVENT_TYPES()

class DijkstraThread : public wxThread
{
public:
	DijkstraThread(wxEvtHandler* parent, function<void()> functionEvent);
	void* Entry();

protected:
	wxEvtHandler* pParent;
	function<void()> functionEvent;
};

