#pragma once
#include<wx/wx.h>
#include<vector>
#include<queue>

class BFSimpl
{

private:
	typedef std::vector<std::pair<int, int>> vector1DPair;
	typedef std::vector<int> vector1DInt;
	typedef std::vector<bool> vector1DBool;
	typedef std::vector<vector1DPair> vector2DPair;
	typedef std::vector<vector1DInt> vector2DInt;

	int MAP_ROWS;
	int MAP_COLS;
	int source = -1;

	std::unique_ptr<vector2DInt> adjList;
	std::unique_ptr<vector1DBool> visList;
	std::unique_ptr<vector1DInt> ancestor;

	wxButton** mapButtons = nullptr;
	vector1DBool mapBlockedCells;

private:
	void bfs(int src);
	void addNeighbours(int i, int j);
	void applyAdjList();
	bool isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT);
public:
	void runBfsAlgorithm();
	void setSource(int src);
	int getSource();
	void setBlockedCells(std::vector<bool>& blockedCells);
	void showPathToSource(int t_vertexFrom);
	BFSimpl(wxButton** buttons, int MAP_ROWS, int MAP_COLS);
	~BFSimpl();

};

