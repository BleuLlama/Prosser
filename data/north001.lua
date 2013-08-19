-- this file gets called on crash!
-- do recovery/dump in here.

room = {
name = "Town Path",
description = [[This is the path through the town.]]
}

exits = {
    { name="North", alias="N", lua="north002",
	description = "There is a path to the north."  },
    { name="South", alias="S", lua="center000",
	description = "There is a path to the south, it seems to go to the town center."  },
}

function OnLoad() end
function OnUnload() end
function RoomDescription() end
function Poll() end
