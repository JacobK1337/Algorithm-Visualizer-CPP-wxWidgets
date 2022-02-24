#pragma once
#include<wx/wx.h>
#include<vector>
#include<queue>
#include"../interface/GraphAlgorithm.h"

class DijkstraSP : public GraphAlgorithm
{
private:
	std::unique_ptr<def_type::vector2DInt> costList;

	struct cellInfo {
		int parent;
		int cost;
		cellInfo() {}
		cellInfo(int t_parent, int t_cost) :
			parent(t_parent),
			cost(t_cost) {}
	};

	void dijkstra(std::vector<cellInfo>& finalPath, AlgorithmThread* workingThread);
	void showPathToSource(std::vector<cellInfo>& finalPath, AlgorithmThread* workingThread);

public:

	DijkstraSP(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler);
	~DijkstraSP();
	void setBlockedCells(def_type::vector1DBool& blockedButtons);
	virtual void generateValues(AlgorithmThread* workingThread) override;
	virtual void runAlgorithm(AlgorithmThread* workingThread) override;
	virtual void setSource(const int& t_newSource) override;
	virtual void setDest(const int& t_newDest) override;
	virtual bool isSafe(const int& i, const int& j) override;
};