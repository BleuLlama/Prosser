/* ProsserShell 
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

#include <stdlib.h>

#include "StringUtils.h"
#include "Utils.h"
#include "ProsserShell.h"

#define kLiveDir   "live/"
#define kEditor    "vi"


////////////////////////////////////////////////////////////////////////////////

int luaSendCommand( lua_State *L )
{
	// SendCommand( 9, "foo" )
	//           -2  -1
        if (!lua_isstring( L, -1 )) {
                fprintf( stderr, "SendCommand param 1 incorrect type\n");
		lua_pop( L, 1 );
                return 0;
        }
	std::string param( lua_tostring( L, -1 ));
	lua_pop( L, 1 );

        if (!lua_isnumber( L, -1 )) {
                fprintf( stderr, "SendCommand param 0 incorrect type\n");
		lua_pop( L, 1 );
                return 0;
        }
        int key = lua_tointeger( L, -1 );
	lua_pop( L, 1 );

	ProsserShell::Shared()->SendCommand( key, param );
	return 0;
}


void ProsserShell::SendCommand( int key, std::string param )
{
	std::string ltxt;

	switch( key ) {
	case( kPSC_Warp ):
		if( param.length() > 0 ) this->Cmd_Warp( param );
		break;

	case( kPSC_Save ):
		this->Cmd_Save();
		break;

	case( kPSC_Load ):
		this->Cmd_Load();
		break;

	case( kPSC_Include ):
		ltxt.assign( this->ContentFromFileOrZip( param ));
		if( ltxt.size() == 0 ) {
			ltxt.assign(  this->ContentFromFileOrZip( param + ".lua" ));
		}
		lua->RunString( ltxt );
		break;

	case( kPSC_Restart ):
		this->Deinit();
		this->Init();
		break;

	case( kPSC_Quit ):
		this->forceExit = true;
		break;

	case( kPSC_Version ):
		std::cout << "Prosser SUD " << version << std::endl;
		break;

	case( kPSC_Null ):
	default:
		std::cout << "SendCommand DNF error" << std::endl;
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////

static ProsserShell * _shared = NULL;

ProsserShell::ProsserShell( std::string _argv0, bool isWizard )
	: version( kVersionString )
	, wizard( isWizard )
	, forceExit( false )
	, loaded( false )
	, age_command( 0 )
	, age_return( 0 )
	, lastLoaded( "" )
	, argv0( _argv0 )
{
	if( !_shared ) {
		_shared = this;
	}

	this->Init();

	this->PrepCommands();
} 

void ProsserShell::Deinit( void )
{
	delete this->datafile;
	this->datafile = NULL;

	//delete this->lua;
	//this->lua = NULL;
}

void ProsserShell::Init( void )
{
	// okay. so the zip file parser will scan for PK\003\004,
	// so we'll just try loading ourselves, in case the zip
	// file is appended to us.

	// I had some code in here to scan the file argv0 for the
	// PK\003\004 block, but it found three of them. Which
	// makes sense.  One in the zip library that it uses for
	// comparison, one in this code, that *we* use for comparison,
	// and the one on the end of the file.  I was going to
	// change the code to search from the end of the file but
	// then I noticed when I accidentally opened the .exe in
	// 'vi', that vi decided that it was AOK to load it, but
	// it complained about garbage on the beginning.  I figured
	// "why not" and just tried loading our .exe as-is into
	// the zip library, and lo-and-behold, it "just worked"(TM).
	// So that's what this code does.

	// It tries to load *us* as a zip file.  If it works,
	// great.  we're done.  If not, load 'wad.zip' instead.

	this->loaded = false;
	this->age_command = 0;
	this->age_return = 0;
	this->lastLoaded = "";

	this->datafile = new BLUnZip( this->argv0 );

	if( this->datafile->isValid() ) {
		if( wizard ) std::cout << "W: Using .exe as the source zip!" << std::endl;
	} else {
		delete this->datafile;

		this->datafile = new BLUnZip( "wad.zip" );
		if( this->datafile->isValid() ) {
			if( wizard ) std::cout << "W: Using wad.zip!" << std::endl;
		}
	}
	this->lua = new BLua();

	this->lua->SetLong( "age_command", this->age_command );
	this->lua->SetLong( "age_return", this->age_return );

	this->lua->RegisterFunction( "SendCommand", luaSendCommand );

	if( !this->LoadLua( "startup-sequence" ) ) {
		std::cout << "Unable to initialize system." << std::endl;
		return;
	}
	std::cout << std::endl;
}

ProsserShell::~ProsserShell( void )
{
	this->Deinit();
}

ProsserShell * ProsserShell::Shared( void )
{
	return _shared;
}


void ProsserShell::PrepCommands( void )
{
	// commandList[ "word the user types" ] = "internal command handler";
	// common commands
	//commandList[ "quit" ] = "quit";
	//helpList[ "quit" ] = "Quit.";
	commandList[ "?" ] = "help";
	commandList[ "help" ] = "help";
	helpList[ "help" ] = "Display this help screen.";
	commandList[ "exa" ] = "look";
	commandList[ "look" ] = "look";
	helpList[ "look" ] = "Look around the current room or at an item.";
	commandList[ "move" ] = "move";
	commandList[ "go" ] = "move";
	helpList[ "move" ] = "Move/Go through one of the room's exits.";

	commandList[ "get" ] = "get";
	commandList[ "drop" ] = "drop";
	commandList[ "use" ] = "use";
	//commandList[ "wear" ] = "use";
	//commandList[ "don" ] = "use";
	//commandList[ "eat" ] = "eat";
	commandList[ "me" ] = "me";
	commandList[ "i" ] = "inventory";
	commandList[ "inventory" ] = "inventory";
	helpList[ "get" ] = "Pick up an item";
	helpList[ "drop" ] = "Drop an item";
	helpList[ "use" ] = "Use an item";
	//helpList[ "eat" ] = "Eat an item";
	helpList[ "inventory" ] = "Get inventory of stuff you have";
	helpList[ "me" ] = "find out about yourself";

	commandList[ "wizard" ] = "wizard";
	helpList[ "wizard" ] = "Toggle Wizard mode.";

	commandList[ "save" ] = "save";
	helpList[ "save" ] = "Save your progress";

	commandList[ "load" ] = "load";
	helpList[ "load" ] = "Restore from the last savegame";

	// wizard commands
	if( this->wizard ) {
		commandList[ "vi" ] = "edit";
		commandList[ "edit" ] = "edit";
		helpList[ "edit" ] = "(W) Edit the current room.";
		commandList[ "lua" ] = "lua";
		helpList[ "lua" ] = "(W) Enter a LUA command.";
		commandList[ "reload" ] = "reload";
		helpList[ "reload" ] = "(W) Reload the current room.";
		commandList[ "room" ] = "room";
		helpList[ "room" ] = "(W) Display internals for the current room.";
		commandList[ "warp" ] = "warp";
		helpList[ "warp" ] = "(W) Warp to any room.";
		commandList[ "listzip" ] = "listzip";
		helpList[ "listzip" ] = "(W) Show contents of zip file.";
		commandList[ "listlive" ] = "listlive";
		helpList[ "listlive" ] = "(W) Show contents of live dir.";
		commandList[ "new" ] = "new";
		helpList[ "new" ] = "(W) Create a new room.";

		commandList[ "newitem" ] = "newitem";
		helpList[ "newitem" ] = "(W) Create a new item.";
	}
}

////////////////////////////////////////


std::string ProsserShell::ContentFromFileOrZip( std::string path )
{
	std::string newpath( kLiveDir );
	newpath.append( path );
	std::string content = StringUtils::FileToString( newpath );

	if( content.length() > 0 ) {
		// it was on the filesystem! use it
		return content;
	}

	// nope. pull it out of the zip instead.
	return datafile->ExtractToString( path );
}

////////////////////////////////////////
std::string ProsserShell::RoomToFilename( std::string roomname )
{
	std::string str( kLiveDir );
        str.append( roomname );
        str.append( ".lua" );
	return str;
}


bool ProsserShell::LoadLua( std::string path )
{
	std::string ll( path );

	path.append( ".lua" );
	//std::string ltxt( datafile->ExtractToString( path ));
	std::string ltxt( this->ContentFromFileOrZip( path ));

	if( ltxt.size() > 0 ) {
		// it was found. do something
		if( loaded ) {
			lua->CallFcn( "OnUnload" );
		}

		std::string cleanRoom( this->ContentFromFileOrZip( "cleanRoom.lua" ));
		if( cleanRoom.size() > 0 ) {
			lua->RunString( cleanRoom );
		}

		this->lastLoaded.assign( ll );
		lua->RunString( ltxt );
		loaded = true;
		lua->CallFcn( "OnLoad" );
	} else {
		std::cout << path << ": Not found." << std::endl;
		return false;
	}

	return true;
}

void ProsserShell::PrePrompt( void )
{
	lua->CallFcn( "OnPoll" );
}


std::string ProsserShell::GetPrompt( void )
{
	std::ostringstream ss;

	if( this->forceExit == true )
	{
		ss << "Press [return] to exit.";
		return ss.str();
	}

	if( this->wizard ) {
		ss << " (W)  " << this->lastLoaded;
	}
	ss << "  [" << this->age_command << "] >";
	return ss.str();
}


////////////////////////////////////////////////////////////////////////////////

void ProsserShell::Cmd_Warp( std::string room )
{
	if( room.size() > 0 ) {
		std::string tlast = this->lastLoaded;
		bool success = this->LoadLua( room );
		if( !success && this->wizard ) {
			std::cout << "W: " << room
			<< ": Doesn't exist. Creating copy of " 
			<< tlast << "." << std::endl;
			this->Cmd_New( room, tlast );
			this->LoadLua( room );
		}
	}
	else this->LoadLua( this->lastLoaded );

	if( wizard ) {
		std::cout << "W: +++ Lua Room is " << this->lastLoaded << ".lua" << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;

	this->Cmd_Look();
}

void ProsserShell::Cmd_Help( void )
{
	// this is a mess.
	std::cout << "Commands:" << std::endl;

	std::map<std::string, std::string>::iterator iter;
	for (iter = helpList.begin(); iter != helpList.end(); ++iter) {
		std::string cmdname( iter->first );
		std::string helptext = iter->second;

		bool isWizcmd = false;
		if( helptext.size() > 3 ) {
			isWizcmd = helptext[0]=='(' && helptext[1]=='W' && helptext[2]==')';
		}

		if( !isWizcmd || ( isWizcmd && wizard ))
		{
			printf( "     %9s - %s\n", cmdname.c_str(), helptext.c_str() );
		}
	}
}


void ProsserShell::Cmd_ListZip( void )
{ 
	if( !this->wizard ) return;

	std::vector< std::string > lst;
	datafile->ListOfItems( lst );
	if( lst.size() == 0 ) {
		std::cout << "W: No files in zip." << std::endl;
	} else {
		std::cout << "W: Zip Listing:" << std::endl;
		std::vector< std::string >::iterator it;
		for( it = lst.begin() ; it != lst.end() ; it++ )
		{
			std::cout << "    " << *it << std::endl;
		}
	}
}


void ProsserShell::Cmd_ListLive( void )
{ 
	if( !this->wizard ) return;

	std::string livedir( kLiveDir );

	if( !Utils::DirectoryExists( livedir )) {
		std::cout << "W: No 'live' directory." << std::endl;
		return;
	}

	std::vector< std::string > lst;
	Utils::DirectoryListing( livedir, lst );

	if( lst.size() == 0 ) {
		std::cout << "W: No files in directory." << std::endl;
	} else {
		std::cout << "W: Live Listing:" << std::endl;
		std::vector< std::string >::iterator it;
		for( it = lst.begin() ; it != lst.end() ; it++ )
		{
			std::cout << "    " << *it << std::endl;
		}
	}
}


bool ProsserShell::Cmd_Move( std::string exitname, bool quiet )
{
	std::string newLuaName = "";

	int i=1;
	std::string name = lua->GetTableString( "exits", "name", i );
	std::string alias = lua->GetTableString( "exits", "alias", i );
	std::string luafile = lua->GetTableString( "exits", "lua", i );

	std::string usedExitName( "" );
	std::string usedExitShort( "" );

	while( name.length() > 0 ) {
		if( StringUtils::SameStringCI( exitname, name )
		 || StringUtils::SameStringCI( exitname, alias ) ) {
			newLuaName.assign( luafile );
			usedExitName.assign( name );
			usedExitShort.assign( alias );
			break;
		}

		i++;
		name = lua->GetTableString( "exits", "name", i );
		alias = lua->GetTableString( "exits", "alias", i );
		luafile = lua->GetTableString( "exits", "lua", i );
	}

	if( newLuaName.length() == 0 ) {

		if( !quiet ) std::cout << exitname << ": Can't go that way!" << std::endl;
		return false;
	} else { 
		//std::cout << " new lua is " << newLuaName << std::endl;
	}

	lua->CallFcnNR( "OnMove", usedExitName, usedExitShort );

	Cmd_Warp( newLuaName );
	return true;
}

void ProsserShell::Cmd_Look( void )
{

	std::string roomName = lua->GetTableString( "room", "name" );
	if( roomName.length() > 0 ) {
		std::cout << "========================================" << std::endl;
		std::cout << "    " << roomName << std::endl;
		std::cout << "========================================" << std::endl;
	}

	std::string roomDesc = lua->GetTableString( "room", "description" );
	if( roomDesc.length() > 0 ) std::cout << roomDesc << std::endl;
	lua->CallFcn( "RoomDescription" );
	lua->CallFcn( "ItemListing", lastLoaded );

	// debug text

	std::cout << std::endl;
	int i=1;
	std::string name = lua->GetTableString( "exits", "name", i );

	while( name.length() > 0 ) {
		std::string exitdesc = lua->GetTableString( "exits", "description", i );
		if( exitdesc.length() > 0 ) std::cout << exitdesc << std::endl;

		i++;
		name = lua->GetTableString( "exits", "name", i );
	}
}

void ProsserShell::Cmd_Room( void )
{
	if( !this->wizard ) return;

	int i=1;
	std::cout << "W: Room file: " << this->lastLoaded  << std::endl;
	std::string name = lua->GetTableString( "exits", "name", i );
	while( name.length() > 0 ) {
		std::string alias = lua->GetTableString( "exits", "alias", i );
		std::string luafile = lua->GetTableString( "exits", "lua", i );

		std::cout << "    " << name << " (" << alias << ")  -> " << luafile << ".lua" << std::endl;

		i++;
		name = lua->GetTableString( "exits", "name", i );
	}

	i = 1;
	name = lua->GetTableString( "exits", "name", i );
}


void ProsserShell::Cmd_Save( void )
{
	std::ostringstream savetext;

	savetext << "-- ProsserShell Save File" << std::endl;
	savetext << std::endl;
	savetext << lua->TableToString( "room" ) << std::endl;
	savetext << std::endl;
	std::cout << "Save content:" << std::endl << savetext.str();
}

void ProsserShell::Cmd_Load( void )
{
	std::cout << "should load here" << std::endl;
}


void ProsserShell::Cmd_LuaRun( std::vector<std::string> argv )
{
	if( !this->wizard ) return;

	std::ostringstream s;
	for( size_t i=1 ; i<argv.size() ; i++ ) {
		if( i!= 1 ) s << " ";
		s << argv[i];
	}
	
	lua->RunString( s.str() );
	lua->StackDump();
	lua->StackClear(); // be sure.
	//lua->Dump();
}


void ProsserShell::Cmd_New( std::string roomname, std::string copyFrom )
{
	if( !this->wizard ) return;

	if( roomname.size() == 0 ) {
		std::cout << "W: Specify a new room name" << std::endl;
		return;
	}

	std::string toFile = this->RoomToFilename( roomname );
	std::cout << "W: Synthesizing new room file: " << toFile << std::endl;
	
	if( Utils::FileExists( toFile )) {
		std::cout << "W: Doing nothing. File exists already." << std::endl;
	} else {
		if( copyFrom.size() == 0 ) {
			copyFrom.assign( "skeleton" );
		}
		copyFrom.assign( this->RoomToFilename( copyFrom ));

		std::string ltxt( this->ContentFromFileOrZip( "skeleton.lua" ));

		Utils::MakeDir( kLiveDir );
		StringUtils::StringToFile( ltxt, toFile );
	}
}


void ProsserShell::Cmd_NewItem( std::string itemname )
{
	if( !this->wizard ) return;

	if( itemname.size() == 0 ) {
		std::cout << "W: Specify a new item name" << std::endl;
		return;
	}

	std::string toFile = this->RoomToFilename( itemname );
	std::cout << "W: Synthesizing new item file: " << toFile << std::endl;
	
	if( Utils::FileExists( toFile )) {
		std::cout << "W: Doing nothing. File exists already." << std::endl;
	} else {
		std::string ltxt( this->ContentFromFileOrZip( "itemskeleton.lua" ));
		StringUtils::StringToFile( ltxt, toFile );
	}
}

void ProsserShell::Cmd_Edit( std::string param )
{
	std::string filepath( kLiveDir );
	if( !this->wizard ) return;

	// let's build the commadn editor string.
	std::string cmd( kEditor );
	cmd.append( " " );

	// first, let's make sure the "live" directory exists.
	Utils::MakeDir( kLiveDir );

	// Okay... we can have a few options here.  

	// If param is "", then we load  the last loaded
	if( StringUtils::SameStringCI( param, "" )) {

		// some special stuff in here.
		// we're working with the currently-loaded room, so we will
		// make sure that the live/ version exists.
		// if it doesn't, we synthesize it appropriately.

		filepath = this->RoomToFilename( this->lastLoaded );

		if( !Utils::FileExists( filepath )) {
			// synthesize last loaded so we have something to edit
			std::string ltxt( this->ContentFromFileOrZip( this->lastLoaded ));

			if( ltxt.length() == 0 ){
				std::string ll( this->lastLoaded );
				ll.append( ".lua" );
				ltxt.assign( this->ContentFromFileOrZip( ll ));
			}

			StringUtils::StringToFile( ltxt, filepath );
		}
		
		cmd.append( this->RoomToFilename( this->lastLoaded ));
	} else {
		filepath.append( param );

		// (live/)param(.lua)
		if( !Utils::FileExists( filepath )) {
			std::size_t pos = filepath.find( ".lua" );
			if( pos == std::string::npos ) {
				// (live/)param.lua
				filepath.append( ".lua" );
			}
		}

		if( !Utils::FileExists( filepath )) {
			// it's not there. attempt to synthesize it.
			std::size_t pos = param.find( ".lua" );
			if( pos == std::string::npos ) {
				param += ".lua";
			}

			std::string txt( this->ContentFromFileOrZip( param ));
			if( txt.length() > 0 ) {
				StringUtils::StringToFile( txt, filepath );
			}
		}

		if( !Utils::FileExists( filepath )) {
			std::cout << "!!!!!!!!!!!!" << std::endl;
			std::cout << param << ": Doesn't exist." << std::endl;
			std::cout << "!!!!!!!!!!!!" << std::endl;
			std::cout << std::endl;
			return;
		}

		cmd.append( filepath );
	}

	system( cmd.c_str() );
}

void ProsserShell::Cmd_Wizard( void )
{
	this->wizard = !this->wizard;

	std::cout << "W: Wizard mode: " << (this->wizard?"Enabled":"Disabled") << "." << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
// item interfaces (uses a lot on the lua side)
void ProsserShell::Cmd_Get( std::string item )
{
	lua->CallFcnNR( "ItemGet", item, this->lastLoaded );
}

void ProsserShell::Cmd_Drop( std::string item )
{
	lua->CallFcnNR( "ItemDrop", item, this->lastLoaded );
}

void ProsserShell::Cmd_Use( std::string item )
{
	lua->CallFcnNR( "ItemUse", item, this->lastLoaded );
}

void ProsserShell::Cmd_Eat( std::string item )
{
	lua->CallFcnNR( "ItemEat", item, this->lastLoaded );
}

void ProsserShell::Cmd_Exa( std::string item )
{
	lua->CallFcnNR( "ItemExamine", item, this->lastLoaded );
}

void ProsserShell::Cmd_Inventory( void )
{
	lua->CallFcn( "ItemListing", "PLAYER" );
}

void ProsserShell::Cmd_Me( void )
{
	lua->CallFcn( "Me" );
}


////////////////////////////////////////////////////////////////////////////////

bool ProsserShell::HandleLine( std::vector<std::string> argv )
{
	// check for a forced exit from message.
	if( this->forceExit == true ) return false;

	this->age_return++;
	this->lua->SetLong( "age_return", this->age_return );

	// no content.. just continue
	if( argv.size() < 1 ) return true;
	
	std::string tc = "";

	std::string param = "";

	long ret = kOT_Unused;

	// replace argv0 with the proper name.
	if( commandList.find( argv[0] ) != commandList.end() ) {
		argv[0] = commandList.find( argv[0] )->second;
	}

	if( argv.size() == 1 ) {
		ret = lua->CallFcn( "OnTyped", argv[0], "" );
	} else {
		ret = lua->CallFcn( "OnTyped", argv[0], argv[1] );
	}

	// it was used, just bail out..
	if( ret == kOT_Used ) {
		this->age_command++;
		this->lua->SetLong( "age_command", this->age_command );

		return true;
	}

	// okay. now we'll check the list of exits to find if 
	// argv[0] matches any of them.  if so, we'll re-inject 
	// with a "move" command
	int i = 1;
	std::string name = lua->GetTableString( "exits", "name", i );
	std::string alias = lua->GetTableString( "exits", "alias", i );

	while( name.length() > 0 ) {
		if(   StringUtils::SameStringCI( argv[0], name )
		   || StringUtils::SameStringCI( argv[0], alias ))
		{
			this->age_command++;
			this->lua->SetLong( "age_command", this->age_command );

			this->Cmd_Move( argv[0] );
			return true;
		}

		i++;
		name = lua->GetTableString( "exits", "name", i );
		alias = lua->GetTableString( "exits", "alias", i );
	}

	// look for the command in the commandList
	if(   (commandList.find( argv[0] ) == commandList.end() )
	   || (ret == kOT_Veto ))
	{
#define WHAT() \
	std::cout << argv[0] << ": What?" << std::endl; \
	return true;

		WHAT();
	}

#define WHAT2() \
	if( argv.size() != 2 ) { \
		WHAT(); \
	}

	this->age_command++;
	this->lua->SetLong( "age_command", this->age_command );

	////////////////////////////////////////
	tc.assign( commandList[ argv[0] ] );
	if( argv.size() >= 2 ) param = argv[1];

	//if(  StringUtils::SameStringCI( tc, "quit" )) return false;

	if(  StringUtils::SameStringCI( tc, "warp" ) && this->wizard ) {
		if( argv.size() == 2 ) 
			this->Cmd_Warp( argv[1] );
		else 
			tc.assign( "listzip" );
	}


	if(  StringUtils::SameStringCI( tc, "help" )) this->Cmd_Help(); 
	
	if(  StringUtils::SameStringCI( tc, "listzip" ) && this->wizard ) this->Cmd_ListZip();
	if(  StringUtils::SameStringCI( tc, "listlive" ) && this->wizard ) this->Cmd_ListLive();
	

	if(  StringUtils::SameStringCI( tc, "move" )) {
		// move with no argument?
		WHAT2();
		this->Cmd_Move( argv[1] );
	}

	if(  StringUtils::SameStringCI( tc, "look" )) {
		if( argv.size() >= 2 ) {
			this->Cmd_Exa( argv[1] );
		} else {
			this->Cmd_Look();
		}
	}

	if(  StringUtils::SameStringCI( tc, "get" )) {
		WHAT2();
		this->Cmd_Get( param );
	}
	if(  StringUtils::SameStringCI( tc, "drop" )) {
		WHAT2();
		this->Cmd_Drop( param );
	}
	if(  StringUtils::SameStringCI( tc, "use" )) {
		WHAT2();
		this->Cmd_Use( param );
	}
	if(  StringUtils::SameStringCI( tc, "eat" )) {
		WHAT2();
		this->Cmd_Eat( param );
	}
	if(  StringUtils::SameStringCI( tc, "examine" )) {
		WHAT2();
		this->Cmd_Exa( param );
	}
	if(  StringUtils::SameStringCI( tc, "inventory" )) this->Cmd_Inventory();
	if(  StringUtils::SameStringCI( tc, "me" )) this->Cmd_Me();

	if(  StringUtils::SameStringCI( tc, "room" ) && this->wizard ) this->Cmd_Room();

	if(  StringUtils::SameStringCI( tc, "luarun" ) && this->wizard ) this->Cmd_LuaRun( argv );

	if(  StringUtils::SameStringCI( tc, "new" ) && this->wizard ) {
		if( argv.size() != 2 )  this->Cmd_New( "" );
		else                    this->Cmd_New( argv[1] );
	}

	if(  StringUtils::SameStringCI( tc, "newitem" ) && this->wizard ) {
		if( argv.size() != 2 ) {
			std::cout << "item name?" << std::endl;
		} else {
			this->Cmd_NewItem( argv[1] );
		}
	}

	if(  StringUtils::SameStringCI( tc, "edit" ) && this->wizard ) {
		if( argv.size() > 1 ) {
			this->Cmd_Edit( argv[1] );
		} else {
			this->Cmd_Edit();
		}
		tc.assign( "reload" );
	}

	if( StringUtils::SameStringCI( tc, "reload" ) && this->wizard ) this->Cmd_Warp( "" );

	if( StringUtils::SameStringCI( tc, "wizard" ) ) this->Cmd_Wizard();

	return true;
}
