/* BLua 
 *
 *	C++ wrapper for Lua, to encapsulate a lot of the boilerplate
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

extern "C" {
#include "lua.h"

#include "lualib.h"
#include "lauxlib.h"
}

#include <iostream>
#include <sstream>
#include "BLua.h"

/*
class BLua {

public:
	BLua( void );
	~BLua( void );

public:
	void Test( void );
}
*/


BLua::BLua( void )
	: hLua( NULL )
{
	this->hLua = luaL_newstate();
	if( !this->hLua ) return;

	luaL_openlibs( this->hLua ); /* Load Lua libraries */
}

BLua::~BLua( void )
{
	lua_close( this->hLua );
}

////////////////////////////////////////////////////////////////////////////////


static int c_testgoober( lua_State *L ) 
{
	double d = luaL_checknumber( L, 1 );
	printf( "(this is in C) Number passed in is %f\n", d );
	lua_pushnumber( L, 13.13 );
	return 1; // 1 result
}

void BLua::Test( void )
{
	std::cout << "TEST" << std::endl;

	this->RunFile( "fs/example.lua" );
	this->RunString( "io.write( \"This is a test.\\n\" )");

	// retval = testfun( 12, 42 );
	this->FcnName( "testfun" );
	this->PushLong( 12 );
	this->PushLong( 42 );
	this->FcnCall( 2, 1 ); // 2 in, 1 out

	long retval = this->StackLong();
	this->Pop();

	std::cout << "return value is " << retval << std::endl;

	this->RunString( "aval = 10" );
	std::cout << "aval = " << this->GetLong( "aval" ) << std::endl;
	this->CallFcn( "simplefun" );
	this->SetLong( "aval", 20 );

	std::cout << "aval = " << this->GetLong( "aval" ) << std::endl;
	this->CallFcn( "simplefun" );
	this->Dump();

	this->RegisterFunction( "goober", c_testgoober );

	this->FcnName( "goober" );
	this->PushDouble( 42.42 );
	this->FcnCall( 1, 1 ); // 1 in, 1 out
	double ret = this->StackDouble();
	this->Pop();
	std::cout << "goober returned " << ret << std::endl;
}

void BLua::StackClear( void )
{
	int i=lua_gettop( this->hLua );
	while( i ) {
		this->Pop();
		i--;
	}
}

void BLua::StackDump( std::string hdg )
{
	std::cout << "---- " << hdg << std::endl;

	int i=lua_gettop( this->hLua );
	if( !i ) {
		printf( "   (stack is empty)\n" );
	}
	int cnt = i +1;

	while( i ) {
		int t = lua_type( this->hLua, i);
		switch (t) {
		case LUA_TSTRING:
		    printf("   %2d:S `%s'\n", i-cnt, lua_tostring( this->hLua, i));
		    break;
		case LUA_TBOOLEAN:
		    printf("   %2d:B %s\n",i-cnt,lua_toboolean( this->hLua, i) ? "true" : "false");
		    break;
		case LUA_TNUMBER:
		    printf("   %2d:N %g\n",  i-cnt, lua_tonumber( this->hLua, i));
		    break;
		default:
		    printf("   %2d:  %s\n", i-cnt, lua_typename( this->hLua, t)); break;
		}
		i--;
	}
}

