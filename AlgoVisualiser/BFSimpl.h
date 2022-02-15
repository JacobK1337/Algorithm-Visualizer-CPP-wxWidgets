#pragma once
#include<wx/wx.h>
#include<vector>
#include<queue>
#include"defined_typenames.h"
#include"declared_events.h"

class BFSimpl
{

private:
	
	int MAP_ROWS;
	int MAP_COLS;
	int source = -1;

	std::unique_ptr<def_type::vector2DInt> adjList;
	std::unique_ptr<def_type::vector1DBool> visList;
	std::unique_ptr<def_type::vector1DInt> ancestor;

	wxEvtHandler* parentEvtHandler = nullptr;
	def_type::vector1DBool mapBlockedCells;

	void bfs(int src);
	void addNeighbours(int i, int j);
	void applyAdjList();
	bool isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT);

public:
	void runBfsAlgorithm();
	void setSource(int src);
	int getSource();
	void setBlockedCells(def_type::vector1DBool& blockedCells);
	void showPathToSource(int t_vertexFrom);
	BFSimpl(const int MAP_ROWS, int const MAP_COLS, wxEvtHandler* handler);
	~BFSimpl();

};

