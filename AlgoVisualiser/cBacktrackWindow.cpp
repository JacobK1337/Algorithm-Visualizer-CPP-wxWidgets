#include "cBacktrackWindow.h"

wxBEGIN_EVENT_TABLE(cBacktrackWindow, wxFrame)
EVT_COMMAND(evt_id::THREAD_END_ID, wxEVT_THREAD_END, cBacktrackWindow::onThreadEnd)
EVT_CLOSE(cBacktrackWindow::OnClose)
wxEND_EVENT_TABLE()

cBacktrackWindow::cBacktrackWindow() : AlgorithmFrame(wxString("Backtracking algorithms"), wxPoint(10, 10), wxSize(1600, 1000)) {

	cBacktrackWindow::applyMapConfig();
	cBacktrackWindow::setupToolbar();
	Connect(evt_id::MAP_UPDATE_REQUEST_ID, wxEVT_MAP_UPDATE_REQUEST, wxThreadEventHandler(cBacktrackWindow::cellUpdate));
}

void cBacktrackWindow::applyMapConfig()
{
	MapConfig KNIGHT_CONFIG = { 5, 5, "Knight's tour problem" };
	MapConfig SUDOKU_CONFIG = { 9, 9, "Sudoku Solver" };
	MapConfig RAT_CONFIG = { 7, 7, "Rat in a maze problem" };

	mapConfig.insert(std::make_pair(KNIGHT, KNIGHT_CONFIG));
	mapConfig.insert(std::make_pair(SUDOKU, SUDOKU_CONFIG));
	mapConfig.insert(std::make_pair(RAT, RAT_CONFIG));
}

cBacktrackWindow::~cBacktrackWindow()
{
	delete mapButtons;
}

void cBacktrackWindow::setupKnightProblem()
{
	ktpImpl = std::make_unique<KTPImpl>(mapConfig[KNIGHT].MAP_ROWS, mapConfig[KNIGHT].MAP_COLS, (AlgorithmFrame*)this);
	ktpImpl->generateValues(algorithmThread);
}

void cBacktrackWindow::runKnightProblem()
{
	if (mapSource != -1) {
		cBacktrackWindow::setMapState(currentMapState = RUNNING);
		ktpImpl->setSource(mapSource);
		cBacktrackWindow::assignAlgorithmThread([this]() -> void {this->ktpImpl->runAlgorithm(algorithmThread); });
	}
}

void cBacktrackWindow::setupSudokuSolver()
{
	sudokuSolver = std::make_unique<SudokuSolver>(mapConfig[SUDOKU].MAP_ROWS, mapConfig[SUDOKU].MAP_COLS, (AlgorithmFrame*)this);
	sudokuSolver->generateValues(algorithmThread);
}

void cBacktrackWindow::runSudokuSolver()
{
	cBacktrackWindow::setMapState(currentMapState = RUNNING);
	cBacktrackWindow::assignAlgorithmThread([this]() -> void {this->sudokuSolver->runAlgorithm(algorithmThread); });
}

void cBacktrackWindow::setupRim()
{
	rimImpl = std::make_unique<RIMimpl>(mapConfig[RAT].MAP_ROWS, mapConfig[RAT].MAP_COLS, (AlgorithmFrame*)this);
	rimImpl->generateValues(algorithmThread);
}

void cBacktrackWindow::runRim()
{
	cBacktrackWindow::setMapState(currentMapState = RUNNING);
	cBacktrackWindow::assignAlgorithmThread([this]() -> void {this->rimImpl->runAlgorithm(algorithmThread); });
}

void cBacktrackWindow::setupMap()
{
	if (frameContent != nullptr) {
		frameContent->Destroy();
		delete mapButtons;
	}

	frameContent = new wxPanel(this, wxID_ANY, wxDefaultPosition);
	frameContent->SetBackgroundColour(wxColour(255, 255, 255));

	int t_MAP_ROWS = mapConfig[currentMapType].MAP_ROWS;
	int t_MAP_COLS = mapConfig[currentMapType].MAP_COLS;
	mapSource = -1;

	mapButtons = new wxButton * [t_MAP_ROWS * t_MAP_COLS];
	wxGridSizer* mapSizer = new wxGridSizer(t_MAP_ROWS, t_MAP_COLS, 0, 0);
	wxFont mapFont(15, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int i = 0; i < t_MAP_ROWS; i++)
		for (int j = 0; j < t_MAP_COLS; j++) {

			const int FIRST_DIM_EQ = i * t_MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ] = new wxButton(frameContent, 20000 + FIRST_DIM_EQ);
			mapButtons[FIRST_DIM_EQ]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cBacktrackWindow::mapButtonClicked, this);
			mapButtons[FIRST_DIM_EQ]->Bind(wxEVT_RIGHT_DOWN, &cBacktrackWindow::rightButtonClicked, this);
			mapButtons[FIRST_DIM_EQ]->SetFont(mapFont);
			mapSizer->Add(mapButtons[FIRST_DIM_EQ], 1, wxEXPAND | wxALL);
		}

	frameContent->SetSizer(mapSizer);

	this->InvalidateBestSize();
	this->Layout();
}

