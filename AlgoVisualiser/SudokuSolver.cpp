#include "SudokuSolver.h"

SudokuSolver::SudokuSolver(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler) : Algorithm(MAP_ROWS, MAP_COLS, handler)
{
	sudokuMap = std::make_unique<def_type::vector2DInt>(m_MAP_ROWS, def_type::vector1DInt(m_MAP_COLS, -1));
	inDiagonalSquare = std::make_unique<def_type::vector2DBool>(m_MAP_ROWS, def_type::vector1DBool(m_MAP_COLS, false));
	m_DIAG_SQRT = sqrt(m_MAP_ROWS);
	rowFilled = std::make_unique<def_type::vector1DInt>(m_MAP_ROWS, 0);
	colFilled = std::make_unique<def_type::vector1DInt>(m_MAP_COLS, 0);
}

SudokuSolver::~SudokuSolver()
{

}

void SudokuSolver::runAlgorithm(AlgorithmThread* workingThread)
{

	int startPoint = 0;
	while ((*sudokuMap)[startPoint / m_MAP_ROWS][startPoint % m_MAP_COLS] != -1)
		startPoint++;


	//if randomly a whole row or column is correctly filled, then we set them as finished at the start.

	for (int i = 0; i < m_MAP_ROWS; i++) {
		if ((*rowFilled)[i] == m_MAP_ROWS)
			SudokuSolver::setRowFinished(i, true);
	}

	for (int i = 0; i < m_MAP_COLS; i++) {
		if ((*colFilled)[i] == m_MAP_ROWS)
			SudokuSolver::setColFinished(i, true);
	}

	SudokuSolver::solveSudoku(startPoint, workingThread);
}


bool SudokuSolver::solveSudoku(int FIRST_DIM_EQ, AlgorithmThread* workingThread)
{
	if (FIRST_DIM_EQ >= m_MAP_ROWS * m_MAP_COLS - 1)
		return true;

	const int ROW = FIRST_DIM_EQ / m_MAP_COLS;
	const int COL = FIRST_DIM_EQ % m_MAP_COLS;

	for (int i = 1; i <= m_MAP_ROWS; i++) {

		if (SudokuSolver::valueFit(ROW, COL, i)) {

			(*sudokuMap)[ROW][COL] = i;

			(*rowFilled)[ROW] ++;
			(*colFilled)[COL] ++;

			int nextCell = FIRST_DIM_EQ + 1;

			while ((*sudokuMap)[nextCell / m_MAP_COLS][nextCell % m_MAP_COLS] != -1 && nextCell < m_MAP_COLS * m_MAP_COLS - 1)
				nextCell++;


			if (!workingThread->TestDestroy()) {
				THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, std::to_string(i), def_col::IDLE_COLOUR);
				evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);

				wxMilliSleep(100);
			}
			else {

				workingThread->flagThreadBreak(true);
				return false;

			}

			if ((*rowFilled)[ROW] == m_MAP_ROWS)
				SudokuSolver::setRowFinished(ROW, true);

			if ((*colFilled)[COL] == m_MAP_COLS)
				SudokuSolver::setColFinished(COL, true);

			if (SudokuSolver::solveSudoku(nextCell, workingThread))
				return true;

			else {
				(*sudokuMap)[ROW][COL] = -1;
				(*rowFilled)[ROW] --;
				(*colFilled)[COL] --;

				THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, "", def_col::IDLE_COLOUR);
				evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);

				SudokuSolver::setRowFinished(ROW, false);
				SudokuSolver::setColFinished(COL, false);
				wxMilliSleep(100);
			}
		}

	}

	return false;
}


void SudokuSolver::generateValues(AlgorithmThread* workingThread) {

	srand(time(NULL));
	for (int i = 0; i < m_MAP_ROWS; i += m_DIAG_SQRT) {
		SudokuSolver::generateValidSquare(i, i);
	}

	//generating valid values in remaining cells

	SudokuSolver::generateRemainingValues(m_DIAG_SQRT);

	//removing no. of filled cells depending on the difficulty


	//difficulty describes number of unfilled cells
	const int difficulty = 50;

	SudokuSolver::unFillCells(difficulty);

	SudokuSolver::printSudoku(workingThread);
}

void SudokuSolver::printSudoku(AlgorithmThread* workingThread) {

	for (int i = 0; i < m_MAP_ROWS; i++)
	{
		for (int j = 0; j < m_MAP_COLS; j++)
		{
			const int FIRST_DIM_EQ = i * m_MAP_COLS + j;
			const int value = (*sudokuMap)[i][j];
			std::string updatedValue = value != -1 ? std::to_string(value) : "";

			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, updatedValue, def_col::IDLE_COLOUR);
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);

		}
	}
}

