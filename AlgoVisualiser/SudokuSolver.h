#pragma once
#include<wx/wx.h>
#include"AlgorithmThread.h"
#include"defined_typenames.h"
#include"declared_events.h"

class SudokuSolver
{
public:
	SudokuSolver(const int& MAP_SIZE, wxEvtHandler* handler);
	~SudokuSolver();
	void runAlgorithm(AlgorithmThread* workingThread);
private:
	const unsigned int m_MAP_SIZE;
	unsigned int m_DIAG_SQRT;
	const def_type::vector1DInt difficulty = { 20, 30, 40 }; //difficulty represents amount unfilled cells

	std::unique_ptr<def_type::vector2DInt> sudokuMap;
	std::unique_ptr<def_type::vector2DBool> inDiagonalSquare;

	wxEvtHandler* parentEvtHandler = nullptr;
	std::unique_ptr<def_type::CELL_UPDATE_INFO> THREAD_DATA;

	void solveSudoku(AlgorithmThread* workingThread);
	void generateValidSudoku();
	void generateValidSquare(const int& ROW, const int& COL);
	bool generateRemainingValues(const int& FIRST_DIM_EQ);
	bool valueAvailableInSquare(const int& ROW, const int& COL, const int& NUM);
	bool valueAvailableInRow(const int& ROW, const int& NUM);
	bool valueAvailableInCol(const int& COL, const int& NUM);
	bool valueFit(const int& ROW, const int& COL, const int& NUM);

	void printSudoku(AlgorithmThread* workingThread);

};

