#include "SideThread.h"

SideThread::SideThread(wxEvtHandler* parent, std::function<void()> functionEvent) : wxThread(wxTHREAD_DETACHED), pParent(parent) {
	this->functionEvent = functionEvent;
}

void* SideThread::Entry() {
	functionEvent();

	wxCommandEvent evt(wxEVT_THREAD_END, evt_id::THREAD_END_ID);

	pParent->AddPendingEvent(evt);
	
	return 0;
}