void BLua::Dump( void )
{
	lua_State *L = this->hLua;
	
	// lua_next will:
	// 1 - pop the key
	// 2 - push the next key
	// 3 - push the value at that key
	// ... so the key will be at index -2 and the value at index -1

	lua_pushnil(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
	for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) {


	//while (lua_next( this->hLua, LUA_GLOBALSINDEX) != 0) {
		// get type of key and value
		int key_type = lua_type(L, -2);
		int value_type = lua_type(L, -1);

		// support only string keys
		// globals aren't likely to have a non-string key, but just to be certain ...
		if (key_type != LUA_TSTRING) {
			lua_pop(L, 1); // pop the value so that the top contains the key for the next iteration
			continue;
		}

		// support only number, boolean and string values
		if (value_type != LUA_TNUMBER &&
			value_type != LUA_TBOOLEAN &&
			value_type != LUA_TSTRING) {
			lua_pop(L, 1); // again, pop the value before going to the next loop iteration
			continue;
		}

		// get the key as a string
		std::string key_string = lua_tostring(L, -2); // no copy required - we already know this is a string

		// do not support variables that start with '_'
		// lua has some predefined values like _VERSION. They all start with underscore

		if (!key_string.size()) { // this again is highly unlikely, but still ...
			lua_pop(L, 1);
			continue;
		}
		if (key_string[0] == '_') {
			lua_pop(L, 1);
			continue;
		}

		std::string value_string;

		// convert the value to a string. This depends on its type
		switch (value_type) {
		case LUA_TSTRING:
		case LUA_TNUMBER:
			// numbers can be converted to strings

			// get the value as a string (this requires a copy because traversing tables
			// uses the top of the stack as an index. If conversion from a number to string
			// happens, the top of the stack will be altered and the table index will become invalid)
			lua_pushvalue(L, -1);
			value_string = lua_tostring(L, -1);
			lua_pop(L, 1);
			break;
		case LUA_TBOOLEAN:
			value_string = lua_toboolean(L, -1) == 0 ? "false" : "true";
			break;
		}

		// enclose the value in "" if it is a string
		if (value_type == LUA_TSTRING) {
			value_string = "\"" + value_string + "\"";
		}

		// resulting line. Somehow save this and when you need to restore it, just
		// call luaL_dostring with that line.
		std::string line(key_string + " = " + value_string);		// Pop the value so the index remains on top of the stack for the next iteration
		std::cout << "Line: " << line << std::endl;
	//	lua_pop(L, 1);
	}
}

////////////////////////////////////////////////////////////////////////////////
void BLua::LoadFile( std::string path )
{
	int status = luaL_loadfile( this->hLua, path.c_str() );

	if (status) {
		/* If something went wrong, error message is at the top of */
		/* the stack */
		fprintf(stderr, "Couldn't load file: %s\n", lua_tostring( hLua, -1));
	}
}

void BLua::LoadString( std::string content )
{
	if( content[ content.length()-1 ] != '\n' )
	{
		content.append( "\n" );
	}

	int status = luaL_loadstring( this->hLua, content.c_str() );

	if (status) {
		/* If something went wrong, error message is at the top of */
		/* the stack */
		fprintf(stderr, "Couldn't load string: %s\n", lua_tostring( hLua, -1));
	}
}

void BLua::Run( void )
{
	lua_pcall( this->hLua, 0, LUA_MULTRET, 0);
}


////////////////////////////////////////////////////////////////////////////////
// Stack stuff

void BLua::PushBool( bool v )
{
	lua_pushboolean( this->hLua, (v==true)?1:0 );
}

void BLua::PushLong( long v )
{
	lua_pushinteger( this->hLua, (ptrdiff_t) v );
}

void BLua::PushDouble( double v )
{
	lua_pushnumber( this->hLua, v );
}

void BLua::PushString( std::string v )
{
	lua_pushstring( this->hLua, v.c_str() );
}


void BLua::Pop( int count )
{
	lua_pop( hLua, count );  /* pop returned value */
}


long BLua::StackLong( int depth )
{
	if (!lua_isnumber( this->hLua, depth )) {
		fprintf( stderr, "function must return a number\n");
		return 0;
	}
	return lua_tointeger( this->hLua, depth );
}

bool BLua::StackBool( int depth )
{
	if (!lua_isboolean( this->hLua, depth )) {
		fprintf( stderr, "function must return a boolean\n");
		return false;
	}
	int b = lua_toboolean( this->hLua, depth ); 
	return (b==0)?false:true; // not necessary
}

double BLua::StackDouble( int depth )
{
	if (!lua_isnumber( this->hLua, depth )) {
		fprintf( stderr, "function must return a double\n");
		return 0.0;
	}
	return lua_tonumber( this->hLua, depth );
}

std::string BLua::StackString( int depth )
{
	std::string ret( "" );

	if (!lua_isstring( this->hLua, depth )) {
		fprintf( stderr, "function must return a string\n");
		return ret;
	}
	ret.assign( lua_tostring( this->hLua, depth ));

	return ret;
}


////////////////////////////////////////////////////////////////////////////////
// Function
void BLua::FcnName( std::string fcn )
{
	lua_getglobal( this->hLua, fcn.c_str() );
}

void BLua::FcnCall( int nParams, int nRets )
{
        int status = lua_pcall( this->hLua, nParams, nRets, 0 );

	if( status ) {
		fprintf( stderr, "error running function: %s\n", lua_tostring(hLua, -1));
	}
}


