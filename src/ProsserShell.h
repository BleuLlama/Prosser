/* main 
 *
 *	Prosser Shell
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
#include <sstream>
#include <map>

#include "Shell.h"
#include "BLUnZip.h"
#include "BLZip.h"
#include "BLua.h"

////////////////////////////////////////////////////////////////////////////////

class ProsserShell: public Shell
{
private:
	BLua * lua;
	BLUnZip * datafile;
	bool wizard;
	bool loaded;
	int age;
	std::string lastLoaded;

	std::map<std::string, std::string> commandList;
	std::map<std::string, std::string> helpList;


public:
	ProsserShell( std::string argv0, bool isWizard = false );
	~ProsserShell();

public:
	static ProsserShell * Shared( void );
	bool isLoaded( void ) { return this->loaded; }

public:
	#define kPSC_Null	(0)
	#define kPSC_Warp	(1)
	#define kPSC_Save	(2)
	#define kPSC_Load	(3)
	void SendCommand( int key, std::string value );

private:
	void PrepCommands( void );

public:
	void Cmd_Warp( std::string room );
	void Cmd_Help( void );
	void Cmd_ZipList( void );
	bool Cmd_Move( std::string exitname, bool quiet = false );
		// that returns TRUE if it found and did the move command
	void Cmd_Look( void );
	void Cmd_Room( void );
	void Cmd_LuaRun( std::vector<std::string> argv );
	void Cmd_New( std::string roomname, std::string copyFrom="" );
	void Cmd_Edit( void );
	void Cmd_Wizard( void );

	void Cmd_Save( void );
	void Cmd_Load( void );

public:
	bool HandleLine( std::vector<std::string> argv );
	void PrePrompt( void );
	std::string GetPrompt( void );

public:
	std::string ContentFromFileOrZip( std::string path );
	std::string RoomToFilename( std::string roomname );
	bool LoadLua( std::string path );
};

