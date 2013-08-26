-- This is an example room, with some basic stuff in it
-- you should replace things with your own content

room = {
    short = "win",
    name = "Stone circle in the woods",
    description = [[

You have escaped from the lodge! 

You are standing in the middle of a small circle of rocks 
in the middle of a lush forest of douglas fir trees.

You are craving a damn good coffee and some wonderful pie.
But that's another adventure...

]]
}

exits = {
}

function OnLoad()
	items = {}
	SendCommand( kPSC_Quit, "" )
end

function OnUnload()
end

function OnMove( exitname, exitalias ) 
end

function OnPoll()
end

function RoomDescription() end

function OnTyped( c, p )
    return kOT_Used
end
