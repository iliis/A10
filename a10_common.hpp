#ifndef A10_COMMON_HPP_INCLUDED
#define A10_COMMON_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstring>

#include <boost/lexical_cast.hpp>

#include "tools/errorclass.hpp"
#include "tools/geometry.hpp"

typedef vector2<double> ScreenCoords;
typedef vector2<int>    MapCoords;

const double GRAVITY  = 2500;

#define valid_dim(x) valid_dim2(x)

#endif // A10_COMMON_HPP_INCLUDED
