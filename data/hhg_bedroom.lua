-- bedroom 


room = {
    short = "hhg_bedroom",
    name = "Darkness",
    description = [[You cannot see anything]],
}

exits = {
}

newexits = {
    { name="West", alias="W", lua="east002", description = "There is a path to the west." },
    { name="Bedroom", alias="Portal", lua="hhg_bedroom", description = "There is a portal to a bedroom." }
}



function OnLoad() end
function OnUnload() end
function Poll() end


views = 0
function RoomDescription() 
	views = views +1

	if( views >3 ) then
		room["name"] = "Bedroom"
		room["description"] = "You see a bedroom."
	end
end
