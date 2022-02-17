#include "cBacktrackWindow.h"

wxBEGIN_EVENT_TABLE(cBacktrackWindow, wxFrame)
EVT_COMMAND(evt_id::THREAD_END_ID, wxEVT_THREAD_END, cBacktrackWindow::onThreadEnd)
wxEND_EVENT_TABLE()

cBacktrackWindow::cBacktrackWindow() : AlgorithmFrame(wxString("Backtracking algorithms"), wxPoint(10, 10), wxSize(1600, 1000)) {
	this->SetBackgroundColour(wxColour(0, 0, 0));

	setupToolbar();

	Connect(evt_id::MAP_UPDATE_REQUEST_ID, wxEVT_MAP_UPDATE_REQUEST, wxThreadEventHandler(cBacktrackWindow::cellVisitedUpdate));
	Connect(evt_id::MAP_UNCHECK_REQUEST_ID, wxEVT_MAP_UNCHECK_REQUEST, wxThreadEventHandler(cBacktrackWindow::cellUncheckUpdate));

	frameContent = new wxPanel(this, wxID_ANY, wxDefaultPosition);
	frameContent->SetBackgroundColour(wxColour(43, 201, 231));

	mapButtons = new wxButton * [MAP_ROWS * MAP_COLS];
	setupMap();
}

cBacktrackWindow::~cBacktrackWindow() {
	delete mapButtons;
}

void cBacktrackWindow::setupKnightProblem()
{
	this->ktpImpl = std::make_unique<KTPImpl>(MAP_ROWS, MAP_COLS, (AlgorithmFrame*)this);

	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->SetLabelText(wxString(""));
			mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(wxColour(255, 255, 255));
		}

	cBacktrackWindow::enableMapButtons();
	cBacktrackWindow::enableToolbarButtons();
}

void cBacktrackWindow::runKnightProblem()
{
	if (ktpImpl->getSource() != -1) {
		cBacktrackWindow::assignAlgorithmThread([this]() -> void {this->ktpImpl->runAlgorithm(algorithmThread); });
	}
}

void cBacktrackWindow::knightMapClick(wxButton* button)
{
	const int FIRST_DIM_EQ = button->GetId() - 20000;

	switch (currentMapState) {
	case CHOOSING_SOURCE: {

		cBacktrackWindow::replaceSource(FIRST_DIM_EQ, "");
		ktpImpl->setSource(FIRST_DIM_EQ);
		cBacktrackWindow::setMapState(currentMapState = IDLE);

		break;
	}

	default:
		break;
	}
}

void cBacktrackWindow::setupMap()
{
	wxGridSizer* mapSizer = new wxGridSizer(MAP_ROWS, MAP_COLS, 0, 0);
	wxFont mapFont(15, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {

			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ] = new wxButton(frameContent, 20000 + FIRST_DIM_EQ);
			mapButtons[FIRST_DIM_EQ]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cBacktrackWindow::mapButtonClicked, this);
			mapButtons[FIRST_DIM_EQ]->Bind(wxEVT_RIGHT_DOWN, &cBacktrackWindow::rightButtonClicked, this);
			mapButtons[FIRST_DIM_EQ]->SetFont(mapFont);
			mapSizer->Add(mapButtons[FIRST_DIM_EQ], 1, wxEXPAND | wxALL);
		}

	frameContent->SetSizer(mapSizer);
}

void cBacktrackWindow::setupAlgorithmList()
{
}

void cBacktrackWindow::setupToolbar()
{
	algorithmTools = this->CreateToolBar(wxTB_HORIZONTAL, wxID_ANY);
	algorithmChoice = new wxChoice(algorithmTools, wxID_ANY, wxDefaultPosition, wxSize(400, 500));
	algorithmChoice->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &cBacktrackWindow::choiceSelected, this);

	for (auto& mapType : mapTypeMapping) {
		algorithmChoice->Append(mapTypeDesc[mapType]);
	}

	startButton = new wxButton(algorithmTools, wxID_ANY, "Start", wxDefaultPosition, wxSize(400, 100));
	startButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cBacktrackWindow::onStart, this);

	sourceButton = new wxButton(algorithmTools, wxID_ANY, "Set source", wxDefaultPosition, wxSize(300, 100));
	sourceButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cBacktrackWindow::sourceSetButtonClicked, this);


	algorithmTools->AddControl(startButton);
	algorithmTools->AddStretchableSpace();
	algorithmTools->AddControl(algorithmChoice);
	algorithmTools->AddStretchableSpace();
	algorithmTools->AddControl(sourceButton);
	algorithmTools->Realize();
}

void cBacktrackWindow::onStart(wxCommandEvent& evt)
{
	switch (currentMapState) {
	case IDLE:
		cBacktrackWindow::runAlgorithm();
		break;

	case RUNNING:
		cBacktrackWindow::stopAlgorithm();
		break;
	case FINISHED:
		cBacktrackWindow::resetMap();
		break;
	}
}

void cBacktrackWindow::runAlgorithm()
{
	if (mapSource != -1) {
		mapTypeStart[currentMapType]();

		cBacktrackWindow::setMapState(currentMapState = RUNNING);
		cBacktrackWindow::disableMapButtons();
	}

}

