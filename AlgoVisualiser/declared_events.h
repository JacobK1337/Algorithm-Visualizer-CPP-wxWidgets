#pragma once
#include<wx/wx.h>
#include"defined_typenames.h"

DECLARE_EVENT_TYPE(wxEVT_MAP_UPDATE_REQUEST, -1)
DECLARE_EVENT_TYPE(wxEVT_MAP_UNCHECK_REQUEST, -1)

DECLARE_EVENT_TYPE(wxEVT_THREAD_END, -1)
DECLARE_EVENT_TYPE(wxEVT_THREAD_BREAK, -1)
namespace evt_id {
	const int THREAD_END_ID = 5555;
	const int THREAD_BREAK_ID = 5559;
	const int MAP_UPDATE_REQUEST_ID = 5556;
	const int MAP_UNCHECK_REQUEST_ID = 5558;
}

namespace evt_thread {

	template<typename T>
	void sendThreadData(const wxEventType& EVENT_TYPE, const int& EVENT_ID, wxEvtHandler* parentEvtHandler, const T& t_Data)
	{
		wxThreadEvent threadDataEvt(EVENT_TYPE, EVENT_ID);
		threadDataEvt.SetPayload(t_Data);
		wxQueueEvent(parentEvtHandler, threadDataEvt.Clone());

	}

}