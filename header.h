/*****************************************************************//**
 * \file   header.h
 * \brief  Main project header.
 * 
 * \author kkoltunski
 * \date   April 2020
***********************************************************************/

#include "Graphic.h"
#include "BMPpicture.h"
#include "PNGpicture.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

/**
 * Converting letter size.
 * Function is converting each letter in string to lower cases.
 * \param [in] _inExpression Expression which has to be converted.
 * \return Expression converted to lower cases.
 */
string changeToLowerCases(string _inExpression) {
	for (char singleChar : _inExpression)	singleChar = tolower(singleChar);
	return _inExpression;
}
