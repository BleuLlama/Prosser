Prosser
=======

Prosser is a Lua-based "single user dungeon" text adventure engine,
created by Scott Lawrence in 2013.  yorgle@gmail.com


# Overview

Prosser is a Lua-based "single user dungeon" text adventure engine
in the style of old LPC Muds. The name is from the character of
"Prosser" in the Hitchhikers Guide To The Galaxy.  This engine was
originally started as the engine for Ludum Dare MiniLD 42, which
was to have a "HHGTTG" theme.  My goal was to make a recreation,
from memory, of the "HHGTTG" Infocom text adventure game.  The most
I did was make much of a lua-based text adventure engine... this
engine.


# License

Prosser is distributed with an MIT license. It is included with the
source files.


# User Operation

# Internal Operation

## Startup WAD Discovery

At startup time, the data WAD file is located for the interperter.
First thing it will do is check to see if there is a .zip file appended to 
the .executable itself.  If there is, it will use that for the data files.
If it does not find that, it looks for "wad.zip" in the current directory.

When a Lua file is opened/read by the engine, it will first look for it in
a directory named "live".  If it finds it there, it uses that version.  In
wizard mode, the edited files will be stashed in the "live" folder.


## A session - startup-sequence

Once the zip/content is loaded, the first thing it runs is
"startup-sequence.lua".  This will contain any system level, one-time
load stuff.  This will set up any enums/global settings, etc.

When a lua file is loaded, it calls the file's "OnLoad()" function.
When a lua file is unloaded (about to load another file) it calls the file's 
"OnUnload()" function.

Lua files can send commands to the engine using the SendCommand() function.  Currently there are four commands:

 - kPSC_Null		- do nothing
 - kPSC_Warp		- change rooms to someplace else immediately
 - kPSC_Save		- save the player data to the save file
 - kPSC_Load		- load the player data from the save file
 - kPSC_Include		- load and run the indicated LUA file

The standard operating procedure is to put the Warp to your first room
within the OnLoad() function of your startup-sequence.


## Room Loading

First thing that happens before a new room is loaded and run, is
that the "cleanRoom.lua" file is pre-loaded. This resets the
structures and functions back to a clean slate.  Anything that needs
to be called once, before *every* room is loaded can be put into
this file as well.

NOTE that the OnLoad() in "cleanRoom.lua" is NOT called.


When a new room is loaded, its "OnLoad()" function will be called once.

There are a few structures in a standard room file that contain
data that the interpreter uses to make the room come alive.

The fields of the room structure:
 - name - The name of the room, displayed in the heading
 - description - the textual description of the room, for when the player enters or types 'look'

When a user types "look" and a "RoomDescription()" function exists,
that function is called as well.

Anoter structure is the "exits" structure.  This one contains a list of 
items, containing:
 - name - the long display name (one word) eg "North"
 - alias - the short name (one letter, usually) eg "N"
 - lua - the name of the lua file to load when the user goes that direction eg "room003"
 - description - the text to display for the exit

When the user types "Go" or "move" with an option that matches
either a name or alias in the exits structure, the local "OnUnload()"
function is called, then the other lua room is loaded immediately
(with "cleanRoom" loaded just before, of course).

If something goes horribly wrong, then "crash.lua" is run.

When the user types something in, it is first passed to the "OnTyped()" function.
The OnTyped function has two parameters, consisting of the first two words
that the user typed.  For example if they typed:

    Move East Now

Then this will be called to the Lua script as though the function call were:

    OnTyped( "Move", "East" )

The OnTyped function can return one of three values.

    kOT_Unused	- the Lua side ignored this command
    kOT_Used    - the Lua side used this command
    kOT_Veto    - the Lua side vetoes its use in the interpreter.

The "Veto" option means that if the OnTyped function sees any "Move"
command, it can make sure that the main interpreter blocks use of
this function.  For example, here is a block of code that will
inhibit passage to an exit until the user tries to move there three
times:

    moves = 0
    function OnTyped( cmd, param )

	if( command == "move" and param == "North" )
	    moves = moves +1

	    if( moves >2 ) then
		return kOT_Unused
	    end

	    print "You push on the door, but it does not move."
	    return kOT_Veto

	return kOT_Unused
    end

## Wizard mode

You can enter "wizard mode" by using the "-wizard" option on the command
line. This enables a few things:
 - Editing
 - Autocreation of rooms
 - more detailed descriptions of filenames, etc

