#include "BFSimpl.h"
using namespace std;

BFSimpl::BFSimpl(wxButton** buttons, int MAP_ROWS, int MAP_COLS) {
	this->MAP_ROWS = MAP_ROWS;
	this->MAP_COLS = MAP_COLS;

	mapButtons = buttons;
	adjList = make_unique<vector2DInt>(MAP_ROWS * MAP_COLS, vector1DInt());
	visList = make_unique<vector1DBool>(MAP_ROWS * MAP_COLS, false);
	ancestor = make_unique<vector1DInt>(MAP_ROWS * MAP_COLS, -1);

}

void BFSimpl::setSource(const int src){
	source = src;
}
int BFSimpl::getSource() {
	return source;
}


void BFSimpl::setBlockedCells(vector<bool>& blockedCells) {
	mapBlockedCells = blockedCells;
}

void BFSimpl::runBfsAlgorithm() {
	BFSimpl::applyAdjList();
	bfs(source);
}

void BFSimpl::bfs(int src) {
	queue<int> q;

	(*visList)[src] = true;
	(*ancestor)[src] = src;
	q.push(src);

	while (!q.empty()) {
		int front = q.front();
		q.pop();
		for (int i = 0; i < (*adjList)[front].size(); i++) {
			int curr = (*adjList)[front][i];

			if (!(*visList)[curr]) {
				(*visList)[curr] = true;
				(*ancestor)[curr] = front;
				q.push(curr);
				mapButtons[curr]->SetBackgroundColour(wxColour(204, 204, 0));
				wxMilliSleep(100);
			}
		}
	}
}

void BFSimpl::applyAdjList() {
	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			BFSimpl::addNeighbours(i, j);
		}

}

void BFSimpl::showPathToSource(const int t_vertexFrom) {

	int temp = t_vertexFrom;

	while (temp != source) {
		mapButtons[temp]->SetBackgroundColour(wxColour(51, 255, 51));
		wxMilliSleep(100);

		temp = (*ancestor)[temp];
	}

}

void BFSimpl::addNeighbours(int i, int j) {

	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++) {
			if (!(x == 0 && y == 0) && isSafe(i + x, j + y, MAP_ROWS, MAP_COLS)) {
				int currentCellNum = i * MAP_COLS + j;
				int neighCellNum = (i + x) * MAP_COLS + (j + y);

				if (!mapBlockedCells[currentCellNum] && !mapBlockedCells[neighCellNum])
					(*adjList)[currentCellNum].push_back(neighCellNum);


			}
		}
}


bool BFSimpl::isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT) {

	return ((i >= 0 && j >= 0) && (i < ROW_LIMIT&& j < COL_LIMIT));

}

BFSimpl::~BFSimpl() {
	
}