// void fcn( void )
void BLua::CallFcn( std::string fcn )
{
	this->FcnName( fcn );
	this->FcnCall( 0, 0 );
}

// void fcn( stringA )
void BLua::CallFcn( std::string fcn, std::string pA )
{
	this->FcnName( fcn );
	this->PushString( pA );
	this->FcnCall( 1, 0 );
}

// int fcn( stringA, stringB )
int BLua::CallFcn( std::string fcn, std::string pA, std::string pB )
{
	this->FcnName( fcn );
	this->PushString( pA );
	this->PushString( pB );
	this->FcnCall( 2, 1 );
	long retval = this->StackLong();
	this->Pop();
	return retval;
}

void BLua::CallFcnNR( std::string fcn, std::string pA, std::string pB )
{
	this->FcnName( fcn );
	this->PushString( pA );
	this->PushString( pB );
	this->FcnCall( 2, 0 );
}

////////////////////////////////////////////////////////////////////////////////
// Variables

void BLua::SetBool( std::string name, bool v )
{
	this->PushBool( v );
	lua_setglobal( this->hLua, name.c_str() );
}

void BLua::SetLong( std::string name, long v )
{
	this->PushLong( v );
	lua_setglobal( this->hLua, name.c_str() );
}

void BLua::SetDouble( std::string name, double v )
{
	this->PushDouble( v );
	lua_setglobal( this->hLua, name.c_str() );
}

void BLua::SetString( std::string name, std::string v )
{
	this->PushString( v );
	lua_setglobal( this->hLua, name.c_str() );
}

////////////////////////////////////////

bool BLua::GetBool( std::string name )
{
	lua_getglobal( this->hLua, name.c_str() );
	bool ret = this->StackBool();
	this->Pop();
	return ret;
}

long BLua::GetLong( std::string name )
{
	lua_getglobal( this->hLua, name.c_str() );
	long ret = this->StackLong();
	this->Pop();
	return ret;
}

double BLua::GetDouble( std::string name )
{
	lua_getglobal( this->hLua, name.c_str() );
	double ret = this->StackDouble();
	this->Pop();
	return ret;
}

std::string BLua::GetString( std::string name )
{
	lua_getglobal( this->hLua, name.c_str() );
	std::string ret = this->StackString();
	this->Pop();
	return ret;
}

