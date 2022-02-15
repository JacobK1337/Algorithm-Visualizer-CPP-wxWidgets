#include "DFSimpl.h"

using namespace std;
using namespace def_type;
DFSimpl::DFSimpl(int MAP_ROWS, int MAP_COLS, wxEvtHandler* handler) : parentEvtHandler(handler) {

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

void DFSimpl::runDfsAlgorithm() {
	DFSimpl::applyAdjList();
	(*ancestor)[source] = source;
	dfs(source);
}

void DFSimpl::dfs(int src) {

	(*visList)[src] = true;

	if (src != source) {
		wxCommandEvent mapUpdateEvt(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID);
		mapUpdateEvt.SetInt(src);
		wxPostEvent(parentEvtHandler, mapUpdateEvt);
	}

	wxMilliSleep(100);
	for (int i = 0; i < (*adjList)[src].size(); i++) {
		int curr = (*adjList)[src][i];

		if (!(*visList)[curr]) {
			(*ancestor)[curr] = src;
			dfs(curr);
		}
	}

}

void DFSimpl::showPathToSource(const int t_vertexFrom) {

	int temp = t_vertexFrom;

	while (temp != source) {

		wxCommandEvent mapUpdateEvt(wxEVT_MAP_RECON_REQUEST, evt_id::MAP_RECON_REQUEST_ID);
		mapUpdateEvt.SetInt(temp);
		wxPostEvent(parentEvtHandler, mapUpdateEvt);
		wxMilliSleep(100);

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
			if (!(x == 0 && y == 0) && isSafe(i + x, j + y, MAP_ROWS, MAP_COLS)) {
				int currentCellNum = i * MAP_COLS + j;
				int neighCellNum = (i + x) * MAP_COLS + (j + y);

				if(!mapBlockedCells[currentCellNum] && !mapBlockedCells[neighCellNum])
					(*adjList)[currentCellNum].push_back(neighCellNum);
				

			}
		}
}


bool DFSimpl::isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT) {

	return ((i >= 0 && j >= 0) && (i < ROW_LIMIT&& j < COL_LIMIT));

}
DFSimpl::~DFSimpl() {
	
}