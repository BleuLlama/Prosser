-- this file gets called on crash!
-- do recovery/dump in here.

room = {
name = "Skeleton room.",
description = [[This is a skeleton room.
Use it as a base for your rooms.]]
}

exits = {
    { name="North", alias="N", lua="room002",
	description = "There is a path to the north."  },
    { name="South", alias="S", lua="room003",
	description = "There is a path to the south."  },
}

function OnLoad()
    print( "skeleton.lua -- OnLoad\n" )
end

function OnUnload()
    print( "skeleton.lua -- OnUnload\n" )
end

function OnPoll()
   -- injectLine = "warp room001"
    print( "skeleton.lua -- Poll\n" );
end

function RoomDescription() end

function OnTyped( c, p )
    return kOT_Unused
end

