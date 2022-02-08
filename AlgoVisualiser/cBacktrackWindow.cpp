#include "cBacktrackWindow.h"

cBacktrackWindow::cBacktrackWindow() : wxFrame(nullptr, wxID_ANY, "", wxPoint(10, 10), wxSize(800, 600)) {
	this->SetBackgroundColour(wxColour(0, 0, 0));
	this->SetTitle("Backtracking algorithms");
}

cBacktrackWindow::~cBacktrackWindow() {

}