#pragma once
#include<wx/wx.h>
#include"defined_typenames.h"
#include"declared_events.h"
#include<assert.h>
class KTPImpl
{
public:
	KTPImpl(const int MAP_ROWS, const int MAP_COLS, wxEvtHandler* handler);
	~KTPImpl();

	int getSource();
	void setSource(const int t_newSource);
	void runAlgorithm();
private:
	wxEvtHandler* parentEvtHandler = nullptr;
	int p_source = -1;
	const int MAP_COLS;
	const int MAP_ROWS;
	
	bool isSafe(const int x, const int y, def_type::vector2DInt& solution);
	bool findSolution(int x, int y, int numOfVisited, def_type::vector2DInt& solution, def_type::vector1DInt& xShift, def_type::vector1DInt& yShift);

};

