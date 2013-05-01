/*
* Copyright (C) 2012-2013, <Aeihu.z, aeihu.z@gmail.com>.
*
* Game Scenario Maker is a free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* Version 2(GPLv2) as published by the Free Software Foundation.
*/

#ifndef _CTEXTSTYLE_H_ 
    #define _CTEXTSTYLE_H_

#include <SFML/Graphics.hpp>

class CTextStyle
{
	private:
	public:
		unsigned int 			_size;
		sf::Color					_color;
		sf::String::Style _style;

		CTextStyle();
};

#endif