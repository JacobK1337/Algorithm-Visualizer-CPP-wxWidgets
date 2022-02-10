#pragma once
#include<wx/wx.h>
#include<vector>
#include<queue>
using namespace std;

class DijkstraSP
{

private:
	int VERTEX_COUNT;
	vector<vector<pair<int,int>>>* adjList = nullptr;
	vector<vector<int>> costList;
	wxButton** mapButtons = nullptr;

private:
	void addNeighbours(int i, int j);
	void applyAdjList();
	bool isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT);

public:
	DijkstraSP(wxButton** buttons, int VERTEX_COUNT);
	~DijkstraSP();

	void setCostList(vector<vector<int>>& costList);
	void runDijkstraAlgorithm(int src);
};

