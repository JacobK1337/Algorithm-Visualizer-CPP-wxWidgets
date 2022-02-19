#include "BFSimpl.h"

using namespace std;
using namespace def_type;
BFSimpl::BFSimpl(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler) : GraphAlgorithm(MAP_ROWS, MAP_COLS, handler) {

	adjList = make_unique<vector2DInt>(MAP_ROWS * MAP_COLS, vector1DInt());
	visList = make_unique<vector1DBool>(MAP_ROWS * MAP_COLS, false);
	ancestor = make_unique<vector1DInt>(MAP_ROWS * MAP_COLS, -1);

}

BFSimpl::~BFSimpl() {

}

void BFSimpl::setSource(const int& src) {
	m_source = src;
}

int BFSimpl::getSource() {
	return m_source;
}

void BFSimpl::generateValues(AlgorithmThread* workingThread)
{
	for (int i = 0; i < m_MAP_ROWS; i++)
	{
		for (int j = 0; j < m_MAP_COLS; j++)
		{
			const int FIRST_DIM_EQ = i * m_MAP_COLS + j;
			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(FIRST_DIM_EQ, -1, wxColour(255, 255, 255));
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);

		}
	}
}

void BFSimpl::runAlgorithm(AlgorithmThread* workingThread)
{
	BFSimpl::applyAdjList();
	int when = 1;
	bfs(m_source, when, workingThread);
}


void BFSimpl::setBlockedCells(vector1DBool& blockedCells) {
	mapBlockedCells = blockedCells;
}


void BFSimpl::bfs(const int& src, int& when, AlgorithmThread* workingThread) {
	queue<int> q;

	(*visList)[src] = true;
	(*ancestor)[src] = src;
	q.push(src);

	while (!q.empty()) {
		int front = q.front();
		q.pop();

		//checking if thread was destroyed in parent
		if (!workingThread->TestDestroy()) {
			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(front, when++, wxColour(204, 204, 0));
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
			wxMilliSleep(100);
		}

		else {
			workingThread->flagThreadBreak(true);
			return;
		}

		for (int i = 0; i < (*adjList)[front].size(); i++) {
			int curr = (*adjList)[front][i];

			if (!(*visList)[curr]) {
				(*visList)[curr] = true;
				(*ancestor)[curr] = front;
				q.push(curr);
			}
		}
	}
}

void BFSimpl::applyAdjList() {
	for (int i = 0; i < m_MAP_ROWS; i++)
		for (int j = 0; j < m_MAP_COLS; j++) {
			BFSimpl::addNeighbours(i, j);
		}

}

void BFSimpl::showPathToSource(const int& t_vertexFrom, AlgorithmThread* workingThread) {

	int temp = t_vertexFrom;

	while (temp != m_source) {

		if (!workingThread->TestDestroy()) {
			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(temp, -1, wxColour(51, 255, 51));
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, m_parentEventHandler, *THREAD_DATA);
			wxMilliSleep(100);
		}

		else {
			workingThread->flagThreadBreak(true);
			return;
		}

		temp = (*ancestor)[temp];
	}

}


bool BFSimpl::isSafe(const int& i, const int& j) {

	return ((i >= 0 && j >= 0) && (i < m_MAP_ROWS && j < m_MAP_COLS));

}

void BFSimpl::addNeighbours(const int& i, const int& j)
{
	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++) {
			if (!(x == 0 && y == 0) && isSafe(i + x, j + y)) {
				int currentCellNum = i * m_MAP_COLS + j;
				int neighCellNum = (i + x) * m_MAP_COLS + (j + y);

				if (!mapBlockedCells[currentCellNum] && !mapBlockedCells[neighCellNum])
					(*adjList)[currentCellNum].push_back(neighCellNum);


			}
		}
}
