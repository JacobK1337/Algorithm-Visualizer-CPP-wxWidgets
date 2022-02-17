#pragma once
#include<wx/wx.h>
#include"AlgorithmFrame.h"
#include<string>
#include<map>
#include<vector>
#include"KTPImpl.h"
class cBacktrackWindow : AlgorithmFrame
{

public:
	cBacktrackWindow();
	~cBacktrackWindow();
	wxDECLARE_EVENT_TABLE();

private:
	const int MAP_ROWS = 5;
	const int MAP_COLS = 5;

	int mapSource = -1;
	std::string mapSourceValue = "";

	const enum MAP_TYPE {KNIGHT};
	MAP_TYPE currentMapType;
	MAP_STATE currentMapState = IDLE;

	wxPanel* frameContent = nullptr;
	wxButton** mapButtons = nullptr;

	wxToolBar* algorithmTools = nullptr;
	wxChoice* algorithmChoice = nullptr;
	wxButton* startButton = nullptr;
	wxButton* sourceButton = nullptr;

	AlgorithmThread* algorithmThread = nullptr;

	std::unique_ptr<KTPImpl> ktpImpl;

	std::vector<MAP_TYPE> mapTypeMapping = {
		KNIGHT
	};

	std::map<MAP_TYPE, std::string> mapTypeDesc = {
		{KNIGHT, "Knight's tour algorithm"},
	};

	std::map < MAP_TYPE, std::function<void()> > mapTypeSetup = {
		{KNIGHT, [this]() -> void {this->setupKnightProblem(); }}
	};

	std::map < MAP_TYPE, std::function<void()> > mapTypeStart = {
		{KNIGHT, [this]() -> void {this->runKnightProblem(); }},
	};

	std::map< MAP_TYPE, std::function<void(wxButton*)> > mapTypeOnClick = {
		{KNIGHT, [this](wxButton* button) -> void {this->knightMapClick(button); }}
	};

	//backtrack algorithms
	//knight problem setups
	void setupKnightProblem();
	void runKnightProblem();
	void knightMapClick(wxButton* button);

	// Odziedziczono za pośrednictwem elementu AlgorithmFrame
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
	//

	void cellUncheckUpdate(wxThreadEvent& evt);
};

