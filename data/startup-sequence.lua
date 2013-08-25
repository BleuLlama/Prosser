-- this is the startup script.

system = {
	debug = 1,
}

-- for the SendCommand function.  make sure these jive with ProsserShell.h
kPSC_Null = 0
kPSC_Warp = 1
kPSC_Save = 2
kPSC_Load = 3
kPSC_Include = 4
kPSC_Restart = 5
kPSC_Quit = 6

-- for the OnTyped function to return. 
kOT_Unused = 0
kOT_Used = 1
kOT_Veto = 2

-- this gets run when the system starts up.
-- game load should be in here.

function OnLoad()
	-- include the item core
	SendCommand( kPSC_Include, "itemcore" )

	-- and add the items
	SendCommand( kPSC_Include, "itemlist" )

	-- and warp to another warp
	SendCommand( kPSC_Warp, "center000" )
end

function OnUnload()
end
