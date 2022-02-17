#include "cPathFindWindow.h"

wxBEGIN_EVENT_TABLE(cPathFindWindow, wxFrame)
EVT_COMMAND(evt_id::THREAD_END_ID, wxEVT_THREAD_END, cPathFindWindow::onThreadEnd)
EVT_COMMAND(evt_id::THREAD_BREAK_ID, wxEVT_THREAD_BREAK, cPathFindWindow::onThreadBreak)
wxEND_EVENT_TABLE()

using namespace std;
cPathFindWindow::cPathFindWindow() : AlgorithmFrame(wxString("Pathfinding algorithms"), wxPoint(10, 10), wxSize(1600, 1000)) {
	this->SetBackgroundColour(wxColour(255, 201, 213));
	setupToolbar();


	Connect(evt_id::MAP_UPDATE_REQUEST_ID, wxEVT_MAP_UPDATE_REQUEST, wxThreadEventHandler(cPathFindWindow::cellVisitedUpdate));

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

void cPathFindWindow::setupAlgorithmList()
{
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

	cPathFindWindow::resetMap();


}

void cPathFindWindow::setupDijkstraSp() {

	vector<vector<int>> initialCostList(MAP_ROWS, vector<int>(MAP_COLS, 1));
	dijkstraImpl = make_unique<DijkstraSP>(MAP_ROWS, MAP_COLS, (AlgorithmFrame*)this);
	dijkstraImpl->setCostList(initialCostList);

	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->SetLabelText("1");
			mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(def_col::IDLE_COLOUR);
		}

	cPathFindWindow::enableMapButtons();
	cPathFindWindow::enableToolbarButtons();
	generateCostButton->Enable();
}

void cPathFindWindow::setupDfs() {

	dfsImpl = make_unique<DFSimpl>(MAP_ROWS, MAP_COLS, (AlgorithmFrame*)this);

	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->SetLabelText(to_string(FIRST_DIM_EQ));
			mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(def_col::IDLE_COLOUR);
		}

	cPathFindWindow::enableMapButtons();
	cPathFindWindow::enableToolbarButtons();
	generateCostButton->Disable();

}


