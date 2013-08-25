-- this file gets called before each new room is loaded in

-- room metastructure
room = nil;
	-- available parameters:
	--  name	- name of the room, displayed in a heading
	--  description - room description

-- list of exits
exits = nil;
-- example:
--	exits = {
-- 		{ name="North", alias="N", lua="room002" },
-- 		{ name="South", alias="S", lua="room003" },
-- 	};

-- list of items
items = nil;

--print "cleanRoom.lua"

function OnLoad() end
function OnUnload() end
function OnPoll() end
function RoomDescription() end
function OnMove( exitname, exitalias ) end

function OnTyped( c, p )
    return kOT_Unused
end