void SudokuSolver::generateValidSquare(const int& ROW, const int& COL) {

	int randomCellValue = 0;
	for (int i = 0; i < m_DIAG_SQRT; i++) {

		for (int j = 0; j < m_DIAG_SQRT; j++) {

			do {

				randomCellValue = rand() % m_MAP_ROWS + 1;

			} while (!SudokuSolver::valueAvailableInSquare(ROW, COL, randomCellValue));

			(*inDiagonalSquare)[i + ROW][j + COL] = true;

			(*rowFilled)[i + ROW] ++;
			(*colFilled)[j + COL] ++;

			(*sudokuMap)[i + ROW][j + COL] = randomCellValue;
		}
	}
}

//first dimension equivalent -> row * MAP_SIZE + col; ex. (2, 2) on 9x9 map is 2 * 9 + 2 = 20

bool SudokuSolver::generateRemainingValues(const int& FIRST_DIM_EQ) {

	if (FIRST_DIM_EQ >= m_MAP_ROWS * m_MAP_COLS - 1)
		return true;

	//possible values are <1, m_MAP_SIZE>

	const int ROW = FIRST_DIM_EQ / m_MAP_COLS;
	const int COL = FIRST_DIM_EQ % m_MAP_COLS;

	for (int i = 1; i <= m_MAP_ROWS; i++) {

		if (SudokuSolver::valueFit(ROW, COL, i)) {
			(*sudokuMap)[ROW][COL] = i;

			(*rowFilled)[ROW] ++;
			(*colFilled)[COL] ++;

			int nextCell = FIRST_DIM_EQ + 1;

			while ((*inDiagonalSquare)[nextCell / m_MAP_COLS][nextCell % m_MAP_COLS] && nextCell < m_MAP_COLS * m_MAP_COLS - 1)
				nextCell++;

			if (SudokuSolver::generateRemainingValues(nextCell))
				return true;

			else {
				(*sudokuMap)[ROW][COL] = -1;
				(*rowFilled)[ROW] --;
				(*colFilled)[COL] --;
			}
		}

	}

	return false;
}

void SudokuSolver::unFillCells(const int& NUM) {

	int temp = NUM;
	int randomValue;
	while (temp != 0) {

		randomValue = rand() % (m_MAP_ROWS * m_MAP_COLS - 1);

		int i = randomValue / m_MAP_COLS;
		int j = randomValue % m_MAP_COLS;

		if ((*sudokuMap)[i][j] != -1) {
			(*rowFilled)[i] --;
			(*colFilled)[j] --;
			(*sudokuMap)[i][j] = -1;
			temp--;
		}
	}
}

bool SudokuSolver::valueAvailableInSquare(const int& ROW, const int& COL, const int& NUM)
{
	for (int i = 0; i < m_DIAG_SQRT; i++)
	{
		for (int j = 0; j < m_DIAG_SQRT; j++)
		{
			if ((*sudokuMap)[i + ROW][j + COL] == NUM)
				return false;
		}
	}
	return true;
}
bool SudokuSolver::valueAvailableInRow(const int& ROW, const int& NUM) {

	for (int i = 0; i < m_MAP_ROWS; i++) {

		if ((*sudokuMap)[ROW][i] == NUM)
			return false;
	}

	return true;
}
bool SudokuSolver::valueAvailableInCol(const int& COL, const int& NUM)
{
	for (int i = 0; i < m_MAP_ROWS; i++) {

		if ((*sudokuMap)[i][COL] == NUM)
			return false;
	}

	return true;
}
bool SudokuSolver::valueFit(const int& ROW, const int& COL, const int& NUM)
{
	return SudokuSolver::valueAvailableInRow(ROW, NUM) &&
		SudokuSolver::valueAvailableInCol(COL, NUM) &&
		SudokuSolver::valueAvailableInSquare(ROW - ROW % m_DIAG_SQRT, COL - COL % m_DIAG_SQRT, NUM);
}

void SudokuSolver::setRowFinished(const int& ROW, const bool& finished) {


	wxColour newRowColour;

	switch (finished) {

	case true:
		newRowColour = def_col::VISITED_COLOUR;
		break;

	case false:
		newRowColour = def_col::IDLE_COLOUR;
		break;
	}


	for (int i = 0; i < m_MAP_COLS; i++) {
		const int FIRST_DIM_EQ = ROW * m_MAP_COLS + i;
		if ((*colFilled)[i] != m_MAP_COLS || finished) {
			std::string updatedValue = (*sudokuMap)[ROW][i] != -1 ? std::to_string((*sudokuMap)[ROW][i]) : "";

			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, updatedValue, newRowColour);
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
		}
	}
}

void SudokuSolver::setColFinished(const int& COL, const bool& finished) {

	wxColour newColColour;

	switch (finished) {

	case true:
		newColColour = def_col::VISITED_COLOUR;
		break;

	case false:
		newColColour = def_col::IDLE_COLOUR;
		break;
	}

	for (int i = 0; i < m_MAP_ROWS; i++) {
		const int FIRST_DIM_EQ = i * m_MAP_COLS + COL;
		if ((*rowFilled)[i] != m_MAP_ROWS || finished) {

			std::string updatedValue = (*sudokuMap)[i][COL] != -1 ? std::to_string((*sudokuMap)[i][COL]) : "";
			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, updatedValue, newColColour);
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
		}
	}
}
