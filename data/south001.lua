-- south path

room = {
    short = "south001",
    name = "Town Path",
    description = [[This is the path through the town.]]
}

exits = {
    { name="North", alias="N", lua="center000",
	description = "There is a path to the north, it seems to go to the town center."  },
    { name="South", alias="S", lua="south002",
	description = "There is a path to the south."  },
}

function OnLoad() end
function OnUnload() end
function RoomDescription() end
function Poll() end
