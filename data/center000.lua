-- this file gets called on crash!
-- do recovery/dump in here.

room = {
    short = "center000",
    name = "Town Center",
    description = [[This is the center of the town
Welcome!
There seems to be a path here.]],

    testbool = true,
    num1 = 32,
    num3 = 99.29
}

exits = {
    { name="North", alias="N", lua="north001",
	description = "There is a path to the north."  },
    { name="South", alias="S", lua="south001",
	description = "There is a path to the south."  },
    { name="East", alias="E", lua="east001",
	description = "The path also goes to the east." },

    { name="Itemtest", alias="I", lua="itemtest",
	description = "Experiment with items in the 'itemtest' room" }
}

function OnLoad() end
function OnUnload() end
function OnPoll() end

function RoomDescription()
	local T = os.time()
	print( os.date( "The timepiece here reads:  %A %B %d, %I:%M %p", T ))
end

eastCount = 0

function OnTyped( c, p )
	if( c == "smell" ) then
		print "It smells funky in here\n"
		return kOT_Used
	end



	if( ( c == "move" and p == "east" ) 
	  or ( c == "move" and p == "e" )) then

		eastCount = eastCount +1

		if( eastCount >= 3 ) then
			print( "You make your way to the east!" )
			return k_Unused
		end

		print( "The exit to the " .. p .. " seems blocked" )
		print( "Try again." )
		return kOT_Used
	end
	return kOT_Unused
end
