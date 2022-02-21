#include "RIMimpl.h"

RIMimpl::RIMimpl(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler) : Algorithm(MAP_ROWS, MAP_COLS, handler)
{
	//before blocking all cells are valid
	map = std::make_unique<std::vector<int>>(m_MAP_ROWS * m_MAP_COLS, 1);
	RIMimpl::blockRandomCells();
}

RIMimpl::~RIMimpl()
{
}

void RIMimpl::runAlgorithm(AlgorithmThread* workingThread)
{
	std::vector<bool> solution(m_MAP_ROWS * m_MAP_COLS, false);
	RIMimpl::ratInAmaze(0, solution, workingThread);
}

bool RIMimpl::ratInAmaze(const int& FIRST_DIM_EQ, std::vector<bool>& solution, AlgorithmThread* workingThread)
{
	if (FIRST_DIM_EQ == m_MAP_ROWS * m_MAP_COLS - 1) {
		solution[FIRST_DIM_EQ] = true;
		return true;
	}

	if (isSafe(FIRST_DIM_EQ / m_MAP_COLS, FIRST_DIM_EQ % m_MAP_COLS)) {
		if (solution[FIRST_DIM_EQ])
			return false;

		solution[FIRST_DIM_EQ] = true;

		THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, "", wxColour(153, 204, 0));
		evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
		wxMilliSleep(200);

		if (ratInAmaze(FIRST_DIM_EQ + m_MAP_COLS, solution, workingThread))
			return true;
		
		if (ratInAmaze(FIRST_DIM_EQ + 1, solution, workingThread))
			return true;
		
		THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, "", wxColour(255, 255, 255));
		evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
		wxMilliSleep(200);

		solution[FIRST_DIM_EQ] = false;

		return false;
	}

	return false;
}

void RIMimpl::generateValues(AlgorithmThread* workingThread)
{

	//value generation depends on a map; if map[cellNum] is 0 then cell is blocked, otherwise cell is valid.
	//source and destination are always valid.

	(*map)[0] = -1; //source
	(*map)[m_MAP_ROWS * m_MAP_COLS - 1] = -2; //destination

	for (int i = 0; i < m_MAP_ROWS; i++)
		for (int j = 0; j < m_MAP_COLS; j++) {
			const int FIRST_DIM_EQ = i * m_MAP_COLS + j;
			std::string updatedValue;
			wxColour updatedColour;
			switch ((*map)[FIRST_DIM_EQ]) {
			case -1:
				updatedValue = "Source";
				updatedColour = wxColour(153, 0, 255);
				break;
			case -2:
				updatedValue = "Dest";
				updatedColour = wxColour(153, 0, 255);
				break;
			case 1:
				updatedValue = "";
				updatedColour = wxColour(255, 255, 255);
				break;
			case 0:
				updatedValue = "";
				updatedColour = wxColour(0, 0, 0);
				break;
			}
			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, updatedValue, updatedColour);
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
		}
}



void RIMimpl::blockRandomCells() {

	int numOfBlockedCells = 15;

	srand(time(NULL));
	int cellToBlock;
	while (numOfBlockedCells) {

		cellToBlock = rand() % (m_MAP_ROWS * m_MAP_COLS - 1);

		if (cellToBlock != 0 && cellToBlock != m_MAP_ROWS * m_MAP_COLS - 1) {
			numOfBlockedCells--;
			(*map)[cellToBlock] = 0;
		}
	}
}

bool RIMimpl::isSafe(const int& x, const int& y)
{
	return ((x >= 0 && y >= 0) && (x < m_MAP_ROWS&& y < m_MAP_COLS) && abs((*map)[x * m_MAP_COLS + y]) == 1);
}