So for example, you type "edit" and it brings the current room into
vi.  From here, you can edit it.  When you save and quit, the room
is automatically reloaded (warped-to).

You can also type "edit <roomname>" to edit a specific other room.

To create a new room, you simply type "new <roomname>".  You can
also just add the room exit in your current room, save-quit, then
move to that new exit.  The "skeleton.lua" file will be copied to
the new "<roomname>.lua" file.  You can then edit it as above.

For items, the command is "newitem <itemname>".  Note that roomname
and itemnames are unique.  You can't have two rooms named "foo" or
a room and an item named "foo".

Note that the editor is not configurable currently, but can be
changed in the source.

Other Wizard options can be found by typing "help".  The Wizard
commands have a "(W)" in their description.

# Room reference guide

Note that all of these are 

Here is a list of functions in a room that are called by the interpreter:

 - OnLoad()	- once, when the file is loaded / player walks in
 - OnUnload()	- once, when the file is unloaded / player leaves
 - RoomDescription() - when the user types "look"
 - Poll()		- just before the command prompt is printed out
 - Typed() 	- the user typed this. See above for more info

Here is a list of expected structures in a room

 - room
  - name 	- name of the room, displayed in the header
  - description - paragraph about the room
 - exits
  - name	- big name for the exit name "North"
  - alias	- letter for the exit name "N"
  - lua		- file that this exit goes to  "room001"
  - description - the text to display for the exit
 - items
  - TBD

# Room example

Here is an example of a room.

    -- room example 1
    -- this is a lua comment.

    room = {
	    name = "Town Center",
	    description = [[This is the town center.
    Welcome!
    There is a path here.]]
    }

    exits = {
	    { name="North", alias="N", lua="north01",
	      description="The path leads to the North." },
	    { name="South", alias="S", lua="south01",
	      description="The path leads to the South." },
    }

    function OnLoad() end
    function OnUnload() end
    function Poll() end
    function RoomDescription() end
    function OnTyped() return kOTUnused end


Since Lua is dynamic, you can actively change things each time the
user interacts.  This means you can add or change exits or descriptions.
Consider the following "RoomDescription" code:

    views = 0
    function RoomDescription()
	    views = views +1

	    if( views >3 ) then
		    room["name"] = "Bedroom"
		    room["description"] = "You see a bedroom."
	    end
    end

Each time the user "look"s, it will increment the "views" variable.
Once it becomes greater than 3, then the description will change
to "Bedroom" as you see there.



# Building The Interpreter

There are some bits of code you will need to install to get this
to work:

 - BLZip
  - BLZip is my minizip-wrapping class thingy.  You can retrieve 
    it from https://github.com/BleuLlama/BLZip

 - zlib-1.2.8
  - The base source tree from zlib contains a few files that are used
    to build the zip library.  

 - Lua 5.2.2
  - BLua wrapper needs to link against this, and it uses some
    headers from it.

In the makfile you may need to adjust paths to BLZip and zlib-1.2.8
If you put them next to the "Prosser" folder than you should be fine.

Once you have this all set up, you should be able to just type
'make' in the contrib/ folder, and it will build libMinizip.a

This zip library is used to access the zip file which contains all
of the game data.

Next, go back to the main directory and type 'make' and it should
build the prosser executable.  If you type "make test", it will
build the executable, zip up the contents of the 'data' folder, and
then run it in 'wizard' mode.


I should note that you also need to have installed on your system:

 - zlib (1.2.8 tested)
 - lua	(5.2.2 tested)

The libraries from these are linked-to at compile time.

And obviously, you will need C++, and all of that falderal.

## details

Some details about building this stuff...

First of all, grab the zlib-1.2.8 source:

        curl -O http://zlib.net/zlib-1.2.8.tar.gz
	gzip -cd zlib-1.2.8.tar.gz | tar -xvf -
	cd zlib-1.2.8
	./configure
	make
	sudo make install

And Lua:

	curl -R -O http://www.lua.org/ftp/lua-5.2.2.tar.gz
	tar zxf lua-5.2.2.tar.gz
	cd lua-5.2.2
	make
	mkdir ~/sw
	edit the Makefile, and change:
		INSTALL_TOP= /home/pi/sw
	make install

On some systems, like raspberry pi, you may need to also install 'zip'.

	sudo apt-get install zip



# todo items

 - item support

 - use the exit names for navigation without "move"

 - get the explicit path for the executable for unzipping.
	- path completion/search fails.
	- not really easily feasible
