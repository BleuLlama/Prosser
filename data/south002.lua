-- south path

room = {
name = "Town Path",
description = [[This is the path through the town.
It seems to end here.]]
}

exits = {
    { name="North", alias="N", lua="south001",
	description = "There is a path to the north."  },
}

function OnLoad() end
function OnUnload() end
function RoomDescription() end
function Poll() end
