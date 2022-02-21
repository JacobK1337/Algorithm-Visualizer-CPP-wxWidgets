#include "cPathFindWindow.h"

wxBEGIN_EVENT_TABLE(cPathFindWindow, wxFrame)
EVT_COMMAND(evt_id::THREAD_END_ID, wxEVT_THREAD_END, cPathFindWindow::onThreadEnd)
EVT_COMMAND(evt_id::THREAD_BREAK_ID, wxEVT_THREAD_BREAK, cPathFindWindow::onThreadBreak)
wxEND_EVENT_TABLE()

using namespace std;
cPathFindWindow::cPathFindWindow() : AlgorithmFrame(wxString("Pathfinding algorithms"), wxPoint(10, 10), wxSize(1600, 1000)) {

	cPathFindWindow::applyMapConfig();
	cPathFindWindow::setupToolbar();
	Connect(evt_id::MAP_UPDATE_REQUEST_ID, wxEVT_MAP_UPDATE_REQUEST, wxThreadEventHandler(cPathFindWindow::cellVisitedUpdate));
	
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
	frameContent->SetBackgroundColour(wxColour(255, 255, 255));

	mapSource = -1;
	mapDest = -1;

	int t_MAP_ROWS = mapConfig[currentMapType].MAP_ROWS;
	int t_MAP_COLS = mapConfig[currentMapType].MAP_COLS;
	mapButtonBlocked = vector<bool>(t_MAP_ROWS * t_MAP_COLS, false);

	mapButtons = new wxButton * [t_MAP_ROWS * t_MAP_COLS];
	wxGridSizer* mapSizer = new wxGridSizer(t_MAP_ROWS, t_MAP_COLS, 0, 0);
	wxFont mapFont(15, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

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

void cPathFindWindow::setupAlgorithmList()
{
}

void cPathFindWindow::setupToolbar() {

	algorithmTools = this->CreateToolBar(wxTB_HORIZONTAL, wxID_ANY);
	algorithmChoice = new wxChoice(algorithmTools, wxID_ANY, wxDefaultPosition, wxSize(400, 500));
	algorithmChoice->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &cPathFindWindow::choiceSelected, this);

	for (auto& mapType : mapTypeMapping) {
		algorithmChoice->Append(mapConfig[mapType].MAP_TITLE);
	}

	startButton = new wxButton(algorithmTools, wxID_ANY, "Start", wxDefaultPosition, wxSize(400, 100));
	startButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPathFindWindow::onStart, this);

	sourceButton = new wxButton(algorithmTools, wxID_ANY, "Set source", wxDefaultPosition, wxSize(300, 100));
	sourceButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPathFindWindow::sourceSetButtonClicked, this);

	destButton = new wxButton(algorithmTools, wxID_ANY, "Set destination", wxDefaultPosition, wxSize(300, 100));
	destButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPathFindWindow::destSetButtonClicked, this);

	algorithmTools->AddControl(startButton);
	algorithmTools->AddStretchableSpace();
	algorithmTools->AddControl(destButton);
	algorithmTools->AddStretchableSpace();
	algorithmTools->AddControl(algorithmChoice);
	algorithmTools->AddStretchableSpace();
	algorithmTools->AddControl(sourceButton);
	algorithmTools->Realize();
}

void cPathFindWindow::choiceSelected(wxCommandEvent& evt) {

	wxChoice* currentChoice = (wxChoice*)evt.GetEventObject();
	int selected = currentChoice->GetCurrentSelection();

	MAP_TYPE selectedMapType = mapTypeMapping[selected];

	currentMapType = selectedMapType;

	setupMap();
	mapTypeSetup[currentMapType]();

}

void cPathFindWindow::setupDijkstraSp() {

	dijkstraImpl = make_unique<DijkstraSP>(mapConfig[DIJKSTRA].MAP_ROWS, mapConfig[DIJKSTRA].MAP_COLS, (AlgorithmFrame*)this);
	dijkstraImpl->generateValues(algorithmThread);
	
	cPathFindWindow::enableMapButtons();
	cPathFindWindow::enableToolbarButtons();
}

