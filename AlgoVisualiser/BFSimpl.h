#pragma once
#include<wx/wx.h>
#include<vector>
#include<queue>
#include"GraphAlgorithm.h"
class BFSimpl : public GraphAlgorithm
{

private:

	std::unique_ptr<def_type::vector2DInt> adjList;
	std::unique_ptr<def_type::vector1DBool> visList;
	std::unique_ptr<def_type::vector1DInt> ancestor;

	std::unique_ptr<def_type::CELL_UPDATE_INFO> THREAD_DATA;
	def_type::vector1DBool mapBlockedCells;

	void bfs(const int &src, int& when, AlgorithmThread* workingThread);

public:

	BFSimpl(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler);
	~BFSimpl();
	int getSource();
	void setBlockedCells(def_type::vector1DBool& blockedCells);

	// Odziedziczono za poœrednictwem elementu GraphAlgorithm
	virtual void generateValues(AlgorithmThread* workingThread) override;
	virtual void runAlgorithm(AlgorithmThread* workingThread) override;
	virtual void setSource(const int& t_newSource) override;
	virtual void showPathToSource(const int& t_vertexFrom, AlgorithmThread* workingThread) override;
	virtual void addNeighbours(const int& i, const int& j) override;
	virtual void applyAdjList() override;
	virtual bool isSafe(const int& i, const int& j) override;

};

