#include "cPathFindWindow.h"

wxBEGIN_EVENT_TABLE(cPathFindWindow, wxFrame)
EVT_COMMAND(evt_id::THREAD_END_ID, wxEVT_THREAD_END, cPathFindWindow::onThreadEnd)
EVT_COMMAND(evt_id::MAP_UPDATE_REQUEST_ID, wxEVT_MAP_UPDATE_REQUEST, cPathFindWindow::cellVisitedUpdate)
EVT_COMMAND(evt_id::MAP_RECON_REQUEST_ID, wxEVT_MAP_RECON_REQUEST, cPathFindWindow::cellPathToSourceUpdate)
wxEND_EVENT_TABLE()

using namespace std;
cPathFindWindow::cPathFindWindow() : wxFrame(nullptr, wxID_ANY, "Pathfinding Algorithms", wxPoint(10, 10), wxSize(1600, 1000)) {
	this->SetBackgroundColour(wxColour(255, 201, 213));
	setupToolbar();

	frameContent = new wxPanel(this, wxID_ANY, wxDefaultPosition);
	frameContent->SetBackgroundColour(wxColour(43, 201, 231));

	mapButtons = new wxButton * [MAP_ROWS * MAP_COLS];
	mapButtonBlocked = vector<bool>(MAP_ROWS * MAP_COLS, false);

	setupMap();
}

void cPathFindWindow::setupMap() {

	wxGridSizer* mapSizer = new wxGridSizer(MAP_ROWS, MAP_COLS, 0, 0);
	wxFont mapFont(15, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {

			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ] = new wxButton(frameContent, 20000 + FIRST_DIM_EQ);
			mapButtons[FIRST_DIM_EQ]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPathFindWindow::mapButtonClicked, this);
			mapButtons[FIRST_DIM_EQ]->Bind(wxEVT_RIGHT_DOWN, &cPathFindWindow::rightButtonClicked, this);
			mapButtons[FIRST_DIM_EQ]->SetFont(mapFont);
			mapSizer->Add(mapButtons[FIRST_DIM_EQ], 1, wxEXPAND | wxALL);
		}

	frameContent->SetSizer(mapSizer);

}

void cPathFindWindow::setupToolbar() {

	algorithmTools = this->CreateToolBar(wxTB_HORIZONTAL, wxID_ANY);
	algorithmChoice = new wxChoice(algorithmTools, wxID_ANY, wxDefaultPosition, wxSize(400, 500));
	algorithmChoice->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &cPathFindWindow::choiceSelected, this);

	for (auto& mapType : mapTypeMapping) {
		algorithmChoice->Append(mapTypeDesc[mapType]);
	}

	startButton = new wxButton(algorithmTools, wxID_ANY, "Start", wxDefaultPosition, wxSize(400, 100));
	startButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPathFindWindow::onStart, this);

	generateCostButton = new wxButton(algorithmTools, wxID_ANY, "Generate Cost", wxDefaultPosition, wxSize(400, 100));
	generateCostButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPathFindWindow::generateRandomCost, this);

	generateCostButton->Disable();

	sourceButton = new wxButton(algorithmTools, wxID_ANY, "Set source", wxDefaultPosition, wxSize(300, 100));
	sourceButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPathFindWindow::sourceSetButtonClicked, this);


	algorithmTools->AddControl(startButton);
	algorithmTools->AddStretchableSpace();
	algorithmTools->AddControl(algorithmChoice);
	algorithmTools->AddStretchableSpace();
	algorithmTools->AddControl(sourceButton);
	algorithmTools->AddStretchableSpace();
	algorithmTools->AddControl(generateCostButton);
	algorithmTools->Realize();
}

void cPathFindWindow::choiceSelected(wxCommandEvent& evt) {

	wxChoice* currentChoice = (wxChoice*)evt.GetEventObject();
	int selected = currentChoice->GetCurrentSelection();

	MAP_TYPE selectedMapType = mapTypeMapping[selected];

	currentMapType = selectedMapType;

	mapTypeSetup[currentMapType]();

}

void cPathFindWindow::setupDijkstraSp() {

	vector<vector<int>> initialCostList(MAP_ROWS, vector<int>(MAP_COLS, 1));
	dijkstraImpl = make_unique<DijkstraSP>(MAP_ROWS, MAP_COLS, (wxFrame*)this);
	dijkstraImpl->setCostList(initialCostList);

	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->SetLabelText("1");
			mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(wxColour(255, 255, 255));
		}

	generateCostButton->Enable();
}

void cPathFindWindow::setupDfs() {

	dfsImpl = make_unique<DFSimpl>(MAP_ROWS, MAP_COLS, (wxFrame*)this);

	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->SetLabelText(to_string(FIRST_DIM_EQ));
			mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(wxColour(255, 255, 255));
		}

	generateCostButton->Disable();

}


