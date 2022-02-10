#include "DijkstraThread.h"

DEFINE_EVENT_TYPE(wxEVT_MYTHREAD)
DijkstraThread::DijkstraThread(wxEvtHandler* parent, function<void()> functionEvent) : wxThread(wxTHREAD_DETACHED), pParent(parent) {
	this->functionEvent = functionEvent;
}

void* DijkstraThread::Entry() {
	//wxCommandEvent event(wxEVT_MYTHREAD, 123);
	functionEvent();

	return 0;
}