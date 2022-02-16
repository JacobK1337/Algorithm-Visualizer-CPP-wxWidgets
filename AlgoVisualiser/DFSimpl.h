#pragma once
#include<wx/wx.h>
#include<vector>
#include"defined_typenames.h"
#include"declared_events.h"
class DFSimpl
{
private:

	int MAP_ROWS;
	int MAP_COLS;
	int source = -1;

	std::unique_ptr<def_type::vector2DInt> adjList;
	std::unique_ptr<def_type::vector1DBool> visList;
	std::unique_ptr<def_type::vector1DInt> ancestor;

	def_type::vector1DBool mapBlockedCells;

	wxEvtHandler* parentEvtHandler = nullptr;
	std::unique_ptr<def_type::CELL_UPDATE_INFO> THREAD_DATA;

	void dfs(int src);
	void addNeighbours(int i, int j);
	void applyAdjList();
	bool isSafe(int i, int j);

public:
	void runDfsAlgorithm();
	void setSource(int src);
	int getSource();
	void setBlockedCells(def_type::vector1DBool& blockedCells);
	void showPathToSource(int t_vertexFrom);
	DFSimpl(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler);
	~DFSimpl();
};

