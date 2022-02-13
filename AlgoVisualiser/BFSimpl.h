#pragma once
#include<wx/wx.h>
#include<vector>
#include<queue>

class BFSimpl
{

private:
	int MAP_ROWS;
	int MAP_COLS;
	int source = -1;
	std::vector<std::vector<int>>* adjList = nullptr;
	std::vector<bool>* visList = nullptr;
	wxButton** mapButtons = nullptr;

private:
	void bfs(int src);
	void addNeighbours(int i, int j);
	void applyAdjList();
	bool isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT);
public:
	void runBfsAlgorithm();
	void setSource(int src);
	int getSource();
	BFSimpl(wxButton** buttons, int MAP_ROWS, int MAP_COLS);
	~BFSimpl();

};

