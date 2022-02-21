#pragma once
#include"GraphAlgorithm.h"
#include<vector>
#include<queue>
#include<sstream>
#include<iomanip>
class AStar : public GraphAlgorithm
{
public:
	AStar(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler);
	~AStar();

	// Odziedziczono za poœrednictwem elementu GraphAlgorithm
	virtual void generateValues(AlgorithmThread* workingThread) override;
	virtual void runAlgorithm(AlgorithmThread* workingThread) override;
	virtual void setSource(const int& t_newSource) override;
	virtual void setDest(const int& t_newDest) override;
	void setBlockedCells(def_type::vector1DBool blockedCells);

private:
	struct cellInfo {
		int parent;
		double f, d, h;
		cellInfo(){}
		cellInfo(int t_parent, double t_newF, double t_newD, double t_newH) :
			parent(t_parent),
			f(t_newF),
			d(t_newD),
			h(t_newH){}
	};
	def_type::vector1DBool mapCellBlocked;
	std::unique_ptr<def_type::CELL_UPDATE_INFO> THREAD_DATA;

	void aStarSearch(AlgorithmThread* workingThread);

	//heuristic value, just the distance between v and m_dest, excluding all blocked cells in the way.
	double getH(const int& v);

	virtual bool isSafe(const int& i, const int& j) override;
	bool isSafe(const int& FIRST_DIM_EQ);

public:
	void showPathToSource(std::vector<cellInfo>& finalPath, AlgorithmThread* workingThread);

};

