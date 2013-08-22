-- north on the path

room = {
name = "Town Path",
description = [[This is the path through the town.
It seems to end here.]]
}

exits = {
    { name="South", alias="S", lua="north001",
	description = "There is a path to the south."  },
}

function OnLoad() end
function OnUnload() end
function RoomDescription() end
function Poll() end
