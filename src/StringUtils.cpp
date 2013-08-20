/* StringUtils 
 *
 *	Some string helper functions
 *
 *	Scott Lawrence  yorgle@gmail.com
 *
 *	v1.0 2013-June-07 - Initial version
 */

/* This code is provided under an MIT license:

The MIT License (MIT)

Copyright (c) 2013 Scott Lawrence

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/



#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cerrno>


#include "StringUtils.h"


////////////////////////////////////////////////////////////////////////////////
StringUtils::StringUtils( void )
{
	// never used
}

StringUtils::~StringUtils( void )
{
	// never used
}

////////////////////////////////////////////////////////////////////////////////
std::string StringUtils::FileToString( std::string path )
{
    std::ifstream ifs( path.c_str() );
    if( !ifs ) return ""; /* file not found, unable to be opened */

    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );

    return(content);
}


void StringUtils::StringToFile( std::string content, std::string path )
{
	std::ofstream f;

	f.open( path.c_str() );
	f << content;
	f.close();
}

////////////////////////////////////////////////////////////////////////////////

bool StringUtils::SameStringCI( std::string str1, std::string str2 )
{
        std::string str1Cpy( str1 );
        std::string str2Cpy( str2 );
        std::transform( str1Cpy.begin(), str1Cpy.end(), str1Cpy.begin(), ::tolower );
        std::transform( str2Cpy.begin(), str2Cpy.end(), str2Cpy.begin(), ::tolower );
        return ( str1Cpy == str2Cpy );
}


////////////////////////////////////////////////////////////////////////////////

void StringUtils::StringSplit(std::string str, std::string delim, std::vector<std::string>& tokens)
{
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delim, 0);

    // Find first "non-delimiter".
    std::string::size_type pos = str.find_first_of(delim, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delim, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delim, lastPos);
    }
}

void StringUtils::GetAndSplitLine( std::vector<std::string>& tokens )
{
	std::string user;
	std::getline( std::cin, user );
	tokens.clear();
	StringUtils::StringSplit( user, " ", tokens );
}


void StringUtils::ArgvDump( std::vector<std::string>& tokens )
{
	for( int t=0 ; t<tokens.size() ; t++ )
	{
		std::cout << "   " << t << "|" << tokens[t] << "|" << std::endl;
	}
}
