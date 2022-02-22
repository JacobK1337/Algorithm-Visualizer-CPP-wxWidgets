#include "RIMimpl.h"

RIMimpl::RIMimpl(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler) : GraphAlgorithm(MAP_ROWS, MAP_COLS, handler)
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
	def_type::vector1DBool solution(m_MAP_ROWS * m_MAP_COLS, false);
	RIMimpl::ratInAmaze(0, solution, workingThread);
}

void RIMimpl::setSource(const int& t_newSource)
{
	m_source = t_newSource;
}

void RIMimpl::setDest(const int& t_newDest)
{
	m_dest = t_newDest;
}

bool RIMimpl::ratInAmaze(const int& FIRST_DIM_EQ, def_type::vector1DBool& solution, AlgorithmThread* workingThread)
{
	if (FIRST_DIM_EQ == m_MAP_ROWS * m_MAP_COLS - 1) {
		solution[FIRST_DIM_EQ] = true;
		return true;
	}

	if (workingThread->TestDestroy()) {
		workingThread->flagThreadBreak(true);
		return false;
	}


	if (isSafe(FIRST_DIM_EQ / m_MAP_COLS, FIRST_DIM_EQ % m_MAP_COLS) && !workingThread->TestDestroy()) {
		if (solution[FIRST_DIM_EQ])
			return false;

		solution[FIRST_DIM_EQ] = true;

		//setting the cell as visited
		THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, "", def_col::VISITED_COLOUR);
		evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
		wxMilliSleep(200);

		if (ratInAmaze(FIRST_DIM_EQ + m_MAP_COLS, solution, workingThread))
			return true;
		
		if (ratInAmaze(FIRST_DIM_EQ + 1, solution, workingThread))
			return true;
		
		//no solution, removing the cell from solution path
		THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, "", def_col::IDLE_COLOUR);
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

	for (int i = 0; i < m_MAP_ROWS; i++)
		for (int j = 0; j < m_MAP_COLS; j++) {

			const int FIRST_DIM_EQ = i * m_MAP_COLS + j;
			std::string updatedValue = "";
			wxColour updatedColour;

			switch ((*map)[FIRST_DIM_EQ]) {
			case 1:
				updatedColour = wxColour(255, 255, 255);
				break;
			case 0:
				updatedColour = wxColour(0, 0, 0);
				break;
			}

			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, updatedValue, updatedColour);
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
		}

	//setting the source and destination;
	RIMimpl::setSource(0);
	RIMimpl::setDest(m_MAP_ROWS * m_MAP_COLS - 1);
	evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, def_type::CELL_UPDATE_INFO(0, "Source", def_col::SOURCE_COLOUR));
	evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, def_type::CELL_UPDATE_INFO(m_MAP_ROWS * m_MAP_COLS - 1, "Dest", def_col::DEST_COLOUR));
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
	return ((x >= 0 && y >= 0) && (x < m_MAP_ROWS&& y < m_MAP_COLS) && (*map)[x * m_MAP_COLS + y] == 1);
}
