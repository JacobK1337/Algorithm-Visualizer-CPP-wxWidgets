#pragma once

#include<wx/wx.h>
#include"cMain.h"


DEFINE_EVENT_TYPE(wxEVT_MAP_UPDATE_REQUEST)
DEFINE_EVENT_TYPE(wxEVT_MAP_RECON_REQUEST)

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

