#pragma once
#include<wx/wx.h>
#include<vector>

class DFSimpl
{

private:
	int MAP_ROWS;
	int MAP_COLS;
	int source = -1;
	std::vector<std::vector<int>>* adjList = nullptr;
	std::vector<bool>* visList = nullptr;
	std::vector<int>* ancestor = nullptr;
	wxButton** mapButtons = nullptr;
	std::vector<bool> mapBlockedCells;

private:

	void dfs(int src);
	void addNeighbours(int i, int j);
	void applyAdjList();
	bool isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT);

public:
	void runDfsAlgorithm();
	void setSource(int src);
	int getSource();
	void setBlockedCells(std::vector<bool>& blockedCells);
	void showPathToSource(int t_vertexFrom);
	DFSimpl(wxButton** buttons, int MAP_ROWS, int MAP_COLS);
	~DFSimpl();
};

