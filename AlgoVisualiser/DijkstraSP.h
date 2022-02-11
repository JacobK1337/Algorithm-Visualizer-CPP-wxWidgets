#pragma once
#include<wx/wx.h>
#include<vector>
#include<queue>

class DijkstraSP
{

private:
	int VERTEX_COUNT;
	std::vector<std::vector<std::pair<int,int>>>* adjList = nullptr;
	std::vector<int>* ancestor = nullptr;
	std::vector<std::vector<int>> costList;
	wxButton** mapButtons = nullptr;

private:
	void addNeighbours(int i, int j);
	void applyAdjList();
	bool isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT);

public:
	DijkstraSP(wxButton** buttons, int VERTEX_COUNT);
	~DijkstraSP();

	void setCostList(std::vector<std::vector<int>>& costList);
	void runDijkstraAlgorithm(int src);
	void showPathToSource(int t_src, int t_vertexFrom);
};

