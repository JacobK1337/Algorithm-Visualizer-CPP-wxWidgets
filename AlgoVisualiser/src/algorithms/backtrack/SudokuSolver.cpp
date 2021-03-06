#include "../../../include/algorithms/backtrack/SudokuSolver.h"

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
	//finding first empty cell
	while ((*sudokuMap)[startPoint / m_MAP_ROWS][startPoint % m_MAP_COLS] != -1)
		startPoint++;


	//if randomly a whole row or column is correctly filled, then we set them as finished at the start.

	for (int i = 0; i < m_MAP_ROWS; i++) {
		if ((*rowFilled)[i] == m_MAP_ROWS)
			SudokuSolver::setRowFinished(i, true);
	}

	for (int i = 0; i < m_MAP_COLS; i++) {
		if ((*colFilled)[i] == m_MAP_COLS)
			SudokuSolver::setColFinished(i, true);
	}

	SudokuSolver::solveSudoku(startPoint, workingThread);
}


bool SudokuSolver::solveSudoku(int cellNum, AlgorithmThread* workingThread)
{
	if (cellNum >= m_MAP_ROWS * m_MAP_COLS - 1)
		return true;

	if (workingThread->TestDestroy()) {
		workingThread->flagThreadBreak(true);
		return false;
	}

	const int ROW = cellNum / m_MAP_COLS;
	const int COL = cellNum % m_MAP_COLS;

	for (int i = 1; i <= m_MAP_ROWS; i++) {

		if (SudokuSolver::valueFit(ROW, COL, i)) {

			(*sudokuMap)[ROW][COL] = i;

			(*rowFilled)[ROW] ++;
			(*colFilled)[COL] ++;

			int nextCell = cellNum + 1;

			while ((*sudokuMap)[nextCell / m_MAP_COLS][nextCell % m_MAP_COLS] != -1 && nextCell < m_MAP_COLS * m_MAP_COLS - 1)
				nextCell++;

			if (workingThread->TestDestroy()) {

				workingThread->flagThreadBreak(true);
				return false;

			}

			animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_YELLOW,
				cellNum,
				std::to_string(i),
				animation::DEFAULT_DELAY,
				m_parentEventHandler);

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

				animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_WHITE, cellNum, "", animation::DEFAULT_DELAY, m_parentEventHandler);

				//setting whole rows/cols colour to white only if this change made it unfilled.
				if ((*rowFilled)[ROW] == m_MAP_ROWS - 1)
					SudokuSolver::setRowFinished(ROW, false);

				if ((*colFilled)[COL] == m_MAP_COLS - 1)
					SudokuSolver::setColFinished(COL, false);
			}
		}

	}

	return false;
}


void SudokuSolver::generateValues(AlgorithmThread* workingThread) {

	srand(time(NULL));
	//filling squares in valid way
	for (int i = 0; i < m_MAP_ROWS; i += m_DIAG_SQRT) {
		SudokuSolver::generateValidSquare(i, i);
	}

	//generating valid values in remaining cells
	SudokuSolver::generateRemainingValues(m_DIAG_SQRT);

	//removing no. of filled cells depending on the difficulty
	//difficulty describes number of unfilled cells
	const int difficulty = 35;

	SudokuSolver::unFillCells(difficulty);

	SudokuSolver::printSudoku(workingThread);
}

void SudokuSolver::printSudoku(AlgorithmThread* workingThread) {

	for (int i = 0; i < m_MAP_ROWS; i++)
	{
		for (int j = 0; j < m_MAP_COLS; j++)
		{
			const int cellNum = i * m_MAP_COLS + j;
			const int value = (*sudokuMap)[i][j];
			std::string updatedValue = value != -1 ? std::to_string(value) : "";
			animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_ONSTART,
				cellNum,
				updatedValue,
				0,
				m_parentEventHandler);

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

bool SudokuSolver::generateRemainingValues(const int& cellNum) {

	if (cellNum >= m_MAP_ROWS * m_MAP_COLS - 1)
		return true;

	//possible values are <1, m_MAP_SIZE>

	const int ROW = cellNum / m_MAP_COLS;
	const int COL = cellNum % m_MAP_COLS;

	for (int i = 1; i <= m_MAP_ROWS; i++) {

		if (SudokuSolver::valueFit(ROW, COL, i)) {
			(*sudokuMap)[ROW][COL] = i;

			(*rowFilled)[ROW] ++;
			(*colFilled)[COL] ++;

			int nextCell = cellNum + 1;

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
	while (temp) {

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


	for (int i = 0; i < m_MAP_COLS; i++) {
		const int cellNum = ROW * m_MAP_COLS + i;
		if ((*colFilled)[i] != m_MAP_COLS || finished) {
			std::string updatedValue = (*sudokuMap)[ROW][i] != -1 ? std::to_string((*sudokuMap)[ROW][i]) : "";


			if (finished)
				animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_GREEN,
					cellNum,
					updatedValue,
					10,
					m_parentEventHandler);

			else
				animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_WHITE,
					cellNum,
					updatedValue,
					10,
					m_parentEventHandler);


		}
	}
}

void SudokuSolver::setColFinished(const int& COL, const bool& finished) {


	for (int i = 0; i < m_MAP_ROWS; i++) {
		const int cellNum = i * m_MAP_COLS + COL;
		if ((*rowFilled)[i] != m_MAP_ROWS || finished) {

			std::string updatedValue = (*sudokuMap)[i][COL] != -1 ? std::to_string((*sudokuMap)[i][COL]) : "";

			if (finished)
				animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_GREEN,
					cellNum,
					updatedValue,
					10,
					m_parentEventHandler);

			else
				animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_WHITE,
					cellNum,
					updatedValue,
					10,
					m_parentEventHandler);
		}
	}

}
