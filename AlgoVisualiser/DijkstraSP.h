#pragma once
#include<wx/wx.h>
#include<vector>
#include<queue>
using namespace std;

class DijkstraSP
{

private:
	int numOfVertexes;
	vector<vector<pair<int,int>>>* adjList = nullptr;
	wxThread* spThread = nullptr;

public:
	DijkstraSP(vector<vector<int>>* costList);
	~DijkstraSP();

	void runDijkstraAlgorithm(int src);
	bool isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT);
	void addNeighbours(int i, int j, vector<vector<int>>& costList);
};

