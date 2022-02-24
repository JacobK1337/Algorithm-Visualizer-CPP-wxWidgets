#include "../../include/frames/cPathFindWindow.h"

wxBEGIN_EVENT_TABLE(cPathFindWindow, wxFrame)
EVT_COMMAND(evt_id::THREAD_END_ID, wxEVT_THREAD_END, cPathFindWindow::onThreadEnd)
EVT_COMMAND(evt_id::THREAD_BREAK_ID, wxEVT_THREAD_BREAK, cPathFindWindow::onThreadBreak)
EVT_CLOSE(cPathFindWindow::OnClose)
wxEND_EVENT_TABLE()

cPathFindWindow::cPathFindWindow() : AlgorithmFrame(wxString("Pathfinding algorithms"), wxPoint(10, 10), wxSize(1600, 1000)) {

	cPathFindWindow::applyMapConfig();
	cPathFindWindow::setupToolbar();
	Connect(evt_id::MAP_UPDATE_REQUEST_ID, wxEVT_MAP_UPDATE_REQUEST, wxThreadEventHandler(cPathFindWindow::cellUpdate));

}

cPathFindWindow::~cPathFindWindow() {
	delete mapButtons;
}

void cPathFindWindow::applyMapConfig() {
	MapConfig DIJKSTRA_CONFIG = { 15, 15, "Dijkstra shortest path" };
	MapConfig DFS_CONFIG = { 15, 15, "Depth-First search" };
	MapConfig BFS_CONFIG = { 15, 15, "Breadth-First search" };
	MapConfig ASTAR_CONFIG = { 30, 30, "A* Pathfinding algorithm" };

	mapConfig.insert(std::make_pair(DIJKSTRA, DIJKSTRA_CONFIG));
	mapConfig.insert(std::make_pair(DFS, DFS_CONFIG));
	mapConfig.insert(std::make_pair(BFS, BFS_CONFIG));
	mapConfig.insert(std::make_pair(ASTAR, ASTAR_CONFIG));

}
void cPathFindWindow::setupMap() {

	if (frameContent != nullptr) {
		frameContent->Destroy();
		delete mapButtons;
	}

	frameContent = new wxPanel(this, wxID_ANY, wxDefaultPosition);
	frameContent->SetBackgroundColour(def_col::BLACK);

	mapSource = -1;
	mapDest = -1;

	int t_MAP_ROWS = mapConfig[currentMapType].MAP_ROWS;
	int t_MAP_COLS = mapConfig[currentMapType].MAP_COLS;
	mapButtonBlocked = std::vector<bool>(t_MAP_ROWS * t_MAP_COLS, false);

	mapButtons = new wxButton * [t_MAP_ROWS * t_MAP_COLS];
	wxGridSizer* mapSizer = new wxGridSizer(t_MAP_ROWS, t_MAP_COLS, 0, 0);
	wxFont mapFont(wxDefaultSize, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int i = 0; i < t_MAP_ROWS; i++)
		for (int j = 0; j < t_MAP_COLS; j++) {

			const int FIRST_DIM_EQ = i * t_MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ] = new wxButton(frameContent, 20000 + FIRST_DIM_EQ);
			mapButtons[FIRST_DIM_EQ]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPathFindWindow::mapButtonClicked, this);
			mapButtons[FIRST_DIM_EQ]->Bind(wxEVT_RIGHT_DOWN, &cPathFindWindow::rightButtonClicked, this);
			mapButtons[FIRST_DIM_EQ]->SetFont(mapFont);
			mapSizer->Add(mapButtons[FIRST_DIM_EQ], 1, wxEXPAND | wxALL);
		}

	frameContent->SetSizer(mapSizer);

	this->InvalidateBestSize();
	this->Layout();

}