void cBacktrackWindow::setupToolbar()
{
	wxFont toolbarFont(15, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);


	algorithmTools = this->CreateToolBar(wxTB_HORIZONTAL, wxID_ANY);
	algorithmTools->SetBackgroundColour(wxColour(255, 255, 255));

	algorithmChoice = new wxChoice(algorithmTools, wxID_ANY, wxDefaultPosition, wxSize(400, 500));
	algorithmChoice->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &cBacktrackWindow::choiceSelected, this);
	algorithmChoice->SetFont(toolbarFont);

	for (MAP_TYPE type = MAP_TYPE(MAP_TYPE_START + 1); type < MAP_TYPE_END; type = MAP_TYPE(type + 1)) {
		algorithmChoice->Append(mapConfig[type].MAP_TITLE);
	}

	startButton = new wxButton(algorithmTools, wxID_ANY, "Start", wxDefaultPosition, wxSize(400, 100));
	startButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cBacktrackWindow::onStart, this);
	startButton->SetFont(toolbarFont);
	startButton->SetBackgroundColour(wxColour(153, 204, 0));

	sourceButton = new wxButton(algorithmTools, wxID_ANY, "Set source", wxDefaultPosition, wxSize(300, 100));
	sourceButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cBacktrackWindow::sourceSetButtonClicked, this);
	sourceButton->SetFont(toolbarFont);
	sourceButton->SetBackgroundColour(wxColour(255, 255, 0));

	//adding all controls
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
void cBacktrackWindow::mapButtonClicked(wxCommandEvent& evt)
{

	wxButton* mapButtonClicked = (wxButton*)evt.GetEventObject();
	const int FIRST_DIM_EQ = mapButtonClicked->GetId() - 20000;

	switch (currentMapState) {
	case CHOOSING_SOURCE:
		cBacktrackWindow::replaceSource(FIRST_DIM_EQ, (std::string)mapButtonClicked->GetLabelText());
		cBacktrackWindow::setMapState(currentMapState = IDLE);
		break;

	default:
		break;
	}
}
void cBacktrackWindow::runAlgorithm()
{
	mapTypeStart[currentMapType]();
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
		cBacktrackWindow::enableToolbarButtons();
		startButton->SetLabelText("Start");
		break;

	case RUNNING:
		cBacktrackWindow::disableToolbarButtons();
		cBacktrackWindow::disableMapButtons();
		startButton->SetLabelText("Stop");
		break;

	case CHOOSING_SOURCE:
		break;

	case FINISHED:
		cBacktrackWindow::enableMapButtons();
		startButton->SetLabelText("Restart");
		break;
	}
}

void cBacktrackWindow::disableMapButtons()
{
	for (int i = 0; i < mapConfig[currentMapType].MAP_ROWS; i++)
		for (int j = 0; j < mapConfig[currentMapType].MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * mapConfig[currentMapType].MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->Disable();
		}
}

void cBacktrackWindow::enableMapButtons()
{
	for (int i = 0; i < mapConfig[currentMapType].MAP_ROWS; i++)
		for (int j = 0; j < mapConfig[currentMapType].MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * mapConfig[currentMapType].MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->Enable();
		}
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

	if (t_newSource != mapSource) {
		mapSource = t_newSource;
		mapSourceValue = t_newSourceValue;
	}

}

// none of the algorithms require setting the destination.
void cBacktrackWindow::replaceDest(const int t_newDest, const std::string t_newDestValue) {

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

void cBacktrackWindow::onThreadBreak(wxCommandEvent& evt)
{
	cBacktrackWindow::setMapState(currentMapState = IDLE);
}

void cBacktrackWindow::choiceSelected(wxCommandEvent& evt)
{
	wxChoice* currentChoice = (wxChoice*)evt.GetEventObject();
	int selected = currentChoice->GetCurrentSelection();

	MAP_TYPE selectedMapType = MAP_TYPE(MAP_TYPE_START + selected + 1);

	currentMapType = selectedMapType;

	setupMap();
	mapTypeSetup[currentMapType]();

	cBacktrackWindow::enableMapButtons();
	cBacktrackWindow::enableToolbarButtons();
}

void cBacktrackWindow::rightButtonClicked(wxMouseEvent& evt)
{
}

void cBacktrackWindow::sourceSetButtonClicked(wxCommandEvent& evt)
{
	cBacktrackWindow::setMapState(currentMapState = CHOOSING_SOURCE);
}

void cBacktrackWindow::cellUpdate(wxThreadEvent& evt)
{

	def_type::CELL_UPDATE_INFO NEW_DATA = evt.GetPayload<def_type::CELL_UPDATE_INFO>();

	int FIRST_DIM_EQ = NEW_DATA.FIRST_DIM_EQ;
	std::string newValue = NEW_DATA.newValue;
	wxColour newColour = NEW_DATA.newColour;

	cBacktrackWindow::updateCellColor(FIRST_DIM_EQ, newColour);
	cBacktrackWindow::updateCellValue(FIRST_DIM_EQ, wxString(newValue));

}

void cBacktrackWindow::updateCellColor(const int& FIRST_DIM_EQ, wxColour const& t_newColour)
{
	mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(t_newColour);
}

void cBacktrackWindow::updateCellValue(const int& FIRST_DIM_EQ, wxString const& t_newValue)
{
	mapButtons[FIRST_DIM_EQ]->SetLabelText(t_newValue);
}

void cBacktrackWindow::OnClose(wxCloseEvent& evt) {
	//if window is closed while algorithm is running, then delete it. We must wait until the worker thread catches the event.
	if (currentMapState == RUNNING) {

		algorithmThread->Delete();

		//wait some time to let the thread close.
		wxMilliSleep(1000);
	}

	//wait till thread finishes action

	//as default destroy the window.
	Destroy();
}
