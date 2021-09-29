#pragma once

#include <iostream>
#include <format>
#include <algorithm>

// Platform unique
#ifdef WIN32
	#include <windows.h>
#endif

// Nuget
#include <fmt/ranges.h>
#include <fmt/color.h>

// Header-libs
#include "tabulate/table.hpp"

// Data parser
#include "DataParser/JsonDataParser.hpp"

// Etc...
#include "MathSolver.hpp"
