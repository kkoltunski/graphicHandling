#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Graphic.h"
#include "BMPpicture.h"
#include "PNGpicture.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

string changeToLowerCases(string _in) {
	for (char singleChar : _in)	singleChar = tolower(singleChar);
	return _in;
}
