#include "DFSimpl.h"

using namespace std;
using namespace def_type;
DFSimpl::DFSimpl(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* handler) : parentEvtHandler(handler) {

	this->MAP_ROWS = MAP_ROWS;
	this->MAP_COLS = MAP_COLS;

	adjList = make_unique<vector2DInt>(MAP_ROWS * MAP_COLS, vector1DInt());
	visList = make_unique<vector1DBool>(MAP_ROWS * MAP_COLS, false);
	ancestor = make_unique<vector1DInt>(MAP_ROWS * MAP_COLS, -1);

}

void DFSimpl::setSource(const int src) {
	source = src;
}
int DFSimpl::getSource() {
	return source;
}

void DFSimpl::setBlockedCells(vector<bool>& blockedCells) {
	mapBlockedCells = blockedCells;
}

void DFSimpl::runDfsAlgorithm(AlgorithmThread* workingThread) {
	DFSimpl::applyAdjList();
	(*ancestor)[source] = source;
	dfs(source, workingThread);
}

void DFSimpl::dfs(const int& src, AlgorithmThread* workingThread) {

	(*visList)[src] = true;

	if (src != source && !workingThread->TestDestroy()) {

		THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(src, src, wxColour(204, 204, 0));
		evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, parentEvtHandler, *THREAD_DATA);
		wxMilliSleep(100);
	}

	else if (workingThread->TestDestroy()) {
		workingThread->flagThreadBreak(true);
		return;
	}

	for (int i = 0; i < (*adjList)[src].size(); i++) {
		int curr = (*adjList)[src][i];

		if (!(*visList)[curr]) {
			(*ancestor)[curr] = src;
			dfs(curr, workingThread);
		}
	}

}

void DFSimpl::showPathToSource(const int t_vertexFrom, AlgorithmThread* workingThread) {

	int temp = t_vertexFrom;

	while (temp != source) {

		if (!workingThread->TestDestroy()) {
			THREAD_DATA = std::make_unique<def_type::CELL_UPDATE_INFO>(temp, -1, wxColour(51, 255, 51));
			evt_thread::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, parentEvtHandler, *THREAD_DATA);
			wxMilliSleep(100);
		}

		else {
			workingThread->flagThreadBreak(true);
			return;
		}

		temp = (*ancestor)[temp];
	}

}

void DFSimpl::applyAdjList() {
	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			DFSimpl::addNeighbours(i, j);
		}

}

void DFSimpl::addNeighbours(int i, int j) {

	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++) {
			if (!(x == 0 && y == 0) && isSafe(i + x, j + y)) {
				int currentCellNum = i * MAP_COLS + j;
				int neighCellNum = (i + x) * MAP_COLS + (j + y);

				if (!mapBlockedCells[currentCellNum] && !mapBlockedCells[neighCellNum])
					(*adjList)[currentCellNum].push_back(neighCellNum);


			}
		}
}


bool DFSimpl::isSafe(int i, int j) {

	return ((i >= 0 && j >= 0) && (i < MAP_ROWS&& j < MAP_COLS));

}
DFSimpl::~DFSimpl() {

}