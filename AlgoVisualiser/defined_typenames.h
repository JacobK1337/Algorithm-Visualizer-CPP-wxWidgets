#pragma once
#include<vector>
#include<wx/wx.h>

namespace def_type {
	typedef std::vector<std::pair<int, int>> vector1DPair;
	typedef std::vector<int> vector1DInt;
	typedef std::vector<bool> vector1DBool;
	typedef std::vector<vector1DPair> vector2DPair;
	typedef std::vector<vector1DInt> vector2DInt;

    struct CELL_UPDATE_INFO {
        int FIRST_DIM_EQ;
        int newValue;
        wxColour newColour;

        CELL_UPDATE_INFO(const int t_FIRST_DIM_EQ, const int t_newValue, const wxColour t_newColour) {
            FIRST_DIM_EQ = t_FIRST_DIM_EQ;
            newValue = t_newValue;
            newColour = t_newColour;
        };

    };
}