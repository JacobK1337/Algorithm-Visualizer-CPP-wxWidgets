#pragma once

#include<wx/wx.h>
#include"../frames/cMain.h"


DEFINE_EVENT_TYPE(wxEVT_MAP_UPDATE_REQUEST)
DEFINE_EVENT_TYPE(wxEVT_THREAD_END)
DEFINE_EVENT_TYPE(wxEVT_THREAD_BREAK)

class cApp : public wxApp
{
private:
	cMain* mainFrame = nullptr;

public:
	cApp();
	~cApp();

public:
	virtual bool OnInit();
};

