#include "DFSimpl.h"
using namespace std;

DFSimpl::DFSimpl(wxButton** buttons, int MAP_ROWS, int MAP_COLS) {
	this->MAP_ROWS = MAP_ROWS;
	this->MAP_COLS = MAP_COLS;

	mapButtons = buttons;
	adjList = new vector<vector<int>>(MAP_ROWS * MAP_COLS, vector < int >());
	visList = new vector<bool>(MAP_ROWS * MAP_COLS, false);
	ancestor = new vector<int>(MAP_ROWS * MAP_COLS, -1);
	
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
	if(src != source)
		mapButtons[src]->SetBackgroundColour(wxColour(204, 204, 0));

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
		mapButtons[temp]->SetBackgroundColour(wxColour(51, 255, 51));
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
	delete adjList;
	delete visList;
	delete ancestor;
}