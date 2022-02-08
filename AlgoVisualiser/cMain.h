#pragma once
#include<wx/wx.h>
#include<string>
#include<vector>
#include "cPathFindWindow.h"
#include "cBacktrackWindow.h"
using namespace std;


class cMain : public wxFrame
{
private:


	vector<string> ALGO_CATEGORIES;
	vector<function <wxFrame* ()> > ALGO_FRAMES;

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