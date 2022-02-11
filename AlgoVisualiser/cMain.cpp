#include "cMain.h"


wxBEGIN_EVENT_TABLE(cMain, wxFrame)
wxEND_EVENT_TABLE()


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Algorithm Visualiser", wxPoint(30, 30), wxSize(800, 600)) {

	initFrames();

	panelContent = new wxPanel(this, wxID_ANY);
	panelContent->SetBackgroundColour(wxColour(255, 102, 231));

	const wxFont buttonFont(20, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	const wxFont headerFont(40, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

	wxBoxSizer* categoryButtonSizer = new wxBoxSizer(wxVERTICAL);

	categoryButtonSizer->AddStretchSpacer();
	welcomeHeader = new wxStaticText(panelContent, wxID_ANY, "Algorithm visualiser");
	welcomeHeader->SetFont(headerFont);

	categoryButtonSizer->Add(welcomeHeader, 0, wxALL | wxALIGN_CENTER, 50);

	const unsigned int ALGO_CATEGORY_NUM = ALGO_FRAMES.size();
	algoCategoryButtons = new wxButton * [ALGO_CATEGORY_NUM];
	
	for (int i = 0; i < ALGO_CATEGORY_NUM; i++)
	{
		algoCategoryButtons[i] = new wxButton(panelContent, 10000 + i, ALGO_CATEGORIES[i]);
		algoCategoryButtons[i]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnCategoryButtonClick, this);
		algoCategoryButtons[i]->SetFont(buttonFont);

		categoryButtonSizer->Add(algoCategoryButtons[i], 0, wxALL | wxALIGN_CENTER, 20);
	}
	categoryButtonSizer->AddStretchSpacer();

	panelContent->SetSizer(categoryButtonSizer);
}


void cMain::OnCategoryButtonClick(wxCommandEvent& evt) {
	
	const int ID = evt.GetId() - 10000;
	ALGO_FRAMES[ID]()->Show();
}


void cMain::initFrames() {
	
	
	// Pathfinding frame
	ALGO_FRAMES.push_back([this]() ->
		wxFrame* {return (wxFrame*) new cPathFindWindow(); });

	ALGO_CATEGORIES.push_back("Pathfinding algorithms");

	// Backtracking frame
	ALGO_FRAMES.push_back([this]() ->
		wxFrame* {return (wxFrame*) new cBacktrackWindow(); });

	ALGO_CATEGORIES.push_back("Backtracking algorithms");

}

cMain::~cMain() {
	delete algoCategoryButtons;
}

