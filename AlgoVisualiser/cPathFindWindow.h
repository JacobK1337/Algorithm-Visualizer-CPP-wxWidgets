#pragma once
#include<wx/wx.h>
#include"AlgorithmFrame.h"
#include"DijkstraSP.h"
#include"DFSimpl.h"
#include"BFSimpl.h"
#include<string>
#include<map>

class cPathFindWindow : AlgorithmFrame
{
private:

	const int MAP_ROWS = 15;
	const int MAP_COLS = 15;
	int mapSource = -1;
	std::string mapSourceValue = "";
	const enum MAP_TYPE{DIJKSTRA, DFS, BFS};

	MAP_TYPE currentMapType;
	MAP_STATE currentMapState = IDLE;

	//algorithm implementations
	std::unique_ptr<DijkstraSP> dijkstraImpl;
	std::unique_ptr<DFSimpl> dfsImpl;
	std::unique_ptr<BFSimpl> bfsImpl;

	//wxWidgets elements
	wxPanel* frameContent = nullptr;
	wxButton** mapButtons = nullptr;
	std::vector<bool> mapButtonBlocked;

	wxToolBar* algorithmTools = nullptr;
	wxChoice* algorithmChoice = nullptr;
	wxButton* startButton = nullptr;
	wxButton* sourceButton = nullptr;

	//side thread to proccess algorithms
	AlgorithmThread* algorithmThread = nullptr;

	//algorithm setup/description/start/onclick maps
	std::vector<MAP_TYPE> mapTypeMapping = {
		DIJKSTRA, DFS, BFS
	};

	std::map<MAP_TYPE, std::string> mapTypeDesc = {
		{DIJKSTRA, "Dijkstra Shortest Path"},
		{DFS, "Depth-First Search"},
		{BFS, "Breadth-First Search"}
	};

	std::map < MAP_TYPE, std::function<void()> > mapTypeSetup = {
		{DIJKSTRA, [this]() -> void {this->setupDijkstraSp(); }},
		{DFS, [this]() -> void {this->setupDfs(); }},
		{BFS, [this]() -> void {this->setupBfs(); }}
	};

	std::map < MAP_TYPE, std::function<void()> > mapTypeStart = {
		{DIJKSTRA, [this]() -> void {this->runDijkstra(); }},
		{DFS, [this]() -> void {this->runDfs(); }},
		{BFS, [this]() -> void {this->runBfs(); }}
	};

	std::map< MAP_TYPE, std::function<void(wxButton*)> > mapTypeOnClick = {
		{DIJKSTRA, [this](wxButton* button) -> void {this->dijkstraButtonClickAction(button); }},
		{DFS, [this](wxButton* button) -> void {this->dfsButtonClickAction(button); }},
		{BFS, [this](wxButton* button) -> void {this->bfsButtonClickAction(button); }}
	};
	

private:
	// Odziedziczono za poœrednictwem elementu AlgorithmFrame
	virtual void setupMap() override;
	virtual void setupAlgorithmList() override;
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
	virtual void assignAlgorithmThread(const std::function<void()>& runFunction) override;
	virtual void onThreadEnd(wxCommandEvent& evt) override;
	virtual void onThreadBreak(wxCommandEvent& evt) override;
	virtual void choiceSelected(wxCommandEvent& evt) override;
	virtual void mapButtonClicked(wxCommandEvent& evt) override;
	virtual void rightButtonClicked(wxMouseEvent& evt) override;
	virtual void sourceSetButtonClicked(wxCommandEvent& evt) override;
	virtual void cellVisitedUpdate(wxThreadEvent& evt) override;
	virtual void updateCellColor(const int& FIRST_DIM_EQ, wxColour const& t_newColour) override;
	virtual void updateCellValue(const int& FIRST_DIM_EQ, wxString const& t_newValue) override;

	//algorithm setup functions
	void setupDijkstraSp();
	void setupDfs();
	void setupBfs();

	void runDijkstra();
	void runDfs();
	void runBfs();

	void dijkstraButtonClickAction(wxButton* t_buttonClicked);
	void dfsButtonClickAction(wxButton* t_buttonClicked);
	void bfsButtonClickAction(wxButton* t_buttonClicked);
	
	void buttonClickAction(wxButton* t_buttonClicked);

public:
	cPathFindWindow();
	~cPathFindWindow();
	wxDECLARE_EVENT_TABLE();
};