void cPathFindWindow::setupBfs() {

	bfsImpl = make_unique<BFSimpl>(MAP_ROWS, MAP_COLS, (wxFrame*)this);

	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->SetLabelText(to_string(FIRST_DIM_EQ));
			mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(wxColour(255, 255, 255));
		}

	generateCostButton->Disable();

}

void cPathFindWindow::mapButtonClicked(wxCommandEvent& evt) {

	wxButton* mapButtonClicked = (wxButton*)evt.GetEventObject();

	mapTypeOnClick[currentMapType](mapButtonClicked);

}

void cPathFindWindow::rightButtonClicked(wxMouseEvent& evt) {

	wxButton* mapButtonClicked = (wxButton*)evt.GetEventObject();
	const int buttonId = mapButtonClicked->GetId() - 20000;

	switch (mapButtonBlocked[buttonId]) {
	case false:
		mapButtonClicked->SetBackgroundColour(wxColour(0, 0, 0));
		mapButtonBlocked[buttonId] = true;
		break;
	case true:
		mapButtonClicked->SetBackgroundColour(wxColour(255, 255, 255));
		mapButtonBlocked[buttonId] = false;
		break;

	}

}
void cPathFindWindow::dijkstraButtonClickAction(wxButton* buttonClicked) {
	const int FIRST_DIM_EQ = buttonClicked->GetId() - 20000;

	switch (currentMapState) {
	case CHOOSING_SOURCE: {
		mapButtons[FIRST_DIM_EQ]->SetLabelText("START");
		mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(wxColour(255, 0, 0));

		const int oldSource = dijkstraImpl->getSource();
		if (oldSource != -1) {
			const int costI = oldSource / MAP_COLS, costJ = oldSource % MAP_COLS;
			const int oldSourceCost = dijkstraImpl->checkCellCost(costI, costJ);
			cPathFindWindow::fixOldSource(oldSource, oldSourceCost);
		}

		dijkstraImpl->setSource(FIRST_DIM_EQ);

		cPathFindWindow::setMapState(currentMapState = IDLE);
		break;
	}

	case IDLE:
		if (FIRST_DIM_EQ != dijkstraImpl->getSource()) {
			int i = FIRST_DIM_EQ / MAP_COLS;
			int j = FIRST_DIM_EQ % MAP_COLS;
			dijkstraImpl->incrementCellCost(i, j);
			buttonClicked->SetLabelText(to_string(dijkstraImpl->checkCellCost(i, j)));
		}
		break;
	case FINISHED:
		cPathFindWindow::setMapState(currentMapState = RUNNING);
		cPathFindWindow::assignAlgorithmThread([this, FIRST_DIM_EQ]() -> void {dijkstraImpl->showPathToSource(FIRST_DIM_EQ); });
		break;
	}
}
void cPathFindWindow::dfsButtonClickAction(wxButton* buttonClicked) {
	const int FIRST_DIM_EQ = buttonClicked->GetId() - 20000;

	switch (currentMapState) {
	case CHOOSING_SOURCE: {

		mapButtons[FIRST_DIM_EQ]->SetLabelText("START");
		mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(wxColour(255, 0, 0));
		const int oldSource = dfsImpl->getSource();

		if (oldSource != -1)
			cPathFindWindow::fixOldSource(oldSource, oldSource);

		dfsImpl->setSource(FIRST_DIM_EQ);

		cPathFindWindow::setMapState(currentMapState = IDLE);
		break;
	}
	case FINISHED:
		cPathFindWindow::setMapState(currentMapState = RUNNING);
		cPathFindWindow::assignAlgorithmThread([this, FIRST_DIM_EQ]() -> void {dfsImpl->showPathToSource(FIRST_DIM_EQ); });
		break;

	default:
		break;
	}
}

void cPathFindWindow::bfsButtonClickAction(wxButton* buttonClicked) {
	const int FIRST_DIM_EQ = buttonClicked->GetId() - 20000;

	switch (currentMapState) {
	case CHOOSING_SOURCE: {
		mapButtons[FIRST_DIM_EQ]->SetLabelText("START");
		mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(wxColour(255, 0, 0));
		const int oldSource = bfsImpl->getSource();

		if (oldSource != -1)
			cPathFindWindow::fixOldSource(oldSource, oldSource);

		bfsImpl->setSource(FIRST_DIM_EQ);

		cPathFindWindow::setMapState(currentMapState = IDLE);
		break;
	}

	default:
		break;
	}
}

