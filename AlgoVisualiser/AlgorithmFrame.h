#pragma once
#include<wx/wx.h>
#include<map>
#include<vector>
#include"declared_events.h"
#include"defined_typenames.h"
#include"SideThread.h"
class AlgorithmFrame : public wxFrame
{
protected:

	const enum MAP_STATE { IDLE, CHOOSING_SOURCE, RUNNING, FINISHED };
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
	virtual void fixOldSource(const int t_oldSource, const int t_valueToSet) = 0; 

	virtual void assignAlgorithmThread(const std::function<void()>& runFunction) = 0; 
	virtual void onThreadEnd(wxCommandEvent& evt) = 0;

	virtual void choiceSelected(wxCommandEvent& evt) = 0;
	virtual void mapButtonClicked(wxCommandEvent& evt) = 0;
	virtual void rightButtonClicked(wxMouseEvent& evt) = 0;
	virtual void sourceSetButtonClicked(wxCommandEvent& evt) = 0;

	virtual void cellVisitedUpdate(wxThreadEvent& evt) = 0;
	virtual void cellPathToSourceUpdate(wxCommandEvent& evt) = 0;
	virtual void updateCellColor(const int& FIRST_DIM_EQ, wxColour const& t_newColour) = 0;
	virtual void updateCellValue(const int& FIRST_DIM_EQ, wxString const& t_newValue) = 0;

public:
	AlgorithmFrame(wxString frameTitle, wxPoint point, wxSize size);
	~AlgorithmFrame();
};

