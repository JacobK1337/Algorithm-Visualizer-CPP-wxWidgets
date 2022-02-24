#pragma once
#include"../interface/GraphAlgorithm.h"
class RIMimpl : public GraphAlgorithm
{
	// Odziedziczono za poœrednictwem elementu Algorithm
public:
	RIMimpl(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler);
	~RIMimpl();
	virtual void generateValues(AlgorithmThread* workingThread) override;
	virtual void runAlgorithm(AlgorithmThread* workingThread) override;

private:
	std::unique_ptr<def_type::vector1DInt> map;
	virtual void setSource(const int& t_newSource) override;
	virtual void setDest(const int& t_newDest) override;
	virtual bool isSafe(const int& x, const int& y) override;

	bool ratInAmaze(const int& FIRST_DIM_EQ, def_type::vector1DBool& solution, AlgorithmThread* workingThread);
	void blockRandomCells();

};

