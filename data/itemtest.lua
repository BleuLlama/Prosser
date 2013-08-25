-- This is an example room, with some basic stuff in it
-- you should replace things with your own content

--------------------------------------------------------------------------------
room = {
    short = "itemtest",
    name = "Item test room.",
    description = [[
This room was created to experiment with item tests.

Type 'restart' in here to restart the system.
]]
}

exits = {
    { name="Center", alias="c", lua="center000",
	description = "Go back to the 'c'enter." }
}
--------------------------------------------------------------------------------

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

function RoomDescription()
end

--------------------------------------------------------------------------------
looks = 0

function OnTyped( c, p )
	if( c == "items" ) then
		ItemDump()
		return kOT_Used
	end

	if( c == "restart" ) then
        	SendCommand( kPSC_Restart, "" )
		return kOT_Used
	end

	if( c == "kill" and p == "self" ) then
		print( "YOU DIE NOW." )
        	SendCommand( kPSC_Quit, "" )
		return kOT_Used
	end

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

