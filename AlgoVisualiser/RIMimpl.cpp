#include "RIMimpl.h"

RIMimpl::RIMimpl(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler) : GraphAlgorithm(MAP_ROWS, MAP_COLS, handler)
{
	//before blocking all cells are valid
	map = std::make_unique<def_type::vector1DInt>(m_MAP_ROWS * m_MAP_COLS, 1);
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

bool RIMimpl::ratInAmaze(const int& cellNum, def_type::vector1DBool& solution, AlgorithmThread* workingThread)
{
	if (cellNum == m_MAP_ROWS * m_MAP_COLS - 1) {
		solution[cellNum] = true;
		return true;
	}

	if (workingThread->TestDestroy()) {
		workingThread->flagThreadBreak(true);
		return false;
	}


	if (isSafe(cellNum / m_MAP_COLS, cellNum % m_MAP_COLS) && !workingThread->TestDestroy()) {
		if (solution[cellNum])
			return false;

		//setting the cell as visited
		solution[cellNum] = true;


		if (workingThread->TestDestroy()) {
			workingThread->flagThreadBreak(true);
			return false;
		}

		animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_YELLOW, 
			cellNum, 
			"", 
			animation::DEFAULT_DELAY, 
			m_parentEventHandler);

		if (ratInAmaze(cellNum + m_MAP_COLS, solution, workingThread))
			return true;
		
		if (ratInAmaze(cellNum + 1, solution, workingThread))
			return true;
		
		//no solution, removing the cell from solution path
		
		animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_WHITE, cellNum, "", animation::DEFAULT_DELAY, m_parentEventHandler);
		solution[cellNum] = false;

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

			const int cellNum = i * m_MAP_COLS + j;
			std::string updatedValue = "";
			wxColour updatedColour;

			switch ((*map)[cellNum]) {
			case 1:
				updatedColour = wxColour(255, 255, 255);
				break;
			case 0:
				updatedColour = wxColour(0, 0, 0);
				break;
			}

			animation::cellColorTransition(std::vector<wxColour>{updatedColour}, cellNum, "", 0, m_parentEventHandler);
		}

	//setting the source and destination;
	RIMimpl::setSource(0);
	RIMimpl::setDest(m_MAP_ROWS * m_MAP_COLS - 1);
	
	wxColour distinctColour = wxColour(153, 255, 0);
	animation::cellColorTransition({distinctColour}, 0, "Source", 0, m_parentEventHandler);
	animation::cellColorTransition({distinctColour}, m_MAP_ROWS* m_MAP_COLS - 1, "Dest", 0, m_parentEventHandler);
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