void cPathFindWindow::sourceSetButtonClicked(wxCommandEvent& evt) {
	cPathFindWindow::setMapState(currentMapState = CHOOSING_SOURCE);
}


void cPathFindWindow::generateRandomCost(wxCommandEvent& evt) {
	srand(time(NULL));

	vector<vector<int>> costList(MAP_ROWS, vector<int>(MAP_COLS, 1));
	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;

			int randomNum = rand() % 100;
			costList[i][j] = randomNum;

			if (FIRST_DIM_EQ != dijkstraImpl->getSource());
			mapButtons[FIRST_DIM_EQ]->SetLabelText(to_string(randomNum));
		}
	}

	dijkstraImpl->setCostList(costList);
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
	mapTypeStart[currentMapType]();

	cPathFindWindow::setMapState(currentMapState = RUNNING);
	cPathFindWindow::disableMapButtons();
}

void cPathFindWindow::stopAlgorithm() {

	someThread->Kill();
	mapTypeSetup[currentMapType]();
	cPathFindWindow::setMapState(currentMapState = IDLE);
	cPathFindWindow::enableMapButtons();

}

void cPathFindWindow::resetMap() {

	mapTypeSetup[currentMapType]();
	cPathFindWindow::setMapState(currentMapState = IDLE);
	cPathFindWindow::enableMapButtons();

}

void cPathFindWindow::assignAlgorithmThread(const function<void()>& runFunction) {
	someThread = new SideThread(this, runFunction);
	someThread->Create();
	someThread->Run();
}

void cPathFindWindow::runDijkstra() {

	if (dijkstraImpl->getSource() != -1) {
		dijkstraImpl->setBlockedCells(mapButtonBlocked);
		cPathFindWindow::assignAlgorithmThread([this]() -> void {this->dijkstraImpl->runDijkstraAlgorithm(); });
	}

}

void cPathFindWindow::runDfs() {

	if (dfsImpl->getSource() != -1) {
		dfsImpl->setBlockedCells(mapButtonBlocked);
		cPathFindWindow::assignAlgorithmThread([this]() -> void {this->dfsImpl->runDfsAlgorithm(); });
	}
}

void cPathFindWindow::runBfs() {

	if (bfsImpl->getSource() != -1) {
		bfsImpl->setBlockedCells(mapButtonBlocked);
		cPathFindWindow::assignAlgorithmThread([this]() -> void {this->bfsImpl->runBfsAlgorithm(); });
	}

}


void cPathFindWindow::onThreadEnd(wxCommandEvent& evt) {
	cPathFindWindow::setMapState(currentMapState = FINISHED);
}

void cPathFindWindow::cellVisitedUpdate(wxCommandEvent& evt) {

	int FIRST_DIM_EQ = evt.GetInt();
	cPathFindWindow::updateCellColor(FIRST_DIM_EQ, wxColour(204, 204, 0));

	switch (currentMapType) {
	case DIJKSTRA:
		cPathFindWindow::updateCellValue(FIRST_DIM_EQ, wxString(to_string(dijkstraImpl->getShortestDistance(FIRST_DIM_EQ))));
		break;
	case DFS:
		cPathFindWindow::updateCellValue(FIRST_DIM_EQ, wxString(to_string(FIRST_DIM_EQ)));
		break;
	case BFS:
		cPathFindWindow::updateCellValue(FIRST_DIM_EQ, wxString(to_string(FIRST_DIM_EQ)));
		break;
	}

}
void cPathFindWindow::cellPathToSourceUpdate(wxCommandEvent& evt) {

	int FIRST_DIM_EQ = evt.GetInt();
	cPathFindWindow::updateCellColor(FIRST_DIM_EQ, wxColour(51, 255, 51));

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
		startButton->SetLabelText("Start");
		break;

	case RUNNING:
		cPathFindWindow::disableMapButtons();
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


bool cPathFindWindow::isCellBlocked(const int& FIRST_DIM_EQ) {
	return mapButtonBlocked[FIRST_DIM_EQ];
}

void cPathFindWindow::fixOldSource(const int t_oldSource, const int t_valueToSet) {

	mapButtons[t_oldSource]->SetLabelText(to_string(t_valueToSet));
	mapButtons[t_oldSource]->SetBackgroundColour(wxColour(255, 255, 255));

}

void cPathFindWindow::disableMapButtons() {

	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->Disable();
		}

	algorithmChoice->Disable();
	sourceButton->Disable();
	generateCostButton->Disable();
}

void cPathFindWindow::enableMapButtons() {

	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->Enable();
		}

	algorithmChoice->Enable();
	sourceButton->Enable();
	generateCostButton->Enable();
}

cPathFindWindow::~cPathFindWindow() {
	delete mapButtons;
}