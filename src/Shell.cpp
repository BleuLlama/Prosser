/* Shell 
 *
 *	command shell core
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

#include "StringUtils.h"
#include "Shell.h"

Shell::Shell( void )
	: loopAgain( true )
{
}

Shell::~Shell( void )
{
}

////////////////////////////////////////////////////////////////////////////////
bool Shell::HandleLine( std::vector<std::string> argv )
{
	StringUtils::ArgvDump( argv );

	// no content.. just continue
	if( argv.size() < 1 ) return true;

	// quick shortcut for 'quit'
	if( !argv[0].compare( "quit" )) return false;
	
	return true;
}

bool Shell::HandleString( std::string line )
{
	std::vector<std::string> tokens;
	StringUtils::StringSplit( line, " ", tokens );
	if( tokens.size() > 0 ) return this->HandleLine( tokens );
	return true;
}

void Shell::PrePrompt( void )
{
}

std::string Shell::GetPrompt( void )
{
	return "  #";
}

void Shell::Go( void )
{
	bool hadStuff = false;
	this->loopAgain = true;

	std::vector<std::string> tokens;

	do {
		this->PrePrompt();
		if( hadStuff ) std::cout << std::endl;
		hadStuff = false;
		std::cout << GetPrompt() << " " << std::flush;
		StringUtils::GetAndSplitLine( tokens );
		if( tokens.size() > 0 ) hadStuff = true;
		loopAgain = this->HandleLine( tokens );
	} while( loopAgain );
}
