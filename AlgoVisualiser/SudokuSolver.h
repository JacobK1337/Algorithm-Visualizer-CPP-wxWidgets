#pragma once
#include<wx/wx.h>
#include"Algorithm.h"

class SudokuSolver : public Algorithm
{
public:

	SudokuSolver(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler);
	~SudokuSolver();
	virtual void runAlgorithm(AlgorithmThread* workingThread) override;
	virtual void generateValues(AlgorithmThread* workingThread) override;

private:

	unsigned int m_DIAG_SQRT;
	const def_type::vector1DInt difficulty = { 20, 30, 60 }; //difficulty represents amount unfilled cells

	std::unique_ptr<def_type::vector2DInt> sudokuMap;
	std::unique_ptr<def_type::vector2DBool> inDiagonalSquare;
	std::unique_ptr<def_type::vector1DInt> rowFilled;
	std::unique_ptr<def_type::vector1DInt> colFilled;

	//wxEvtHandler* parentEvtHandler = nullptr;
	std::unique_ptr<def_type::CELL_UPDATE_INFO> THREAD_DATA;

	bool solveSudoku(const int& FIRST_DIM_EQ, AlgorithmThread* workingThread);
	void generateValidSquare(const int& ROW, const int& COL);
	bool generateRemainingValues(const int& FIRST_DIM_EQ);
	void unFillCells(const int& NUM);
	bool valueAvailableInSquare(const int& ROW, const int& COL, const int& NUM);
	bool valueAvailableInRow(const int& ROW, const int& NUM);
	bool valueAvailableInCol(const int& COL, const int& NUM);
	bool valueFit(const int& ROW, const int& COL, const int& NUM);

	void setRowFinished(const int& ROW, const bool& finished);
	void setColFinished(const int& COL, const bool& finished);

	void printSudoku(AlgorithmThread* workingThread);

};

