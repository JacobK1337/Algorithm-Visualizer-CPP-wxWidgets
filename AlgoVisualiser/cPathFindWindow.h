#pragma once
#include<wx/wx.h>
#include"AlgorithmFrame.h"
#include"DijkstraSP.h"
#include"DFSimpl.h"
#include"BFSimpl.h"
#include"AStar.h"
#include<string>
#include<map>

class cPathFindWindow : AlgorithmFrame
{

public:
	cPathFindWindow();
	~cPathFindWindow();
	wxDECLARE_EVENT_TABLE();

private:
	int mapSource = -1;
	int mapDest = -1;
	std::string mapSourceValue = "";
	std::string mapDestValue = "";
	std::vector<bool> mapButtonBlocked;
	const enum MAP_TYPE{
		MAP_TYPE_START,

		DIJKSTRA, 
		DFS,
		BFS, 
		ASTAR,

		MAP_TYPE_END
	};

	MAP_TYPE currentMapType;
	MAP_STATE currentMapState = IDLE;

	//algorithm implementations
	std::unique_ptr<DijkstraSP> dijkstraImpl;
	std::unique_ptr<DFSimpl> dfsImpl;
	std::unique_ptr<BFSimpl> bfsImpl;
	std::unique_ptr<AStar> aStarImpl;

	//wxWidgets elements
	wxPanel* frameContent = nullptr;
	wxButton** mapButtons = nullptr;

	wxToolBar* algorithmTools = nullptr;
	wxChoice* algorithmChoice = nullptr;
	wxButton* startButton = nullptr;
	wxButton* sourceButton = nullptr;
	wxButton* destButton = nullptr;

	//side thread to proccess algorithms
	AlgorithmThread* algorithmThread = nullptr;

	//algorithm setup/description/start/onclick maps
	
	std::map<MAP_TYPE, MapConfig> mapConfig;

	std::map<MAP_TYPE, std::string> mapTypeDesc = {
		{DIJKSTRA, "Dijkstra Shortest Path"},
		{DFS, "Depth-First Search"},
		{BFS, "Breadth-First Search"}
	};

	std::map < MAP_TYPE, std::function<void()> > mapTypeSetup = {
		{DIJKSTRA, [this]() -> void {this->setupDijkstraSp(); }},
		{DFS, [this]() -> void {this->setupDfs(); }},
		{BFS, [this]() -> void {this->setupBfs(); }},
		{ASTAR, [this]() -> void {this->setupAstar(); }}
	};

	std::map < MAP_TYPE, std::function<void()> > mapTypeStart = {
		{DIJKSTRA, [this]() -> void {this->runDijkstra(); }},
		{DFS, [this]() -> void {this->runDfs(); }},
		{BFS, [this]() -> void {this->runBfs(); }},
		{ASTAR, [this]() -> void {this->runAstar(); }}
	};
	/*
	std::map< MAP_TYPE, std::function<void(wxButton*)> > mapTypeOnClick = {
		{DIJKSTRA, [this](wxButton* button) -> void {this->dijkstraButtonClickAction(button); }},
		{DFS, [this](wxButton* button) -> void {this->dfsButtonClickAction(button); }},
		{BFS, [this](wxButton* button) -> void {this->bfsButtonClickAction(button); }},
		{ASTAR, [this](wxButton* button) -> void {this->aStarButtonClickAction(button); }}
	};
	
	*/

private:
	// Odziedziczono za poœrednictwem elementu AlgorithmFrame
	virtual void setupMap() override;
	virtual void setupToolbar() override;
	virtual void onStart(wxCommandEvent& evt) override;
	virtual void runAlgorithm() override;
	virtual void stopAlgorithm() override;
	virtual void resetMap() override;
	virtual void setMapState(MAP_STATE t_mapState) override;
	virtual void disableMapButtons() override;
	virtual void enableMapButtons() override;
	virtual void enableToolbarButtons() override;
	virtual void disableToolbarButtons() override;
	virtual void replaceSource(const int t_newSource, const std::string t_newSourceValue) override;
	virtual void replaceDest(const int t_newDest, const std::string t_newDestValue) override;
	virtual void assignAlgorithmThread(const std::function<void()>& runFunction) override;
	virtual void onThreadEnd(wxCommandEvent& evt) override;
	virtual void onThreadBreak(wxCommandEvent& evt) override;
	virtual void choiceSelected(wxCommandEvent& evt) override;
	virtual void mapButtonClicked(wxCommandEvent& evt) override;
	virtual void rightButtonClicked(wxMouseEvent& evt) override;
	virtual void sourceSetButtonClicked(wxCommandEvent& evt) override;
	void destSetButtonClicked(wxCommandEvent& evt);
	virtual void cellUpdate(wxThreadEvent& evt) override;
	virtual void updateCellColor(const int& FIRST_DIM_EQ, wxColour const& t_newColour) override;
	virtual void updateCellValue(const int& FIRST_DIM_EQ, wxString const& t_newValue) override;

	//algorithm setup functions
	void setupDijkstraSp();
	void setupDfs();
	void setupBfs();
	void setupAstar();

	void runDijkstra();
	void runDfs();
	void runBfs();
	void runAstar();


	void applyMapConfig();
	void OnClose(wxCloseEvent& evt);
};

