-- this file gets called on crash!
-- do recovery/dump in here.

room = {
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
	description = "The path also goes to the east." }
}

function OnLoad() end
function OnUnload() end
function Poll() end

function RoomDescription()
	local T = os.time()
	print( os.date( "The timepiece here reads:  %A %B %d, %I:%M %p", T ))
end
