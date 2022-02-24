#include "../../../include/algorithms/interface/Algorithm.h"

Algorithm::Algorithm(const int& MAP_ROWS, const int& MAP_COLS, wxEvtHandler* eventHandler) 
	: m_MAP_ROWS(MAP_ROWS), m_MAP_COLS(MAP_COLS), m_parentEventHandler(eventHandler) {

}

Algorithm::~Algorithm() {

}