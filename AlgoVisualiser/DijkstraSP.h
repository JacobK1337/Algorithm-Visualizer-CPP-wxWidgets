#pragma once
#include<wx/wx.h>
#include<vector>
#include<queue>

class DijkstraSP
{

private:
	int VERTEX_COUNT;
	int source = -1;
	std::vector<std::vector<std::pair<int,int>>>* adjList = nullptr;
	std::vector<int>* ancestor = nullptr;
	std::vector<std::vector<int>> costList;
	std::vector<bool> mapButtonBlocked;
	wxButton** mapButtons = nullptr;

private:
	void addNeighbours(int i, int j);
	void applyAdjList();
	bool isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT);

public:
	DijkstraSP(wxButton** buttons, int VERTEX_COUNT);
	~DijkstraSP();

	void setCostList(std::vector<std::vector<int>>& costList);
	void setBlockedCells(std::vector<bool>& blockedButtons);
	void incrementCellCost(const int i, const int j);
	int checkCellCost(const int i, const int j);
	void setSource(int src);
	int getSource();
	void runDijkstraAlgorithm();
	void showPathToSource(int t_vertexFrom);
};

