#pragma once
#include<wx/wx.h>

namespace thread_func {

	template<typename T>
	void sendThreadData(const wxEventType& EVENT_TYPE, const int& EVENT_ID, wxEvtHandler* parentEvtHandler, const T& t_Data)
	{
		wxThreadEvent threadDataEvt(EVENT_TYPE, EVENT_ID);
		threadDataEvt.SetPayload(t_Data);
		wxQueueEvent(parentEvtHandler, threadDataEvt.Clone());
	}
}