#pragma once
#include<vector>
#include<wx/wx.h>

namespace def_type {
	typedef std::vector<std::pair<int, int>> vector1DPair;
	typedef std::vector<int> vector1DInt;
	typedef std::vector<bool> vector1DBool;
	typedef std::vector<vector1DPair> vector2DPair;
	typedef std::vector<vector1DInt> vector2DInt;
}