void cPathFindWindow::setupBfs() {

	bfsImpl = make_unique<BFSimpl>(MAP_ROWS, MAP_COLS, (AlgorithmFrame*)this);

	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->SetLabelText(to_string(FIRST_DIM_EQ));
			mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(def_col::IDLE_COLOUR);
		}

	cPathFindWindow::enableMapButtons();
	cPathFindWindow::enableToolbarButtons();
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
		mapButtonClicked->SetBackgroundColour(def_col::BLOCK_COLOUR);
		mapButtonBlocked[buttonId] = true;
		break;
	case true:
		mapButtonClicked->SetBackgroundColour(def_col::IDLE_COLOUR);
		mapButtonBlocked[buttonId] = false;
		break;

	}

}
void cPathFindWindow::dijkstraButtonClickAction(wxButton* buttonClicked) {
	const int FIRST_DIM_EQ = buttonClicked->GetId() - 20000;

	switch (currentMapState) {
	case CHOOSING_SOURCE: {

		cPathFindWindow::replaceSource(FIRST_DIM_EQ, (std::string)buttonClicked->GetLabelText());
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
		cPathFindWindow::assignAlgorithmThread([this, FIRST_DIM_EQ]() -> void {dijkstraImpl->showPathToSource(FIRST_DIM_EQ, algorithmThread); });

		break;
	}
}
void cPathFindWindow::dfsButtonClickAction(wxButton* buttonClicked) {
	const int FIRST_DIM_EQ = buttonClicked->GetId() - 20000;

	switch (currentMapState) {
	case CHOOSING_SOURCE: {

		cPathFindWindow::replaceSource(FIRST_DIM_EQ, std::to_string(FIRST_DIM_EQ));
		dfsImpl->setSource(FIRST_DIM_EQ);
		cPathFindWindow::setMapState(currentMapState = IDLE);

		break;
	}
	case FINISHED:
		cPathFindWindow::setMapState(currentMapState = RUNNING);
		cPathFindWindow::assignAlgorithmThread([this, FIRST_DIM_EQ]() -> void {dfsImpl->showPathToSource(FIRST_DIM_EQ, algorithmThread); });
		break;

	default:
		break;
	}
}

void cPathFindWindow::bfsButtonClickAction(wxButton* buttonClicked) {
	const int FIRST_DIM_EQ = buttonClicked->GetId() - 20000;

	switch (currentMapState) {
	case CHOOSING_SOURCE: {

		cPathFindWindow::replaceSource(FIRST_DIM_EQ, std::to_string(FIRST_DIM_EQ));
		bfsImpl->setSource(FIRST_DIM_EQ);
		cPathFindWindow::setMapState(currentMapState = IDLE);

		break;
	}
	case FINISHED:
		cPathFindWindow::setMapState(currentMapState = RUNNING);
		cPathFindWindow::assignAlgorithmThread([this, FIRST_DIM_EQ]() -> void {bfsImpl->showPathToSource(FIRST_DIM_EQ, algorithmThread); });
		break;

	default:
		break;
	}
}

void cPathFindWindow::buttonClickAction(wxButton* t_buttonClicked) {
	const int FIRST_DIM_EQ = t_buttonClicked->GetId() - 20000;

	switch (currentMapState) {
	case IDLE:

		break;
	case CHOOSING_SOURCE:

		break;
	case FINISHED:

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

	if (dijkstraImpl->getSource() != -1) {
		dijkstraImpl->setBlockedCells(mapButtonBlocked);
		cPathFindWindow::assignAlgorithmThread([this]() -> void {this->dijkstraImpl->runDijkstraAlgorithm(algorithmThread); });

	}

}

void cPathFindWindow::runDfs() {

	if (dfsImpl->getSource() != -1) {
		dfsImpl->setBlockedCells(mapButtonBlocked);
		cPathFindWindow::assignAlgorithmThread([this]() -> void { this->dfsImpl->runDfsAlgorithm(algorithmThread); });
	}
}

void cPathFindWindow::runBfs() {

	if (bfsImpl->getSource() != -1) {
		bfsImpl->setBlockedCells(mapButtonBlocked);
		cPathFindWindow::assignAlgorithmThread([this]() -> void { this->bfsImpl->runBfsAlgorithm(algorithmThread); });
	}

}


void cPathFindWindow::onThreadEnd(wxCommandEvent& evt) {
	cPathFindWindow::setMapState(currentMapState = FINISHED);
}


void cPathFindWindow::onThreadBreak(wxCommandEvent& evt) {
	cPathFindWindow::setMapState(currentMapState = IDLE);
}

void cPathFindWindow::cellVisitedUpdate(wxThreadEvent& evt) {

	//int FIRST_DIM_EQ = evt.GetInt();

	def_type::CELL_UPDATE_INFO NEW_DATA = evt.GetPayload<def_type::CELL_UPDATE_INFO>();

	int FIRST_DIM_EQ = NEW_DATA.FIRST_DIM_EQ;
	int newValue = NEW_DATA.newValue;
	wxColour newColour = NEW_DATA.newColour;

	cPathFindWindow::updateCellColor(FIRST_DIM_EQ, newColour);

	if (newValue != -1)
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

void cPathFindWindow::disableMapButtons() {

	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->Disable();
		}
}

void cPathFindWindow::enableMapButtons() {

	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->Enable();
		}
}

void cPathFindWindow::enableToolbarButtons()
{
	algorithmChoice->Enable();
	sourceButton->Enable();
	generateCostButton->Enable();
}

void cPathFindWindow::disableToolbarButtons()
{
	algorithmChoice->Disable();
	sourceButton->Disable();
	generateCostButton->Disable();
}

cPathFindWindow::~cPathFindWindow() {
	delete mapButtons;
}