#pragma once
#include<wx/wx.h>
#include<vector>
#include<queue>
#include"GraphAlgorithm.h"

class DijkstraSP : public GraphAlgorithm
{
private:
	std::unique_ptr<def_type::vector2DPair> adjList;
	std::unique_ptr<def_type::vector1DInt> ancestor;
	std::unique_ptr<def_type::vector1DInt> shortestDistance;
	std::unique_ptr<def_type::vector2DInt> costList;

	def_type::vector1DBool mapButtonBlocked;
	std::unique_ptr<def_type::CELL_UPDATE_INFO> THREAD_DATA;

	void dijkstra(AlgorithmThread* workingThread);

public:
	DijkstraSP(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler);
	~DijkstraSP();

	void setBlockedCells(def_type::vector1DBool& blockedButtons);

	int getSource();
	const int getShortestDistance(const int FIRST_DIM_EQ);

	// Odziedziczono za poœrednictwem elementu GraphAlgorithm
	virtual void generateValues(AlgorithmThread* workingThread) override;
	virtual void runAlgorithm(AlgorithmThread* workingThread) override;
	virtual void setSource(const int& t_newSource) override;
	virtual void showPathToSource(const int& t_vertexFrom, AlgorithmThread* workingThread) override;
	virtual void addNeighbours(const int& i, const int& j) override;
	virtual void applyAdjList() override;
	virtual bool isSafe(const int& i, const int& j) override;
};