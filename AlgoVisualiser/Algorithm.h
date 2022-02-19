#pragma once
#include<wx/wx.h>
#include"AlgorithmThread.h"
#include"defined_typenames.h"
#include"declared_events.h"
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
	wxEvtHandler* m_parentEventHandler;
};