void cPathFindWindow::setupToolbar() {

	wxFont toolbarFont(15, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	algorithmTools = this->CreateToolBar(wxTB_HORIZONTAL, wxID_ANY);
	algorithmTools->SetBackgroundColour(def_col::WHITE);

	algorithmChoice = new wxChoice(algorithmTools, wxID_ANY, wxDefaultPosition, wxSize(400, 500));
	algorithmChoice->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &cPathFindWindow::choiceSelected, this);
	algorithmChoice->SetFont(toolbarFont);

	for (MAP_TYPE type = MAP_TYPE(MAP_TYPE_START + 1); type < MAP_TYPE_END; type = MAP_TYPE(type + 1)) {
		algorithmChoice->Append(mapConfig[type].MAP_TITLE);
	}

	startButton = new wxButton(algorithmTools, wxID_ANY, "Start", wxDefaultPosition, wxSize(400, 100));
	startButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPathFindWindow::onStart, this);
	startButton->SetFont(toolbarFont);
	startButton->SetBackgroundColour(def_col::GREEN);

	sourceButton = new wxButton(algorithmTools, wxID_ANY, "Set source", wxDefaultPosition, wxSize(300, 100));
	sourceButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPathFindWindow::sourceSetButtonClicked, this);
	sourceButton->SetFont(toolbarFont);
	sourceButton->SetBackgroundColour(def_col::YELLOW);

	destButton = new wxButton(algorithmTools, wxID_ANY, "Set destination", wxDefaultPosition, wxSize(300, 100));
	destButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPathFindWindow::destSetButtonClicked, this);
	destButton->SetFont(toolbarFont);
	destButton->SetBackgroundColour(def_col::VIOLET);

	algorithmTools->AddControl(startButton);
	algorithmTools->AddStretchableSpace();
	algorithmTools->AddControl(algorithmChoice);
	algorithmTools->AddStretchableSpace();
	algorithmTools->AddControl(sourceButton);
	algorithmTools->AddStretchableSpace();
	algorithmTools->AddControl(destButton);
	algorithmTools->Realize();
}

void cPathFindWindow::choiceSelected(wxCommandEvent& evt) {

	wxChoice* currentChoice = (wxChoice*)evt.GetEventObject();
	int selected = currentChoice->GetCurrentSelection();

	MAP_TYPE selectedMapType = MAP_TYPE(MAP_TYPE_START + selected + 1);

	currentMapType = selectedMapType;

	setupMap();
	mapTypeSetup[currentMapType]();

}

void cPathFindWindow::setupDijkstraSp() {

	dijkstraImpl = std::make_unique<DijkstraSP>(mapConfig[DIJKSTRA].MAP_ROWS, mapConfig[DIJKSTRA].MAP_COLS, (AlgorithmFrame*)this);
	dijkstraImpl->generateValues(algorithmThread);

	cPathFindWindow::enableMapButtons();
	cPathFindWindow::enableToolbarButtons();
}

void cPathFindWindow::setupDfs() {

	dfsImpl = std::make_unique<DFSimpl>(mapConfig[DFS].MAP_ROWS, mapConfig[DFS].MAP_COLS, (AlgorithmFrame*)this);
	dfsImpl->generateValues(algorithmThread);

	cPathFindWindow::enableMapButtons();
	cPathFindWindow::enableToolbarButtons();
}


void cPathFindWindow::setupBfs() {

	bfsImpl = std::make_unique<BFSimpl>(mapConfig[BFS].MAP_ROWS, mapConfig[BFS].MAP_COLS, (AlgorithmFrame*)this);

	bfsImpl->generateValues(algorithmThread);

	cPathFindWindow::enableMapButtons();
	cPathFindWindow::enableToolbarButtons();
}

void cPathFindWindow::setupAstar() {
	aStarImpl = std::make_unique<AStar>(mapConfig[ASTAR].MAP_ROWS, mapConfig[ASTAR].MAP_COLS, (AlgorithmFrame*)this);

	aStarImpl->generateValues(algorithmThread);

	cPathFindWindow::enableMapButtons();
	cPathFindWindow::enableToolbarButtons();
}
void cPathFindWindow::mapButtonClicked(wxCommandEvent& evt) {

	wxButton* mapButtonClicked = (wxButton*)evt.GetEventObject();
	const int FIRST_DIM_EQ = mapButtonClicked->GetId() - 20000;

	switch (currentMapState) {
	case CHOOSING_SOURCE:
		cPathFindWindow::replaceSource(FIRST_DIM_EQ, (std::string)mapButtonClicked->GetLabelText());
		cPathFindWindow::setMapState(currentMapState = IDLE);
		break;

	case CHOOSING_DEST:
		cPathFindWindow::replaceDest(FIRST_DIM_EQ, (std::string)mapButtonClicked->GetLabelText());
		cPathFindWindow::setMapState(currentMapState = IDLE);
		break;

	default:
		break;
	}
}

