#pragma once

#include<wx/wx.h>
#include"cMain.h"


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

