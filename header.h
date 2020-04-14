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

template<typename T>
T bigToLittle(T _valToSwap){					//Endians handshake
	T result {0x0000};
	unsigned short ct = (sizeof(T));
	
	if(ct){									
		for(int i = 0; i < ct; ++i){
			result <<= 8;
			result += (_valToSwap & 0xFF);
			_valToSwap >>= 8;	
		}	
	}
	else result = _valToSwap;
	
	return result;
}

string changeToLowerCases(string _in) {
	for (char singleChar : _in)	singleChar = tolower(singleChar);
	return _in;
}
