#include "SideThread.h"

//wxDEFINE_EVENT(wxEVT_THREAD_END, wxCommandEvent);
DEFINE_EVENT_TYPE(wxEVT_THREAD_END)
SideThread::SideThread(wxEvtHandler* parent, function<void()> functionEvent) : wxThread(wxTHREAD_DETACHED), pParent(parent) {
	this->functionEvent = functionEvent;
}

void* SideThread::Entry() {
	functionEvent();

	wxCommandEvent evt(wxEVT_THREAD_END, END_THREAD_ID);

	pParent->AddPendingEvent(evt);
	
	return 0;
}
