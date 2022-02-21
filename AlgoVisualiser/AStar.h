#pragma once
#include"GraphAlgorithm.h"
#include<vector>
#include<queue>
class AStar : public GraphAlgorithm
{
public:
	AStar(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler);
	~AStar();

	// Odziedziczono za poœrednictwem elementu GraphAlgorithm
	virtual void generateValues(AlgorithmThread* workingThread) override;
	virtual void runAlgorithm(AlgorithmThread* workingThread) override;
	virtual void setSource(const int& t_newSource) override;
	void setDest(const int& t_newDest);
	virtual void showPathToSource(const int& t_vertexFrom, AlgorithmThread* workingThread) override;
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

	virtual void addNeighbours(const int& i, const int& j) override;
	virtual void applyAdjList() override;
	bool isSafe(const int& FIRST_DIM_EQ);
	virtual bool isSafe(const int& i, const int& j) override;

	void aStarSearch(AlgorithmThread* workingThread);
	double getH(const int& v);

public:
	void showPathToSource(std::vector<cellInfo>& finalPath, AlgorithmThread* workingThread);
};

