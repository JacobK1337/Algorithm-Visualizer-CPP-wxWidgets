#pragma once
#include"Algorithm.h"
class GraphAlgorithm : public Algorithm
{
public:
	GraphAlgorithm(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* eventHandler);
	~GraphAlgorithm();
	virtual void setSource(const int& t_newSource) = 0;
	virtual void showPathToSource(const int& t_vertexFrom, AlgorithmThread* workingThread) = 0;

protected:
	int m_source = -1;
	int m_dest = -1;
	virtual void addNeighbours(const int& i, const int& j) = 0;
	virtual void applyAdjList() = 0;
	virtual bool isSafe(const int& i, const int& j) = 0;
};

