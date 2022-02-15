#pragma once
#include<wx/wx.h>
#include"declared_events.h"

class SideThread : public wxThread
{
public:
	SideThread(wxEvtHandler* parent, std::function<void()> functionEvent);
	void* Entry();

protected:
	wxEvtHandler* pParent;
	std::function<void()> functionEvent;

};

