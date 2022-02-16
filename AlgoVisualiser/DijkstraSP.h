#pragma once
#include<wx/wx.h>
#include<vector>
#include<queue>
#include"defined_typenames.h"
#include"declared_events.h"

class DijkstraSP
{
private:
	
	int VERTEX_COUNT;
	int source = -1;

	std::unique_ptr<def_type::vector2DPair> adjList;
	std::unique_ptr<def_type::vector1DInt> ancestor;
	std::unique_ptr<def_type::vector1DInt> shortestDistance;
	def_type::vector2DInt costList;

	def_type::vector1DBool mapButtonBlocked;

	wxEvtHandler* parentEvtHandler = nullptr;
	std::unique_ptr<def_type::CELL_UPDATE_INFO> THREAD_DATA;

	void addNeighbours(int i, int j);
	void applyAdjList();
	bool isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT);

public:
	DijkstraSP(const int t_MAP_ROWS, const int t_MAP_COLS, wxEvtHandler* handler);
	~DijkstraSP();

	void setCostList(def_type::vector2DInt& costList);
	void setBlockedCells(def_type::vector1DBool& blockedButtons);
	void incrementCellCost(const int i, const int j);
	int checkCellCost(const int i, const int j);
	void setSource(int src);
	int getSource();
	const int getShortestDistance(const int FIRST_DIM_EQ);
	wxThread::ExitCode runDijkstraAlgorithm(wxThread* workingThread);
	void showPathToSource(int t_vertexFrom);
};