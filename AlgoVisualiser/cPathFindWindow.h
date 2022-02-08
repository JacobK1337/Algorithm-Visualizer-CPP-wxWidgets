#pragma once
#include<wx/wx.h>
#include"DijkstraSP.h"
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

	DijkstraSP* dijkstraImpl = nullptr;
	

	int source = -1;

	vector<int> *dijkstraMapCost = nullptr;
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


	void setupMap();
	void setupAlgorithmList();
	void setupToolbar();
	void setupDijkstraSp();
	void setupDfs();
	void setupBfs();



	void dijkstraButtonClickAction(wxButton* buttonClicked);

public:

	cPathFindWindow();
	~cPathFindWindow();
	void choiceSelected(wxCommandEvent& evt);
	void mapButtonClicked(wxCommandEvent& evt);
	void generateRandomCost(wxCommandEvent& evt);
	void sourceSetButtonClicked(wxCommandEvent& evt);
	void swapSources(const int newSourceInd);

	wxDECLARE_EVENT_TABLE();
};

