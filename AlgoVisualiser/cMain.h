#pragma once
#include<wx/wx.h>
#include<string>
#include<vector>
#include "cPathFindWindow.h"
#include "cBacktrackWindow.h"


class cMain : public wxFrame
{
private:


	std::vector<std::string> ALGO_CATEGORIES;
	std::vector<std::function <wxFrame* ()> > ALGO_FRAMES;

	wxPanel* panelContent = nullptr;
	wxStaticText* welcomeHeader = nullptr;
	wxButton** algoCategoryButtons = nullptr;

	void initFrames();

public:
	cMain();
	~cMain();

	void OnCategoryButtonClick(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};