-- This is an example room, with some basic stuff in it
-- you should replace things with your own content

--------------------------------------------------------------------------------
room = {
    short = "itemtest",
    name = "Item test room.",
    description = [[
This room was created to experiment with item tests.
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

function oldOnTyped( c, p )

	if( c == "get" ) then
		ItemGet( p, room["short"] )
		return kOT_Used
	end

	if( c == "drop" ) then
		ItemDrop( p, room["short"] )
		return kOT_Used
	end

	if( c == "use" ) then
		ItemUse( p, room["short"] )
		return kOT_Used
	end
	if( c == "eat" ) then
		ItemEat( p, room["short"] )
		return kOT_Used
	end


	if( c == "exa" or c == "look" ) then
		if( p ~= "" ) then
			ItemExamine( p, room["short"] )
			return kOT_Used
		end
	end

	if( c == "i" or c == "inventory" ) then
		ItemListing( "PLAYER" )
		return kOT_Used
	end

	if( c == "items" ) then
		ItemDump()
		return kOT_Used
	end

end


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

