#pragma once
#include<wx/wx.h>
#include"thread_func.h"
#include"defined_typenames.h"
#include"declared_events.h"

namespace animation 
{
	typedef std::vector<wxColour> COLOR_TRANSITION;

	static const COLOR_TRANSITION DEFAULT_COLOR_TRANS_YELLOW = { wxColour(204, 204, 0), wxColour(255, 204, 153), wxColour(255, 178, 102) };
	static const COLOR_TRANSITION DEFAULT_COLOR_TRANS_WHITE = { wxColour(255, 255, 204), wxColour(224, 224, 224), wxColour(255, 255, 255) };
	static const COLOR_TRANSITION DEFAULT_COLOR_TRANS_ONSTART = { wxColour(255, 255, 255) };
	static const COLOR_TRANSITION DEFAULT_COLOR_TRANS_GREEN = { wxColour(204, 255, 204), wxColour(153, 255, 153), wxColour(0, 204, 0) };
	static const int DEFAULT_DELAY = 33;


	static void cellColorTransition(const COLOR_TRANSITION& colorTransition, const int& t_cellNum, const std::string& t_cellValue, const int& delay, wxEvtHandler* t_eventHandler) {
		def_type::CELL_UPDATE_INFO threadData;

		for (auto& color : colorTransition) {
			threadData = def_type::CELL_UPDATE_INFO(t_cellNum, t_cellValue, color);
			thread_func::sendThreadData(wxEVT_MAP_UPDATE_REQUEST, evt_id::MAP_UPDATE_REQUEST_ID, t_eventHandler, threadData);
			wxMilliSleep(delay);
		}
	}
}