void cPathFindWindow::setupDfs() {

	dfsImpl = make_unique<DFSimpl>(mapConfig[DFS].MAP_ROWS, mapConfig[DFS].MAP_COLS, (AlgorithmFrame*)this);

	dfsImpl->generateValues(algorithmThread);

	cPathFindWindow::enableMapButtons();
	cPathFindWindow::enableToolbarButtons();

}


void cPathFindWindow::setupBfs() {

	bfsImpl = make_unique<BFSimpl>(mapConfig[BFS].MAP_ROWS, mapConfig[BFS].MAP_COLS, (AlgorithmFrame*)this);

	bfsImpl->generateValues(algorithmThread);

	cPathFindWindow::enableMapButtons();
	cPathFindWindow::enableToolbarButtons();

}

void cPathFindWindow::setupAstar() {
	aStarImpl = make_unique<AStar>(mapConfig[ASTAR].MAP_ROWS, mapConfig[ASTAR].MAP_COLS, (AlgorithmFrame*)this);

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
		mapButtonClicked->SetBackgroundColour(def_col::BLOCK_COLOUR);
		mapButtonBlocked[buttonId] = true;
		break;
	case true:
		mapButtonClicked->SetBackgroundColour(def_col::IDLE_COLOUR);
		mapButtonBlocked[buttonId] = false;
		break;

	}
}

void cPathFindWindow::buttonClickAction(wxButton* t_buttonClicked) {
	const int FIRST_DIM_EQ = t_buttonClicked->GetId() - 20000;

	switch (currentMapState) {
	case CHOOSING_SOURCE:
		cPathFindWindow::replaceSource(FIRST_DIM_EQ, (std::string)t_buttonClicked->GetLabelText());
		cPathFindWindow::setMapState(currentMapState = IDLE);
		break;

	case CHOOSING_DEST:
		cPathFindWindow::replaceDest(FIRST_DIM_EQ, (std::string)t_buttonClicked->GetLabelText());
		cPathFindWindow::setMapState(currentMapState = IDLE);
		break;

	default:
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
	mapTypeSetup[currentMapType]();

}

void cPathFindWindow::assignAlgorithmThread(const function<void()>& runFunction) {
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

void cPathFindWindow::cellVisitedUpdate(wxThreadEvent& evt) {

	def_type::CELL_UPDATE_INFO NEW_DATA = evt.GetPayload<def_type::CELL_UPDATE_INFO>();

	int FIRST_DIM_EQ = NEW_DATA.FIRST_DIM_EQ;
	int newValue = NEW_DATA.newValue;
	wxColour newColour = NEW_DATA.newColour;

	cPathFindWindow::updateCellColor(FIRST_DIM_EQ, newColour);

	if (newValue == -1)
		cPathFindWindow::updateCellValue(FIRST_DIM_EQ, wxString(""));

	else
		cPathFindWindow::updateCellValue(FIRST_DIM_EQ, wxString(to_string(newValue)));

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

	case CHOOSING_SOURCE:
		break;

	case FINISHED:
		cPathFindWindow::enableMapButtons();
		startButton->SetLabelText("Restart");
		break;
	}
}

void cPathFindWindow::replaceSource(const int t_newSource, const std::string t_newSourceValue) {


	if (mapSource != -1) {
		mapButtons[mapSource]->SetBackgroundColour(def_col::IDLE_COLOUR);
		mapButtons[mapSource]->SetLabelText(mapSourceValue);
	}

	mapButtons[t_newSource]->SetBackgroundColour(def_col::SOURCE_COLOUR);
	mapButtons[t_newSource]->SetLabelText("Source");

	mapSource = t_newSource;
	mapSourceValue = t_newSourceValue;

}
void cPathFindWindow::replaceDest(const int t_newDest, const std::string t_newDestValue) {

	if (mapDest != -1) {
		mapButtons[mapDest]->SetBackgroundColour(def_col::IDLE_COLOUR);
		mapButtons[mapDest]->SetLabelText(mapDestValue);
	}

	mapButtons[t_newDest]->SetBackgroundColour(def_col::SOURCE_COLOUR);
	mapButtons[t_newDest]->SetLabelText("Destination");

	mapDest = t_newDest;
	mapDestValue = t_newDestValue;
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
}

void cPathFindWindow::disableToolbarButtons()
{
	algorithmChoice->Disable();
	sourceButton->Disable();
}


cPathFindWindow::~cPathFindWindow() {
	delete mapButtons;
}