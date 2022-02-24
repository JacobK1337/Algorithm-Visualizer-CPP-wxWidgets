#include "../../include/threads/AlgorithmThread.h"

AlgorithmThread::AlgorithmThread(wxEvtHandler* parent, std::function<void()> functionEvent) : wxThread(wxTHREAD_DETACHED), pParent(parent) {
	this->functionEvent = functionEvent;
}

void AlgorithmThread::flagThreadBreak(const bool& statement) {
	threadBreak = statement;
}

wxThread::ExitCode AlgorithmThread::Entry() {


	functionEvent();

	if (!threadBreak) {

		wxCommandEvent evt(wxEVT_THREAD_END, evt_id::THREAD_END_ID);

		pParent->AddPendingEvent(evt);

		return 0;
	}

	else {

		wxCommandEvent breakEvt(wxEVT_THREAD_BREAK, evt_id::THREAD_BREAK_ID);

		pParent->AddPendingEvent(breakEvt);

		return 0;
	}

}
