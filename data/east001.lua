-- east path

room = {
    short = "east001",
    name = "Easterly Path",
    description = [[This path winds off to the east and west]]
}

exits = {
    { name="West", alias="W", lua="center000", description = "The path goes to the west." },
    { name="East", alias="E", lua="east002", description = "The path goes to the east." }
}

function OnLoad() end
function OnUnload() end
function RoomDescription() end
function Poll() end
