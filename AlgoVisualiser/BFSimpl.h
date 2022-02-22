#pragma once
#include<wx/wx.h>
#include<vector>
#include<queue>
#include"GraphAlgorithm.h"
class BFSimpl : public GraphAlgorithm
{

private:
	struct cellInfo {
		int parent;
		int when;
		cellInfo() {}
		cellInfo(int t_parent, int t_when) :
			parent(t_parent),
			when(t_when) {}
	};

	void bfs(std::vector<cellInfo>& finalPath, std::vector<bool>& visited, AlgorithmThread* workingThread);
	void showPathToSource(std::vector<cellInfo>& finalPath, AlgorithmThread* workingThread);
	virtual bool isSafe(const int& i, const int& j) override;

public:
	BFSimpl(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler);
	~BFSimpl();
	void setBlockedCells(def_type::vector1DBool& blockedCells);
	virtual void generateValues(AlgorithmThread* workingThread) override;
	virtual void runAlgorithm(AlgorithmThread* workingThread) override;
	virtual void setSource(const int& t_newSource) override;
	virtual void setDest(const int& t_newDest) override;


};

