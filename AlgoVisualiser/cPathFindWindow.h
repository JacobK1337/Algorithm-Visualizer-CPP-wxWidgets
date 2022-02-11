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
	int source = -1;

	MAP_TYPE currentMapType;
	MAP_STATE currentMapState = IDLE;

	unique_ptr<DijkstraSP> dijkstraImpl = nullptr;
	unique_ptr<vector<vector<int>>> dijkstraMapCost = nullptr;

	unique_ptr<DFSimpl> dfsImpl = nullptr;
	unique_ptr<BFSimpl> bfsImpl = nullptr;

	wxPanel* frameContent = nullptr;
	wxButton** mapButtons = nullptr;

	wxToolBar* algorithmTools = nullptr;
	wxChoice* algorithmChoice = nullptr;
	wxButton* startButton = nullptr;
	wxButton* sourceButton = nullptr;
	wxButton* generateCostButton = nullptr;

	SideThread* someThread = nullptr;

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

	void onThreadRun(wxCommandEvent& evt);
	void onThreadEnd(wxCommandEvent& evt);

	void choiceSelected(wxCommandEvent& evt);
	void mapButtonClicked(wxCommandEvent& evt);
	void generateRandomCost(wxCommandEvent& evt);
	void sourceSetButtonClicked(wxCommandEvent& evt);
	void assignSource(const int t_newSourceInd);

public:
	cPathFindWindow();
	~cPathFindWindow();
	wxDECLARE_EVENT_TABLE();
};

