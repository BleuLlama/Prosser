-- this is the startup script.

system = {
	debug = 1,
}

-- for the SendCommand function.  make sure these jive with ProsserShell.h
kPSC_Null = 0
kPSC_Warp = 1
kPSC_Save = 2
kPSC_Load = 3

-- this gets run when the system starts up.
-- game load should be in here.

function OnLoad()
	SendCommand( kPSC_Warp, "center000" )
end

function OnUnload()
end