void cPathFindWindow::rightButtonClicked(wxMouseEvent& evt) {

	wxButton* mapButtonClicked = (wxButton*)evt.GetEventObject();
	const int buttonId = mapButtonClicked->GetId() - 20000;

	switch (mapButtonBlocked[buttonId]) {
	case false:
		mapButtonClicked->SetBackgroundColour(def_col::BLACK);
		mapButtonBlocked[buttonId] = true;
		break;
	case true:
		mapButtonClicked->SetBackgroundColour(def_col::WHITE);
		mapButtonBlocked[buttonId] = false;
		break;

	}
}

void cPathFindWindow::sourceSetButtonClicked(wxCommandEvent& evt) {
	cPathFindWindow::setMapState(currentMapState = CHOOSING_SOURCE);
}

void cPathFindWindow::destSetButtonClicked(wxCommandEvent& evt) {
	cPathFindWindow::setMapState(currentMapState = CHOOSING_DEST);
}

void cPathFindWindow::onStart(wxCommandEvent& evt) {

	switch (currentMapState) {
	case IDLE:
		cPathFindWindow::runAlgorithm();
		break;

	case RUNNING:
		cPathFindWindow::stopAlgorithm();
		break;

	case FINISHED:
		cPathFindWindow::resetMap();
		break;
	}
}

void cPathFindWindow::runAlgorithm() {

	if (mapSource != -1) {
		mapTypeStart[currentMapType]();
		cPathFindWindow::setMapState(currentMapState = RUNNING);
		cPathFindWindow::disableMapButtons();

	}

}

void cPathFindWindow::stopAlgorithm() {

	algorithmThread->Delete();
	mapTypeSetup[currentMapType]();

	mapSource = -1;
	mapSourceValue = "";
	cPathFindWindow::setMapState(currentMapState = IDLE);
	cPathFindWindow::enableMapButtons();

}

void cPathFindWindow::resetMap() {

	for (int i = 0; i < mapButtonBlocked.size(); i++)
		mapButtonBlocked[i] = false;

	mapSource = -1;
	mapDest = -1;
	mapSourceValue = "";

	cPathFindWindow::setMapState(currentMapState = IDLE);
	startButton->SetLabelText("Start");
	mapTypeSetup[currentMapType]();

}

void cPathFindWindow::assignAlgorithmThread(const std::function<void()>& runFunction) {
	algorithmThread = new AlgorithmThread(this, runFunction);
	algorithmThread->Create();
	algorithmThread->Run();
}

void cPathFindWindow::runDijkstra() {

	if (mapSource != -1 && mapDest != -1) {
		dijkstraImpl->setSource(mapSource);
		dijkstraImpl->setDest(mapDest);
		dijkstraImpl->setBlockedCells(mapButtonBlocked);
		cPathFindWindow::assignAlgorithmThread([this]() -> void {this->dijkstraImpl->runAlgorithm(algorithmThread); });

	}

}

void cPathFindWindow::runDfs() {

	if (mapSource != -1 && mapDest != -1) {
		dfsImpl->setSource(mapSource);
		dfsImpl->setDest(mapDest);
		dfsImpl->setBlockedCells(mapButtonBlocked);
		cPathFindWindow::assignAlgorithmThread([this]() -> void { this->dfsImpl->runAlgorithm(algorithmThread); });
	}
}

void cPathFindWindow::runBfs() {

	if (mapSource != -1 && mapDest != -1) {
		bfsImpl->setSource(mapSource);
		bfsImpl->setDest(mapDest);
		bfsImpl->setBlockedCells(mapButtonBlocked);
		cPathFindWindow::assignAlgorithmThread([this]() -> void { this->bfsImpl->runAlgorithm(algorithmThread); });
	}

}

