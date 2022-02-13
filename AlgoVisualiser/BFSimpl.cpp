#include "BFSimpl.h"
using namespace std;

BFSimpl::BFSimpl(wxButton** buttons, int MAP_ROWS, int MAP_COLS) {
	this->MAP_ROWS = MAP_ROWS;
	this->MAP_COLS = MAP_COLS;

	mapButtons = buttons;
	adjList = new vector<vector<int>>(MAP_ROWS * MAP_COLS, vector < int >());
	visList = new vector<bool>(MAP_ROWS * MAP_COLS, false);

}

void BFSimpl::setSource(const int src){
	source = src;
}
int BFSimpl::getSource() {
	return source;
}
void BFSimpl::runBfsAlgorithm() {
	BFSimpl::applyAdjList();
	bfs(source);
}

void BFSimpl::bfs(int src) {
	queue<int> q;

	(*visList)[src] = true;
	q.push(src);

	while (!q.empty()) {
		int front = q.front();
		q.pop();
		for (int i = 0; i < (*adjList)[front].size(); i++) {
			int curr = (*adjList)[front][i];

			if (!(*visList)[curr]) {
				(*visList)[curr] = true;
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

void BFSimpl::addNeighbours(int i, int j) {

	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++) {
			if (!(x == 0 && y == 0) && isSafe(i + x, j + y, MAP_ROWS, MAP_COLS)) {
				int currentCellNum = i * MAP_COLS + j;
				int neighCellNum = (i + x) * MAP_COLS + (j + y);
				(*adjList)[currentCellNum].push_back(neighCellNum);


			}
		}
}


bool BFSimpl::isSafe(int i, int j, const int ROW_LIMIT, const int COL_LIMIT) {

	return ((i >= 0 && j >= 0) && (i < ROW_LIMIT&& j < COL_LIMIT));

}

BFSimpl::~BFSimpl() {
	delete adjList;
	delete visList;
}