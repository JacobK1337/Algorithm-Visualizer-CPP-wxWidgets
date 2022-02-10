#include "cPathFindWindow.h"

wxBEGIN_EVENT_TABLE(cPathFindWindow, wxFrame)
//EVT_COMMAND(123, wxEVT_MYTHREAD, cPathFindWindow::onThreadRun)
wxEND_EVENT_TABLE()

cPathFindWindow::cPathFindWindow() : wxFrame(nullptr, wxID_ANY, "Pathfinding Algorithms", wxPoint(10, 10), wxSize(1600, 1000)) {
	this->SetBackgroundColour(wxColour(255, 201, 213));

	setupToolbar();

	frameContent = new wxPanel(this, wxID_ANY, wxDefaultPosition);
	frameContent->SetBackgroundColour(wxColour(43, 201, 231));

	mapButtons = new wxButton * [MAP_ROWS * MAP_COLS];

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
	startButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cPathFindWindow::runAlgorithm, this);

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

	//mockup
	dijkstraMapCost = new vector<vector<int>>(MAP_ROWS, vector<int>(MAP_COLS, 1));
	dijkstraImpl = new DijkstraSP(mapButtons, MAP_ROWS * MAP_COLS);
	//dijkstraImpl->setCostList(*dijkstraMapCost);


	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->SetLabelText("1");
			mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(wxColour(255, 255, 255));
		}

	generateCostButton->Enable();
}

void cPathFindWindow::setupDfs() {

	//mockup
	dfsImpl = new DFSimpl(mapButtons, MAP_ROWS, MAP_COLS);

	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;
			mapButtons[FIRST_DIM_EQ]->SetLabelText(to_string(FIRST_DIM_EQ));
			mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(wxColour(255, 255, 255));
		}
	source = 100;
	generateCostButton->Disable();

}


void cPathFindWindow::setupBfs() {

	//mockup
	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;

			mapButtons[FIRST_DIM_EQ]->SetLabelText("BFS");
		}

}

void cPathFindWindow::mapButtonClicked(wxCommandEvent& evt) {

	wxButton* currentButton = (wxButton*)evt.GetEventObject();

	switch (currentMapType) {
	case DIJKSTRA:
		cPathFindWindow::dijkstraButtonClickAction(currentButton);
		break;
	case DFS:
		break;
	case BFS:
		break;
	}

}


void cPathFindWindow::dijkstraButtonClickAction(wxButton* buttonClicked) {
	const int FIRST_DIM_EQ = buttonClicked->GetId() - 20000;

	switch (choosingSource) {
	case true:
		mapButtons[FIRST_DIM_EQ]->SetLabelText("START");
		mapButtons[FIRST_DIM_EQ]->SetBackgroundColour(wxColour(255, 0, 0));
		swapSources(FIRST_DIM_EQ);
		choosingSource = false;
		break;


	case false:
		if (FIRST_DIM_EQ != source) {
			int i = FIRST_DIM_EQ / MAP_COLS;
			int j = FIRST_DIM_EQ % MAP_COLS;
			(*dijkstraMapCost)[i][j] ++;
			buttonClicked->SetLabelText(to_string((*dijkstraMapCost)[i][j]));

		}
		break;

	}
}

void cPathFindWindow::sourceSetButtonClicked(wxCommandEvent& evt) {
	choosingSource ^= true;
}


void cPathFindWindow::generateRandomCost(wxCommandEvent& evt) {
	srand(time(NULL));

	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * MAP_COLS + j;

			int randomNum = rand() % 100;
			(*dijkstraMapCost)[i][j] = randomNum;

			if (FIRST_DIM_EQ != source)
				mapButtons[FIRST_DIM_EQ]->SetLabelText(to_string(randomNum));
		}
	}

	//dijkstraImpl->setCostList(*dijkstraMapCost);
}

void cPathFindWindow::runAlgorithm(wxCommandEvent& evt) {

	
	mapTypeStart[currentMapType]();

}

void cPathFindWindow::runDijkstra() {
	dijkstraImpl->setCostList(*dijkstraMapCost);

	switch (running) {
	case false:
		running = true;
		startButton->SetLabelText("Stop");
		someThread = new DijkstraThread(this, [this]() -> void {dijkstraImpl->runDijkstraAlgorithm(source); });
		disableMapButtons();

		someThread->Create();
		someThread->Run();
		break;

	case true:
		running = false;
		startButton->SetLabelText("Start");
		someThread->Kill();
		cPathFindWindow::setupDijkstraSp();
		enableMapButtons();

		source = -1;
		break;

	}

}

void cPathFindWindow::runDfs() {
	
	switch (running) {
	case false:
		running = true;
		startButton->SetLabelText("Stop");
		someThread = new DijkstraThread(this, [this]() -> void {dfsImpl->runDfsAlgorithm(source); });
		disableMapButtons();

		someThread->Create();
		someThread->Run();
		break;
	case true:
		running = false;
		startButton->SetLabelText("Start");
		someThread->Kill();
		cPathFindWindow::setupDfs();
		enableMapButtons();

		source = -1;

		break;
	}
}

void cPathFindWindow::runBfs() {

}

void cPathFindWindow::swapSources(const int newSourceInd) {

	wxButton* newSourceButton = mapButtons[newSourceInd];
	newSourceButton->SetBackgroundColour(wxColour(255, 0, 0));
	newSourceButton->SetLabelText("Source");

	if (source != -1) {
		wxButton* currentSourceButton = mapButtons[source];
		currentSourceButton->SetBackgroundColour(wxColour(255, 255, 255));
		currentSourceButton->SetLabelText("Deprived source");

	}

	source = newSourceInd;
}


void cPathFindWindow::disableMapButtons() {
	for(int i = 0; i < MAP_ROWS; i++)
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

cPathFindWindow::~cPathFindWindow() {
	delete dijkstraMapCost;
}