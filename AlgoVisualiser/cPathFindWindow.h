#pragma once
#include<wx/wx.h>
#include"DijkstraSP.h"
#include"DFSimpl.h"
#include"DijkstraThread.h"
#include<string>
#include<stdio.h>
#include<map>
using namespace std;

class cPathFindWindow : wxFrame
{

private:

	const int MAP_ROWS = 15;
	const int MAP_COLS = 15;
	const enum MAP_TYPE{DIJKSTRA, DFS, BFS};
	MAP_TYPE currentMapType;

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

	DijkstraSP* dijkstraImpl = nullptr;
	DFSimpl* dfsImpl = nullptr;

	int source = -1;
	bool running = false;

	vector<vector<int>> *dijkstraMapCost = nullptr;
	wxPanel* frameContent = nullptr;
	wxToolBar* algorithmTools = nullptr;
	wxChoice* algorithmChoice = nullptr;

	wxButton* startButton = nullptr;
	wxButton* stopButton = nullptr;
	wxButton* sourceButton = nullptr;
	wxButton* generateCostButton = nullptr;

	wxButton** mapButtons = nullptr;

	bool choosingSource = false;
	//bool choosingDestination = false;

	DijkstraThread* someThread = nullptr;

	void setupMap();
	void setupAlgorithmList();
	void setupToolbar();
	void setupDijkstraSp();
	void setupDfs();
	void setupBfs();


	void runAlgorithm(wxCommandEvent& evt);
	void runDijkstra();
	void runDfs();
	void runBfs();
	void dijkstraButtonClickAction(wxButton* buttonClicked);

	void disableMapButtons();
	void enableMapButtons();

public:

	cPathFindWindow();
	~cPathFindWindow();
	void onThreadRun(wxCommandEvent& evt);
	void choiceSelected(wxCommandEvent& evt);
	void mapButtonClicked(wxCommandEvent& evt);
	void generateRandomCost(wxCommandEvent& evt);
	void sourceSetButtonClicked(wxCommandEvent& evt);
	void swapSources(const int newSourceInd);

	wxDECLARE_EVENT_TABLE();
};

