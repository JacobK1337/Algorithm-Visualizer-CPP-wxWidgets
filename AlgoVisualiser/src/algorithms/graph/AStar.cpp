#include "../../../include/algorithms/graph/AStar.h"

AStar::AStar(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler) : GraphAlgorithm(MAP_ROWS, MAP_COLS, handler) {
}

AStar::~AStar() {

}

void AStar::generateValues(AlgorithmThread* workingThread)
{
	for (int i = 0; i < m_MAP_ROWS; i++)
	{
		for (int j = 0; j < m_MAP_COLS; j++)
		{
			const int cellNum = i * m_MAP_COLS + j;
			animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_ONSTART, cellNum, "", 0, m_parentEventHandler);
		}
	}
}

void AStar::setBlockedCells(def_type::vector1DBool blockedCells) {
	cellBlocked = blockedCells;
}

void AStar::setSource(const int& t_newSource)
{
	m_source = t_newSource;
}

void AStar::setDest(const int& t_newDest) {
	m_dest = t_newDest;
}

void AStar::runAlgorithm(AlgorithmThread* workingThread)
{
	AStar::aStarSearch(workingThread);
}

void AStar::aStarSearch(AlgorithmThread* workingThread) {
	typedef std::tuple<double, int> cellInfoTuple;
	std::priority_queue<cellInfoTuple, std::vector<cellInfoTuple>, std::greater<> > calculatedDistances;
	std::vector<bool> cellProccessed(m_MAP_ROWS * m_MAP_COLS, false);
	std::vector<cellInfo> finalPath(m_MAP_ROWS * m_MAP_COLS, {-1, DBL_MAX, DBL_MAX, DBL_MAX});

	//parent of a source is a source; all distances are 0.
	finalPath[m_source] = { m_source, 0.0, 0.0, 0.0 };

	calculatedDistances.push(std::make_tuple(0.0, m_source));


	while (!calculatedDistances.empty()) {

		const cellInfoTuple& currentCellInfo = calculatedDistances.top();
		double cellDistance = std::get<0>(currentCellInfo);
		int cellNum = std::get<1>(currentCellInfo);
		cellProccessed[m_source] = true;

		//only two decimal places in the result;
		std::stringstream stream;
		stream << std::fixed << std::setprecision(2) << cellDistance;
		std::string truncCellDistance = stream.str();
		
		animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_YELLOW, 
			cellNum, 
			truncCellDistance, 
			animation::DEFAULT_DELAY, 
			m_parentEventHandler);

		calculatedDistances.pop();

		for(int x = -1; x <= 1; x++)
			for (int y = -1; y <= 1; y++) {
				int nextCellX = cellNum / m_MAP_COLS + x;
				int nextCellY = cellNum % m_MAP_COLS + y;

				if (AStar::isSafe(nextCellX, nextCellY) && !(x == 0 && y == 0)) {

					int nextCellNum = cellNum + (x * m_MAP_COLS) + y;
					if (nextCellNum == m_dest) {
						finalPath[nextCellNum].parent = cellNum;
						AStar::showPathToSource(finalPath, workingThread);
						return;
					}

					else if(!cellProccessed[nextCellNum]) {
						double newF, newD, newH;

						//checking if neighbour is adjacent diagonally; if so, then distance from cellNum is 1.4, if not - distance is 1.0.
						newD = ((x == -1 || x == 1) && y != 0) ? newD = finalPath[cellNum].d + 1.4 : newD = finalPath[cellNum].d + 1.0;

						newH = AStar::getH(nextCellNum);

						newF = newD + newH;

						if (finalPath[nextCellNum].f > newF) {
							calculatedDistances.push(std::make_tuple(newF, nextCellNum));

							finalPath[nextCellNum].d = newD;
							finalPath[nextCellNum].h = newH;
							finalPath[nextCellNum].f = newF;
							finalPath[nextCellNum].parent = cellNum;
						}
					}

				}

			}


	}
}

void AStar::showPathToSource(std::vector<cellInfo>& finalPath, AlgorithmThread* workingThread)
{
	int temp = m_dest;

	while (temp != m_source) {

		if (!workingThread->TestDestroy()) {

			animation::cellColorTransition(animation::DEFAULT_COLOR_TRANS_GREEN, 
				temp, 
				"", 
				animation::DEFAULT_DELAY, 
				m_parentEventHandler);

			temp = finalPath[temp].parent;
			wxMilliSleep(100);
		}

		else {

			workingThread->flagThreadBreak(true);
			return;

		}

	}
}

bool AStar::isSafe(const int& i, const int& j)
{
	return (((i >= 0 && j >= 0) && (i < m_MAP_ROWS&& j < m_MAP_COLS) && !cellBlocked[i * m_MAP_COLS + j]));
}

double AStar::getH(const int& v)
{
	int cellX = v / m_MAP_COLS;
	int cellY = v % m_MAP_COLS;

	int destX = m_dest / m_MAP_COLS;
	int destY = m_dest % m_MAP_COLS;

	return sqrt(pow(cellX - destX, 2) + pow(cellY - destY, 2));
}
