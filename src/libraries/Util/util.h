#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <CL/cl.hpp>

float rand_float(float mn, float mx);

std::string loadfromfile(std::string filename);

const char* oclErrorString(cl_int error);

#endif //UTIL_H