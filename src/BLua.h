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

class BLua {
private:
	lua_State *hLua;

public:
	BLua( void );
	~BLua( void );

////////////////////////////////////////
// load and run
public:
	void LoadFile( std::string path );
	void LoadString( std::string content );

	void Run( void );
	void RunFile( std::string path ) {
		this->LoadFile( path );
		this->Run();
	}
	void RunString( std::string content ) {
		this->LoadString( content );
		this->Run();
	}


////////////////////////////////////////
// Stack
public:
	void PushBool( bool v );
	void PushLong( long v );
	void PushDouble( double v );
	void PushString( std::string v );

	void Pop( int count = 1 );

	bool StackBool( int stackdepth = -1 );
	long StackLong( int stackdepth = -1 );
	double StackDouble( int stackdepth = -1 );
	std::string StackString( int stackdepth = -1 );


////////////////////////////////////////
// function stuff
public:
	/* push calls for calling a function */

	/* set the function name */
	void FcnName( std::string fcn );
	/* push parameters onto the stack */
	//  then PUSH params
	/* make the function call */
	void FcnCall( int nParams=1, int nRets=0 );
	//  then pop results

public:
	// void fcn( void )
	void CallFcn( std::string fcn );

	// void fcn( stringA )
	void CallFcn( std::string fcn, std::string pA );

	// int fcn( stringA, stringB )
	int CallFcn( std::string fcn, std::string pA, std::string pB );
	void CallFcnNR( std::string fcn, std::string pA, std::string pB );

////////////////////////////////////////
// variable access
public:
	void SetBool( std::string name, bool v );
	void SetLong( std::string name, long v );
	void SetDouble( std::string name, double v );
	void SetString( std::string name, std::string v );

	bool GetBool( std::string name );
	long GetLong( std::string name );
	double GetDouble( std::string name );
	std::string GetString( std::string name );

////////////////////////////////////////
// table access 

// tablename = { foo="bar", baz="narf }
// GetTableString( "tablename", "foo" )  => "bar"
//
// otherwise, it's a 1-based index of something like:
// tablename = {
//    { one="uno", two="dos" },	// index 1
//    { one="une", two="deux" }, // index 2
// }
// GetTableString( "tablename", "one", 2 );  => "une"
//
public:
	bool GetTableBool( std::string table, std::string element, int aidx=-1 );
	long GetTableLong( std::string table, std::string element, int aidx=-1 );
	double GetTableDouble( std::string table, std::string element, int aidx=-1 );
	std::string GetTableString( std::string table, std::string element, int aidx=-1 );

private:
	bool GetTableItemPrep( std::string table, std::string element, int aidx );
	void GetTableItemBreakdown( int aidx );

public:
	std::string TableToString( std::string table );


////////////////////////////////////////
// adding c functionality to Lua
//typedef int (*lua_CFunction) (lua_State *L);
public:
	void RegisterFunction( std::string name, lua_CFunction theFcn );


////////////////////////////////////////
// Testing
public:
	void Test( void );
	void StackClear( void );
	void StackDump( std::string hdg = "Stack Dump" );
	void Dump( void );
	void DumpStruct( std::string stname );
};
