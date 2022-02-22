#pragma once
#include<wx/wx.h>
#include"AlgorithmThread.h"
#include"animation.h"

class Algorithm
{
public:
	Algorithm(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* eventHandler);
	~Algorithm();
	virtual void generateValues(AlgorithmThread* workingThread) = 0;
	virtual void runAlgorithm(AlgorithmThread* workingThread) = 0;

protected:
	const int m_MAP_ROWS;
	const int m_MAP_COLS;
	std::unique_ptr<def_type::CELL_UPDATE_INFO> THREAD_DATA;
	wxEvtHandler* m_parentEventHandler;
};

