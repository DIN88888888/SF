#pragma once
#include "Const.h"
#include "Predef.h"

#include <stdexcept>

struct ex_arr_out_of_range :public std::exception
{
};