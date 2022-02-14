#pragma once
#include<wx/wx.h>
#include"DijkstraSP.h"
#include"DFSimpl.h"
#include"BFSimpl.h"
#include"SideThread.h"
#include<string>
#include<map>
class cPathFindWindow : wxFrame
{
private:

	const int MAP_ROWS = 15;
	const int MAP_COLS = 15;
	const enum MAP_TYPE{DIJKSTRA, DFS, BFS};
	const enum MAP_STATE{IDLE, CHOOSING_SOURCE, RUNNING, FINISHED};
	MAP_TYPE currentMapType;
	MAP_STATE currentMapState = IDLE;



	//algorithm implementations
	unique_ptr<DijkstraSP> dijkstraImpl;
	unique_ptr<DFSimpl> dfsImpl;
	unique_ptr<BFSimpl> bfsImpl;

	//wxWidgets elements
	wxPanel* frameContent = nullptr;
	wxButton** mapButtons = nullptr;
	vector<bool> mapButtonBlocked;

	wxToolBar* algorithmTools = nullptr;
	wxChoice* algorithmChoice = nullptr;
	wxButton* startButton = nullptr;
	wxButton* sourceButton = nullptr;
	wxButton* generateCostButton = nullptr;

	//side thread to proccess algorithms
	SideThread* someThread = nullptr;

	//algorithm setup/description/start/onclick maps
	vector<MAP_TYPE> mapTypeMapping = {
		DIJKSTRA, DFS, BFS
	};

	map<MAP_TYPE, string> mapTypeDesc = {
		{DIJKSTRA, "Dijkstra Shortest Path"},
		{DFS, "Depth-First Search"},
		{BFS, "Breadth-First Search"}
	};

	map < MAP_TYPE, function<void()> > mapTypeSetup = {
		{DIJKSTRA, [this]() -> void {this->setupDijkstraSp(); }},
		{DFS, [this]() -> void {this->setupDfs(); }},
		{BFS, [this]() -> void {this->setupBfs(); }}
	};

	map < MAP_TYPE, function<void()> > mapTypeStart = {
		{DIJKSTRA, [this]() -> void {this->runDijkstra(); }},
		{DFS, [this]() -> void {this->runDfs(); }},
		{BFS, [this]() -> void {this->runBfs(); }}
	};

	map< MAP_TYPE, function<void(wxButton*)> > mapTypeOnClick = {
		{DIJKSTRA, [this](wxButton* button) -> void {this->dijkstraButtonClickAction(button); }},
		{DFS, [this](wxButton* button) -> void {this->dfsButtonClickAction(button); }},
		{BFS, [this](wxButton* button) -> void {this->bfsButtonClickAction(button); }}
	};

private:
	void setupMap();
	void setupAlgorithmList();
	void setupToolbar();
	void setupDijkstraSp();
	void setupDfs();
	void setupBfs();

	void onStart(wxCommandEvent& evt);
	void runAlgorithm();
	void stopAlgorithm();
	void runDijkstra();
	void runDfs();
	void runBfs();

	void dijkstraButtonClickAction(wxButton* t_buttonClicked);
	void dfsButtonClickAction(wxButton* t_buttonClicked);
	void bfsButtonClickAction(wxButton* t_buttonClicked);

	void setMapState(MAP_STATE t_mapState);
	void disableMapButtons();
	void enableMapButtons();
	void fixOldSource(const int t_oldSource, const int t_valueToSet);

	void assignAlgorithmThread(const function<void()> &runFunction);
	void onThreadRun(wxCommandEvent& evt);
	void onThreadEnd(wxCommandEvent& evt);

	void choiceSelected(wxCommandEvent& evt);
	void mapButtonClicked(wxCommandEvent& evt);
	void rightButtonClicked(wxMouseEvent& evt);
	void generateRandomCost(wxCommandEvent& evt);
	void sourceSetButtonClicked(wxCommandEvent& evt);

	void cellVisitedUpdate(wxCommandEvent& evt);
	void cellPathToSourceUpdate(wxCommandEvent& evt);
	void updateCellColor(const int& FIRST_DIM_EQ, wxColour const& t_newColour);
	void updateCellValue(const int& FIRST_DIM_EQ, wxString const& t_newValue);


public:
	bool isCellBlocked(const int& FIRST_DIM_EQ);
	cPathFindWindow();
	~cPathFindWindow();
	wxDECLARE_EVENT_TABLE();
};

