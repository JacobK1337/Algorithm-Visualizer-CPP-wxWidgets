#pragma once
#include<wx/wx.h>
#include<vector>
#include<queue>
#include"GraphAlgorithm.h"

class DijkstraSP : public GraphAlgorithm
{
private:
	/*
	std::unique_ptr<def_type::vector2DPair> adjList;
	std::unique_ptr<def_type::vector1DInt> ancestor;
	std::unique_ptr<def_type::vector1DInt> shortestDistance;
	*/
	std::unique_ptr<def_type::vector2DInt> costList;

	struct cellInfo {
		int parent;
		int cost;
		cellInfo() {}
		cellInfo(int t_parent, int t_cost) :
			parent(t_parent),
			cost(t_cost) {}
	};

	def_type::vector1DBool mapButtonBlocked;
	std::unique_ptr<def_type::CELL_UPDATE_INFO> THREAD_DATA;

	void dijkstra(std::vector<cellInfo>& finalPath, AlgorithmThread* workingThread);

public:
	DijkstraSP(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler);
	~DijkstraSP();

	void setBlockedCells(def_type::vector1DBool& blockedButtons);

	int getSource();

	// Odziedziczono za poœrednictwem elementu GraphAlgorithm
	virtual void generateValues(AlgorithmThread* workingThread) override;
	virtual void runAlgorithm(AlgorithmThread* workingThread) override;
	virtual void setSource(const int& t_newSource) override;
	void setDest(const int& t_newDest);
	virtual void showPathToSource(const int& t_vertexFrom, AlgorithmThread* workingThread) override;
	void showPathToSource(std::vector<cellInfo>& finalPath, AlgorithmThread* workingThread);
	virtual void addNeighbours(const int& i, const int& j) override;
	virtual void applyAdjList() override;
	virtual bool isSafe(const int& i, const int& j) override;
};