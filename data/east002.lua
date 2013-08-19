-- this file gets called on crash!
-- do recovery/dump in here.

room = {
name = "The Hub",
    description = [[Welcome to The Hub.
From here, you can wander off into any number
of realms for exploration and adventure.
Please pick a direction to go, and have fun! ]],
}

exits = {
    { name="West", alias="W", lua="east002", description = "There is a path to the west." },
    { name="Bedroom", alias="Portal", lua="hhg_bedroom", description = "There is a portal to a bedroom." }
}



function OnLoad() end
function OnUnload() end
function RoomDescription() end
function Poll() end
