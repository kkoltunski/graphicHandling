/*****************************************************************//**
 * \file   settings.h
 * \brief  Project main settings.
 * 
 * \author kkoltunski
 * \date   April 2020
***********************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H

using Tulli = unsigned long long int;

///Magic numbers up to 18,446,744,073,709,551,615 value.
enum bigMagicNumbers : Tulli { pngSignature = 727905341920923785 };
///Magic numbers up to 32,767 value.
enum smallMagicNumbers : short { pixelFormatBGR = 3, pixelFormatBGRA, bitsPerBytes = 8, bmpSignature = 19778};

#endif // !SETTINGS_H

