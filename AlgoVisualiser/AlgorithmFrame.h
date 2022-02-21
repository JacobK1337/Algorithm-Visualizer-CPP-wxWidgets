#pragma once
#include<wx/wx.h>
#include<map>
#include<vector>
#include"declared_events.h"
#include"defined_typenames.h"
#include"AlgorithmThread.h"
#include"defined_colours.h"
class AlgorithmFrame : public wxFrame
{
protected:

	struct MapConfig {
		int MAP_ROWS;
		int MAP_COLS;
		std::string MAP_TITLE;
		MapConfig(){}
		MapConfig(int t_MAP_ROWS, int t_MAP_COLS, std::string t_MAP_TITLE) : 
			MAP_ROWS(t_MAP_ROWS), 
			MAP_COLS(t_MAP_COLS), 
			MAP_TITLE(t_MAP_TITLE) {}
	};

	const enum MAP_STATE { IDLE, CHOOSING_SOURCE, CHOOSING_DEST, RUNNING, FINISHED };
	virtual void setupMap() = 0;
	virtual void setupAlgorithmList() = 0;
	virtual void setupToolbar() = 0;

	//setupAlgorithms

	virtual void onStart(wxCommandEvent& evt) = 0;
	virtual void runAlgorithm() = 0;
	virtual void stopAlgorithm() = 0; 
	virtual void resetMap() = 0;

	//start algorithms

	virtual void setMapState(MAP_STATE t_mapState) = 0; 
	virtual void disableMapButtons() = 0;
	virtual void enableMapButtons() = 0;
	virtual void enableToolbarButtons() = 0;
	virtual void disableToolbarButtons() = 0;
	virtual void replaceSource(const int t_newSource, const std::string t_newSourceValue) = 0; 

	virtual void assignAlgorithmThread(const std::function<void()>& runFunction) = 0; 
	virtual void onThreadEnd(wxCommandEvent& evt) = 0;
	virtual void onThreadBreak(wxCommandEvent& evt) = 0;
	virtual void choiceSelected(wxCommandEvent& evt) = 0;
	virtual void mapButtonClicked(wxCommandEvent& evt) = 0;
	virtual void rightButtonClicked(wxMouseEvent& evt) = 0;
	virtual void sourceSetButtonClicked(wxCommandEvent& evt) = 0;

	virtual void cellVisitedUpdate(wxThreadEvent& evt) = 0;
	virtual void updateCellColor(const int& FIRST_DIM_EQ, wxColour const& t_newColour) = 0;
	virtual void updateCellValue(const int& FIRST_DIM_EQ, wxString const& t_newValue) = 0;

public:
	AlgorithmFrame(wxString frameTitle, wxPoint point, wxSize size);
	~AlgorithmFrame();
};

