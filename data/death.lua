-- This is an example room, with some basic stuff in it
-- you should replace things with your own content

room = {
    short = "death",
    name = "You have died.",
    description = [[

You were unable to escape in time. 

To try again type "again"

To give up, type "surrender"

]]
}

exits = {
}

function OnLoad()
	items = {}
end

function OnUnload()
end

function OnMove( exitname, exitalias ) 
end

function OnPoll()
end

function RoomDescription() end

function OnTyped( c, p )
	if( c == "again" ) then
		SendCommand( kPSC_Restart, "" )
	end

	if( c == "surrender" ) then
		print( [[

Sorry it didn't work out for you.

]])
		SendCommand( kPSC_Quit, "" )
	end

	return kOT_Used
end

