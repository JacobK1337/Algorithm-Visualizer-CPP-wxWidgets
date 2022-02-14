#pragma once
#include<wx/wx.h>
#include<vector>
#include"defined_typenames.h"
#include"declared_events.h"
class DFSimpl
{
private:

	const int MAP_UPDATE_REQUEST_ID = 5556;
	const int MAP_RECON_REQUEST_ID = 5557;

	int MAP_ROWS;
	int MAP_COLS;
	int source = -1;

	std::unique_ptr<def_type::vector2DInt> adjList;
	std::unique_ptr<def_type::vector1DBool> visList;
	std::unique_ptr<def_type::vector1DInt> ancestor;

	wxEvtHandler* parentEvtHandler = nullptr;
	def_type::vector1DBool mapBlockedCells;

private:

	void dfs(int src);
	void addNeighbours(int i, int j);
	void applyAdjList();
	bool isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT);

public:
	void runDfsAlgorithm();
	void setSource(int src);
	int getSource();
	void setBlockedCells(def_type::vector1DBool& blockedCells);
	void showPathToSource(int t_vertexFrom);
	DFSimpl(int MAP_ROWS, int MAP_COLS, wxEvtHandler* handler);
	~DFSimpl();
};

