#pragma once
#include<wx/wx.h>
#include<vector>
using namespace std;

class DFSimpl
{

private:
	int MAP_ROWS;
	int MAP_COLS;
	vector<vector<int>>* adjList = nullptr;
	vector<bool>* visList = nullptr;
	wxButton** mapButtons = nullptr;

private:

	void dfs(int src);
	void addNeighbours(int i, int j);
	void applyAdjList();
	bool isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT);

public:
	void runDfsAlgorithm(int src);
	DFSimpl(wxButton** buttons, int MAP_ROWS, int MAP_COLS);
	~DFSimpl();
};

