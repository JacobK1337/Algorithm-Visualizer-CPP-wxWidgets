#include "SudokuSolver.h"

SudokuSolver::SudokuSolver(const int& MAP_SIZE, wxEvtHandler* handler) : m_MAP_SIZE(MAP_SIZE), parentEvtHandler(handler)
{
}

SudokuSolver::~SudokuSolver()
{
}

void SudokuSolver::runAlgorithm(AlgorithmThread* workingThread)
{
	sudokuMap = std::make_unique<def_type::vector2DInt>(m_MAP_SIZE, def_type::vector1DInt(m_MAP_SIZE, 0));
	inDiagonalSquare = std::make_unique<def_type::vector2DBool>(m_MAP_SIZE, def_type::vector1DBool(m_MAP_SIZE, false));
	m_DIAG_SQRT = sqrt(m_MAP_SIZE);


	SudokuSolver::solveSudoku(workingThread);
}

void SudokuSolver::solveSudoku(AlgorithmThread* workingThread)
{
	SudokuSolver::generateValidSudoku();

	SudokuSolver::printSudoku(workingThread);
}

void SudokuSolver::generateValidSudoku()
{
	//generating diagonal valid values in squares

	srand(time(NULL));
	for (int i = 0; i < m_MAP_SIZE; i += m_DIAG_SQRT) {
		SudokuSolver::generateValidSquare(i, i);
	}

	//generating valid values in remaining cells

	SudokuSolver::generateRemainingValues(m_DIAG_SQRT);

	
}


void SudokuSolver::printSudoku(AlgorithmThread* workingThread) {

	for (int i = 0; i < m_MAP_SIZE; i++)
	{
		for (int j = 0; j < m_MAP_SIZE; j++)
		{
			const int FIRST_DIM_EQ = i * m_MAP_SIZE + j;
			const int value = (*sudokuMap)[i][j];

			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, value, wxColour(255, 255, 255));
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, parentEvtHandler, *THREAD_DATA);

		}
	}
}

void SudokuSolver::generateValidSquare(const int& ROW, const int& COL) {

	int randomCellValue = 0;
	for (int i = 0; i < m_DIAG_SQRT; i++) {

		for (int j = 0; j < m_DIAG_SQRT; j++) {

			do {

				randomCellValue = rand() % m_MAP_SIZE + 1;

			} while (!SudokuSolver::valueAvailableInSquare(ROW, COL, randomCellValue));

			(*inDiagonalSquare)[i + ROW][j + COL] = true;
			(*sudokuMap)[i + ROW][j + COL] = randomCellValue;
		}
	}
}

//first dimension equivalent -> row * MAP_SIZE + col; ex. (2, 2) on 9x9 map is 2 * 9 + 2 = 20

bool SudokuSolver::generateRemainingValues(const int& FIRST_DIM_EQ) {

	if (FIRST_DIM_EQ >= m_MAP_SIZE * m_MAP_SIZE - 1)
		return true;

	//possible values are <1, m_MAP_SIZE>

	const int ROW = FIRST_DIM_EQ / m_MAP_SIZE;
	const int COL = FIRST_DIM_EQ % m_MAP_SIZE;

	for (int i = 1; i <= m_MAP_SIZE; i++) {

		if (SudokuSolver::valueFit(ROW, COL, i)) {
			(*sudokuMap)[ROW][COL] = i;

			int nextCell = FIRST_DIM_EQ + 1;
			
			while ((*inDiagonalSquare)[nextCell / m_MAP_SIZE][nextCell % m_MAP_SIZE] && nextCell < m_MAP_SIZE * m_MAP_SIZE - 1)
				nextCell++;

			if (SudokuSolver::generateRemainingValues(nextCell))
				return true;

			(*sudokuMap)[ROW][COL] = 0;
		}

	}

	return false;
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

	for (int i = 0; i < m_MAP_SIZE; i++) {

		if ((*sudokuMap)[ROW][i] == NUM )
			return false;
	}

	return true;
}
bool SudokuSolver::valueAvailableInCol(const int& COL, const int& NUM)
{
	for (int i = 0; i < m_MAP_SIZE; i++) {

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


