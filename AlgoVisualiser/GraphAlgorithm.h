#pragma once
#include"Algorithm.h"
class GraphAlgorithm : public Algorithm
{
public:
	GraphAlgorithm(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* eventHandler);
	~GraphAlgorithm();
	virtual void setSource(const int& t_newSource) = 0;
	virtual void setDest(const int& t_newDest) = 0;

protected:
	int m_source = -1;
	int m_dest = -1;
	virtual bool isSafe(const int& i, const int& j) = 0;
};