void cPathFindWindow::runAstar() {

	if (mapSource != -1 && mapDest != -1) {
		aStarImpl->setSource(mapSource);
		aStarImpl->setDest(mapDest);
		aStarImpl->setBlockedCells(mapButtonBlocked);
		cPathFindWindow::assignAlgorithmThread([this]() -> void {this->aStarImpl->runAlgorithm(algorithmThread); });
	}

}

void cPathFindWindow::onThreadEnd(wxCommandEvent& evt) {
	cPathFindWindow::setMapState(currentMapState = FINISHED);
}


void cPathFindWindow::onThreadBreak(wxCommandEvent& evt) {
	cPathFindWindow::setMapState(currentMapState = IDLE);
}

void cPathFindWindow::cellUpdate(wxThreadEvent& evt) {

	def_type::CELL_UPDATE_INFO NEW_DATA = evt.GetPayload<def_type::CELL_UPDATE_INFO>();

	int FIRST_DIM_EQ = NEW_DATA.FIRST_DIM_EQ;
	std::string newValue = NEW_DATA.newValue;
	wxColour newColour = NEW_DATA.newColour;

	cPathFindWindow::updateCellColor(FIRST_DIM_EQ, newColour);
	cPathFindWindow::updateCellValue(FIRST_DIM_EQ, wxString(newValue));

}

void cPathFindWindow::updateCellColor(const int& FIRST_DIM_EQ, const wxColour& t_newColour) {
	mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(t_newColour);
}

void cPathFindWindow::updateCellValue(const int& FIRST_DIM_EQ, const wxString& t_newValue) {
	mapButtons[FIRST_DIM_EQ]->SetLabelText(t_newValue);
}

void cPathFindWindow::setMapState(MAP_STATE t_mapState) {
	switch (t_mapState) {
	case IDLE:
		cPathFindWindow::enableMapButtons();
		cPathFindWindow::enableToolbarButtons();
		startButton->SetLabelText("Start");
		break;

	case RUNNING:
		cPathFindWindow::disableMapButtons();
		cPathFindWindow::disableToolbarButtons();
		startButton->SetLabelText("Stop");
		break;

	case FINISHED:
		cPathFindWindow::enableMapButtons();
		startButton->SetLabelText("Restart");
		break;
	}
}

void cPathFindWindow::replaceSource(const int t_newSource, const std::string t_newSourceValue) {


	if (mapSource != -1) {
		mapButtons[mapSource]->SetBackgroundColour(def_col::WHITE);
		mapButtons[mapSource]->SetLabelText(mapSourceValue);
	}


	mapButtons[t_newSource]->SetBackgroundColour(def_col::BLUE);
	mapButtons[t_newSource]->SetLabelText("Source");

	if (t_newSource != mapSource) {
		mapSource = t_newSource;
		mapSourceValue = t_newSourceValue;
	}

}
void cPathFindWindow::replaceDest(const int t_newDest, const std::string t_newDestValue) {

	if (mapDest != -1) {
		mapButtons[mapDest]->SetBackgroundColour(def_col::WHITE);
		mapButtons[mapDest]->SetLabelText(mapDestValue);
	}

	mapButtons[t_newDest]->SetBackgroundColour(def_col::BLUE);
	mapButtons[t_newDest]->SetLabelText("Dest");

	if (t_newDest != mapDest) {
		mapDest = t_newDest;
		mapDestValue = t_newDestValue;
	}
}
void cPathFindWindow::disableMapButtons() {

	for (int i = 0; i < mapConfig[currentMapType].MAP_ROWS; i++)
		for (int j = 0; j < mapConfig[currentMapType].MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * mapConfig[currentMapType].MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->Disable();
		}
}

void cPathFindWindow::enableMapButtons() {

	for (int i = 0; i < mapConfig[currentMapType].MAP_ROWS; i++)
		for (int j = 0; j < mapConfig[currentMapType].MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * mapConfig[currentMapType].MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->Enable();
		}
}

void cPathFindWindow::enableToolbarButtons()
{
	algorithmChoice->Enable();
	sourceButton->Enable();
	destButton->Enable();
}

void cPathFindWindow::disableToolbarButtons()
{
	algorithmChoice->Disable();
	sourceButton->Disable();
	destButton->Disable();
}

void cPathFindWindow::OnClose(wxCloseEvent& evt) {
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