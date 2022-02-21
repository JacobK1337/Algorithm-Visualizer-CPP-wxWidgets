#pragma once
#include"Algorithm.h"
class RIMimpl : public Algorithm
{
	// Odziedziczono za poœrednictwem elementu Algorithm
public:
	RIMimpl(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler);
	~RIMimpl();
	virtual void generateValues(AlgorithmThread* workingThread) override;
	virtual void runAlgorithm(AlgorithmThread* workingThread) override;

private:
	std::unique_ptr<def_type::CELL_UPDATE_INFO> THREAD_DATA;
	std::unique_ptr<std::vector<int>> map;
	bool ratInAmaze(const int& FIRST_DIM_EQ, std::vector<bool>& solution, AlgorithmThread* workingThread);
	void blockRandomCells();
	bool isSafe(const int& x, const int& y);

};

