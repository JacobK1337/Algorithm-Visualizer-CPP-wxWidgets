#pragma once
#include<wx/wx.h>
#include"GraphAlgorithm.h"
class KTPImpl : public GraphAlgorithm
{
public:
	KTPImpl(const int MAP_ROWS, const int MAP_COLS, wxEvtHandler* handler);
	~KTPImpl();

	int getSource();
	//void setSource(const int t_newSource);

	virtual void runAlgorithm(AlgorithmThread* workingThread) override;
	virtual void setSource(const int& t_newSource) override;
	virtual void generateValues(AlgorithmThread* workingThread) override;
	virtual void showPathToSource(const int& t_vertexFrom, AlgorithmThread* workingThread) override;

private:
	
	std::unique_ptr<def_type::CELL_UPDATE_INFO> THREAD_DATA;
	AlgorithmThread* m_workingThread = nullptr;

	bool isSafe(const int& x, const int& y, def_type::vector2DInt& solution);
	bool findSolution(const int& x, const int& y, const int& numOfVisited, def_type::vector2DInt& solution, def_type::vector1DInt& xShift, def_type::vector1DInt& yShift);

	virtual void addNeighbours(const int& i, const int& j) override;
	virtual void applyAdjList() override;
	virtual bool isSafe(const int& i, const int& j) override;

};

