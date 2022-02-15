#include "KTPImpl.h"

KTPImpl::KTPImpl(const int t_MAP_ROWS, const int t_MAP_COLS, wxEvtHandler* handler)
	: MAP_ROWS(t_MAP_ROWS), MAP_COLS(t_MAP_COLS), parentEvtHandler(handler)
{
}

KTPImpl::~KTPImpl()
{

}

int KTPImpl::getSource()
{
	return p_source;
}

void KTPImpl::setSource(const int t_newSource)
{
	p_source = t_newSource;
}

void KTPImpl::runAlgorithm()
{
	def_type::vector2DInt solution(MAP_ROWS, def_type::vector1DInt(MAP_COLS, -1));
	def_type::vector1DInt xShift = { 2, 1, -1, -2, -2, -1, 1, 2 };
	def_type::vector1DInt yShift = { 1, 2, 2, 1, -1, -2, -2, -1 };


	solution[2][2] = 0;
	KTPImpl::findSolution(2, 2, 1, solution, xShift, yShift);
	
}

bool KTPImpl::isSafe(const int x, const int y, def_type::vector2DInt& solution)
{
	return ((x >= 0 && y >= 0) && (x < MAP_ROWS && y < MAP_COLS) && solution[x][y] == -1);
}



bool KTPImpl::findSolution(int x, int y, int numOfVisited, def_type::vector2DInt& solution, def_type::vector1DInt& xShift, def_type::vector1DInt& yShift)
{
	
	const def_type::CELL_UPDATE_INFO NEW_DATA(x * MAP_COLS + y, numOfVisited, wxColour(204, 204, 0));
	wxThreadEvent mapUpdateEvt(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID);
	mapUpdateEvt.SetPayload(NEW_DATA);
	wxQueueEvent(parentEvtHandler, mapUpdateEvt.Clone());
	//wxMilliSleep(100);

	if (numOfVisited == MAP_ROWS * MAP_COLS) 
		return true;

	for (int i = 0; i < 8; i++) {
		int newX = x + xShift[i];
		int newY = y + yShift[i];

		if (isSafe(newX, newY, solution)) {

			solution[newX][newY] = numOfVisited;

			if (KTPImpl::findSolution(newX, newY, numOfVisited + 1, solution, xShift, yShift))
				return true;

			else {
				solution[newX][newY] = -1;

				const def_type::CELL_UPDATE_INFO UNCHECK_CELL_DATA(newX * MAP_COLS + newY, -1, wxColour(255, 255, 255));
				wxThreadEvent uncheckCellEvt(wxEVT_MAP_UNCHECK_REQUEST, evt_id::MAP_UNCHECK_REQUEST_ID);
				uncheckCellEvt.SetPayload(UNCHECK_CELL_DATA);
				wxQueueEvent(parentEvtHandler, uncheckCellEvt.Clone());
				//wxMilliSleep(100);
			}
		}
	}

	return false;
}

