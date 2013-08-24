-- This is an example room, with some basic stuff in it
-- you should replace things with your own content

room = {
    short = "skeleton",
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
    print( "skeleton.lua -- Poll\n" );
    SendCommand( kPSC_Warp, "center000" )
end

function RoomDescription() end

looks = 0

function OnTyped( c, p )
	if( c == "look" ) then
		looks = looks + 1
		return kOT_Unused
	end

        if( looks >3 ) then
                room["name"] = "Bedroom"
                room["description"] = "You see a bedroom."
        end

	if( looks > 10 ) then
		return kOT_Veto
	end

	if( looks > 15 ) then
		return kOT_Used
	end

	return kOT_Unused
end

