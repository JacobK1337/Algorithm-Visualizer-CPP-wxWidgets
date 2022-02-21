#pragma once
#include<wx/wx.h>
#include<vector>
#include"GraphAlgorithm.h"
class DFSimpl : public GraphAlgorithm
{
private:
	
	struct cellInfo {
		int parent;
		int when;
		cellInfo() {}
		cellInfo(int t_parent, int t_when) :
			parent(t_parent),
			when(t_when){}
	};

	def_type::vector1DBool mapBlockedCells;
	std::unique_ptr<def_type::CELL_UPDATE_INFO> THREAD_DATA;
	
	void dfs(const int& src, std::vector<cellInfo>& finalPath, std::vector<bool>& visited, AlgorithmThread* workingThread);


public:
	DFSimpl(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler);
	~DFSimpl();
	int getSource();
	void setBlockedCells(def_type::vector1DBool& blockedCells);

	// Odziedziczono za poœrednictwem elementu GraphAlgorithm
	virtual void generateValues(AlgorithmThread* workingThread) override;
	virtual void runAlgorithm(AlgorithmThread* workingThread) override;
	virtual void setSource(const int& t_newSource) override;
	void setDest(const int& t_newDest);
	virtual void showPathToSource(const int& t_vertexFrom, AlgorithmThread* workingThread) override;
	void showPathToSource(std::vector<cellInfo>& finalPath, AlgorithmThread* workingThread);
	virtual void addNeighbours(const int& i, const int& j) override;
	virtual void applyAdjList() override;
	virtual bool isSafe(const int& i, const int& j) override;
};