void cBacktrackWindow::stopAlgorithm()
{
	algorithmThread->Delete();
	mapTypeSetup[currentMapType]();

	mapSource = -1;
	mapSourceValue = "";
	cBacktrackWindow::setMapState(currentMapState = IDLE);
	cBacktrackWindow::enableMapButtons();
}

void cBacktrackWindow::resetMap()
{

	mapSource = -1;
	mapSourceValue = "";
	cBacktrackWindow::setMapState(currentMapState = IDLE);
	mapTypeSetup[currentMapType]();
}

void cBacktrackWindow::setMapState(MAP_STATE t_mapState)
{
	switch (t_mapState) {
	case IDLE:
		cBacktrackWindow::enableMapButtons();
		startButton->SetLabelText("Start");
		break;

	case RUNNING:
		cBacktrackWindow::disableMapButtons();
		startButton->SetLabelText("Stop");
		break;

	case CHOOSING_SOURCE:
		break;

	case FINISHED:
		startButton->Enable();
		startButton->SetLabelText("Restart");
		break;
	}
}

void cBacktrackWindow::disableMapButtons()
{
	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->Disable();
		}

	algorithmChoice->Disable();
	sourceButton->Disable();
}

void cBacktrackWindow::enableMapButtons()
{
	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->Enable();
		}

	algorithmChoice->Enable();
	sourceButton->Enable();
}

void cBacktrackWindow::enableToolbarButtons()
{
	algorithmChoice->Enable();
	sourceButton->Enable();
}

void cBacktrackWindow::disableToolbarButtons()
{
	algorithmChoice->Disable();
	sourceButton->Disable();
}

void cBacktrackWindow::replaceSource(const int t_newSource, const std::string t_newSourceValue)
{

	if (mapSource != -1) {
		mapButtons[mapSource]->SetBackgroundColour(wxColour(255, 255, 255));
		mapButtons[mapSource]->SetLabelText(mapSourceValue);
	}

	mapButtons[t_newSource]->SetBackgroundColour(wxColour(153, 255, 255));
	mapButtons[t_newSource]->SetLabelText("Source");

	mapSource = t_newSource;
	mapSourceValue = t_newSourceValue;

}



void cBacktrackWindow::assignAlgorithmThread(const std::function<void()>& runFunction)
{
	algorithmThread = new AlgorithmThread(this, runFunction);
	algorithmThread->Create();
	algorithmThread->Run();
}

void cBacktrackWindow::onThreadEnd(wxCommandEvent& evt)
{
	cBacktrackWindow::setMapState(currentMapState = FINISHED);
}
void cBacktrackWindow::onThreadBreak(wxCommandEvent& evt) {

}

void cBacktrackWindow::choiceSelected(wxCommandEvent& evt)
{
	wxChoice* currentChoice = (wxChoice*)evt.GetEventObject();
	int selected = currentChoice->GetCurrentSelection();

	MAP_TYPE selectedMapType = mapTypeMapping[selected];

	currentMapType = selectedMapType;

	mapTypeSetup[currentMapType]();
}

void cBacktrackWindow::mapButtonClicked(wxCommandEvent& evt)
{
	wxButton* mapButtonClicked = (wxButton*)evt.GetEventObject();

	mapTypeOnClick[currentMapType](mapButtonClicked);
}

void cBacktrackWindow::rightButtonClicked(wxMouseEvent& evt)
{

}


void cBacktrackWindow::sourceSetButtonClicked(wxCommandEvent& evt)
{
	cBacktrackWindow::setMapState(currentMapState = CHOOSING_SOURCE);
}

void cBacktrackWindow::cellVisitedUpdate(wxThreadEvent& evt)
{

	def_type::CELL_UPDATE_INFO NEW_DATA = evt.GetPayload<def_type::CELL_UPDATE_INFO>();

	int FIRST_DIM_EQ = NEW_DATA.FIRST_DIM_EQ;
	int newValue = NEW_DATA.newValue;
	wxColour newColour = NEW_DATA.newColour;

	cBacktrackWindow::updateCellColor(FIRST_DIM_EQ, newColour);
	cBacktrackWindow::updateCellValue(FIRST_DIM_EQ, wxString(std::to_string(newValue)));


}
void cBacktrackWindow::cellUncheckUpdate(wxThreadEvent& evt) {

	def_type::CELL_UPDATE_INFO NEW_DATA = evt.GetPayload<def_type::CELL_UPDATE_INFO>();

	int FIRST_DIM_EQ = NEW_DATA.FIRST_DIM_EQ;
	int newValue = NEW_DATA.newValue;
	wxColour newColour = NEW_DATA.newColour;

	cBacktrackWindow::updateCellColor(FIRST_DIM_EQ, newColour);
	cBacktrackWindow::updateCellValue(FIRST_DIM_EQ, wxString(""));

}

void cBacktrackWindow::updateCellColor(const int& FIRST_DIM_EQ, wxColour const& t_newColour)
{
	mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(t_newColour);
}

void cBacktrackWindow::updateCellValue(const int& FIRST_DIM_EQ, wxString const& t_newValue)
{
	mapButtons[FIRST_DIM_EQ]->SetLabelText(t_newValue);
}
