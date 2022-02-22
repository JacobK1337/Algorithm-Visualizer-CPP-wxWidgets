#include "KTPImpl.h"

KTPImpl::KTPImpl(const int MAP_ROWS, const int MAP_COLS, wxEvtHandler* handler) : GraphAlgorithm(MAP_ROWS, MAP_COLS, handler)
{
}

KTPImpl::~KTPImpl()
{

}

void KTPImpl::setSource(const int& t_newSource)
{
	m_source = t_newSource;
}

void KTPImpl::setDest(const int& t_newDest) {
	m_dest = t_newDest;
}

void KTPImpl::runAlgorithm(AlgorithmThread* workingThread)
{
	def_type::vector2DInt solution(m_MAP_ROWS, def_type::vector1DInt(m_MAP_COLS, -1));
	def_type::vector1DInt xShift = { 2, 1, -1, -2, -2, -1, 1, 2 };
	def_type::vector1DInt yShift = { 1, 2, 2, 1, -1, -2, -2, -1 };

	const int srcX = m_source / m_MAP_COLS;
	const int srcY = m_source % m_MAP_COLS;

	solution[srcX][srcY] = 0;

	m_workingThread = workingThread;
	KTPImpl::findSolution(srcX, srcY, 1, solution, xShift, yShift);
}



bool KTPImpl::isSafe(const int& x, const int& y, def_type::vector2DInt& solution)
{
	return ((x >= 0 && y >= 0) && (x < m_MAP_ROWS&& y < m_MAP_COLS) && solution[x][y] == -1);
}

bool KTPImpl::findSolution(const int& x, const int& y, const int& numOfVisited, def_type::vector2DInt& solution, def_type::vector1DInt& xShift, def_type::vector1DInt& yShift)
{
	const int FIRST_DIM_EQ = x * m_MAP_COLS + y;


	if (!m_workingThread->TestDestroy())
	{

		THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, std::to_string(numOfVisited), def_col::VISITED_COLOUR);
		evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
		wxMilliSleep(50);

	}

	else 
	{
		m_workingThread->flagThreadBreak(true);
		return false;
	}

	if (numOfVisited == m_MAP_ROWS * m_MAP_COLS)
		return true;

	for (int i = 0; i < 8; i++) {
		int newX = x + xShift[i];
		int newY = y + yShift[i];

		if (isSafe(newX, newY, solution)) {
			const int n_FIRST_DIM_EQ = newX * m_MAP_COLS + newY;
			solution[newX][newY] = numOfVisited;

			if (KTPImpl::findSolution(newX, newY, numOfVisited + 1, solution, xShift, yShift))
				return true;

			else {
				solution[newX][newY] = -1;

				THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(n_FIRST_DIM_EQ, "", def_col::IDLE_COLOUR);
				evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);

				wxMilliSleep(50);
			}
		}
	}

	return false;
}

void KTPImpl::generateValues(AlgorithmThread* workingThread)
{
	for (int i = 0; i < m_MAP_ROWS; i++)
		for (int j = 0; j < m_MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * m_MAP_COLS + j;
			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, "", def_col::IDLE_COLOUR);
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
		}
}

bool KTPImpl::isSafe(const int& i, const int& j)
{
	return false;
}


