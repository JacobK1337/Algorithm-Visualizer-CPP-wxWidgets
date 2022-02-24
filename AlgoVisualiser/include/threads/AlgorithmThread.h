#pragma once
#include<wx/wx.h>
#include"../side_functions/declared_events.h"

class AlgorithmThread : public wxThread
{
public:
	AlgorithmThread(wxEvtHandler* parent, std::function<void()> functionEvent);
	ExitCode Entry();
	void flagThreadBreak(const bool& statement);

protected:
	wxEvtHandler* pParent;
	std::function<void()> functionEvent;

	volatile bool threadBreak = false;

};