////////////////////////////////////////////////////////////////////////////////
//typedef int (*lua_CFunction) (lua_State *L);
void BLua::RegisterFunction( std::string name, lua_CFunction theFcn )
{
    lua_pushcfunction( this->hLua, theFcn );
    lua_setglobal( this->hLua, name.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
// table access

////////////////////

bool BLua::GetTableItemPrep( std::string table, std::string element, int aidx )
{
	bool isokay = false;

	lua_getglobal( this->hLua, table.c_str() );
	if( !lua_istable( this->hLua, -1 )) {
		//std::cout << table << ": is not valid" << std::endl;
		this->Pop();
		return false;
	}

	if( aidx >= 0 ){
		// get the index into the arry
		lua_pushinteger( this->hLua, aidx );
		lua_gettable( this->hLua, -2 );

		if( !lua_istable( this->hLua, -1 )) {
			//std::cout << table << "[" << aidx << "]: is not valid" << std::endl;
			this->Pop(); // array nil
			this->Pop(); // table
			return false;
		}
	}

	lua_pushstring( this->hLua, element.c_str() );
	lua_gettable( this->hLua, -2 );

	return true;
}

void BLua::GetTableItemBreakdown( int aidx )
{
	this->Pop(); // remove the value

	if( aidx >= 0 ) {
		this->Pop(); // remove the arrayed table
	}

	this->Pop(); // remove the table or array
}

////////////////////

bool BLua::GetTableBool( std::string table, std::string element, int aidx )
{
	bool ret = false;

	if( !this->GetTableItemPrep( table, element, aidx ) ) {
		return ret;
	}
	// first half of getting the value is in the above item prep
	ret = this->StackBool( -1 );

	this->GetTableItemBreakdown( aidx );

	return ret;
}

long BLua::GetTableLong( std::string table, std::string element, int aidx )
{
	long ret = 0;

	if( !this->GetTableItemPrep( table, element, aidx ) ) {
		return ret;
	}
	// first half of getting the value is in the above item prep
	ret = this->StackLong( -1 );

	this->GetTableItemBreakdown( aidx );

	return ret;
}

double BLua::GetTableDouble( std::string table, std::string element, int aidx )
{
	double ret = 0.0;

	if( !this->GetTableItemPrep( table, element, aidx ) ) {
		return ret;
	}
	// first half of getting the value is in the above item prep
	ret = this->StackDouble( -1 );

	this->GetTableItemBreakdown( aidx );

	return ret;
}

std::string BLua::GetTableString( std::string table, std::string element, int aidx )
{
	std::string ret = "";

	if( !this->GetTableItemPrep( table, element, aidx ) ) {
		return ret;
	}
	// first half of getting the value is in the above item prep
	ret = this->StackString( -1 );

	this->GetTableItemBreakdown( aidx );

	return ret;
}


////////////////////////////////////////////////////////////////////////////////
std::string BLua::TableToString( std::string table )
{
	std::ostringstream ret;

	lua_getglobal( this->hLua, table.c_str() );
	if( !lua_istable( this->hLua, -1 )) {
		std::cout << table << ": is not a valid table" << std::endl;
		this->Pop();
		return ret.str();
	}
	this->StackDump();
	// -1 = table

	lua_pushnil( this->hLua );
	// -2 = table
	// -1 - nil

	ret << "--- Table dump of " << table << std::endl;
	ret << table << " = {" << std::endl; 

	while( lua_next( this->hLua, -2 ))
	{
		// -3 = table
		// -2 = key
		// -1 = value

		lua_pushvalue( this->hLua, -2 ); // tostring protection
		// -4 = table
		// -3 = key
		// -2 = value
		// -1 = key
		const char * key = lua_tostring( this->hLua, -1 );
		int value_type = lua_type( this->hLua, -2);
		std::string bv;

		switch( value_type ) {
		case( LUA_TSTRING ):
			ret << "    " << key << " = [[" << lua_tostring( this->hLua, -2 ) << "]]" << std::endl;
			break;

		case( LUA_TNUMBER ):
			ret << "    " << key << " = " << lua_tonumber( this->hLua, -2 ) << std::endl;
			break;

		case( LUA_TBOOLEAN ):
			if( lua_toboolean( this->hLua, -2) ) bv.assign( "true" );
			else                                 bv.assign( "false" );
			ret << "    " << key << " = " << bv << std::endl;
			break;

		default:
			ret << "    " << key << " = (" << lua_typename( this->hLua, value_type) << ") -- ?" << std::endl;
			break;
		}

		lua_pop( this->hLua, 2 );
		// -2 = table
		// -1 = key
	}


	ret << "}" << std::endl; 

	this->Pop();
	return ret.str();
}




////////////////////////////////////////////////////////////////////////////////

void BLua::DumpStruct( std::string stname )
{
	std::cout << "Structure: " << stname << std::endl;
	std::cout << std::endl;

	std::cout << "Direct table: " << std::endl;
	std::cout << "testarray[foo] = " << this->GetTableString( "testarray", "foo" ) << std::endl;
	std::cout << "testarray[bar] = " << this->GetTableString( "testarray", "bar" ) << std::endl;
	std::cout << "testarray[DNE] = " << this->GetTableString( "testarray", "DNE" ) << std::endl;

	std::cout << std::endl;

	std::cout << "indexed table:" << std::endl;
	std::cout << "exits[0][name] = " << this->GetTableString( "exits", "name", 0 ) << std::endl;
	std::cout << "exits[1][name] = " << this->GetTableString( "exits", "name", 1 ) << std::endl;
	std::cout << "exits[1][alias] = " << this->GetTableString( "exits", "alias", 1 ) << std::endl;
	std::cout << "exits[1][lua] = " << this->GetTableString( "exits", "lua", 1 ) << std::endl;
	std::cout << "exits[1][DNE] = " << this->GetTableString( "exits", "DNE", 1 ) << std::endl;
	std::cout << "exits[2][name] = " << this->GetTableString( "exits", "name", 2 ) << std::endl;
	std::cout << "exits[2][alias] = " << this->GetTableString( "exits", "alias", 2 ) << std::endl;
	std::cout << "exits[2][lua] = " << this->GetTableString( "exits", "lua", 2 ) << std::endl;
	std::cout << "exits[2][DNE] = " << this->GetTableString( "exits", "DNE", 2 ) << std::endl;
}


