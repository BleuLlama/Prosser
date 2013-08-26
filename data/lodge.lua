-- Game start point

room = {
    short = "lodge",
    name = "The lodge.",
    description = 
[[You are standing in a room.
There is a black and white checkered floor.
There are blue curtains hanging around the perimeter of the room. 
You can hear some strange music playing.
]]
}

exits = {
    --{ name="Curtains", alias="c", lua="lodge",
--escription = "The curtains appear to be open." },

    { name="North", alias="N", lua="lodge",
	description = "The curtains appear to be open to the north." },
    { name="South", alias="S", lua="lodge",
	description = "The curtains appear to be open to the south." },
    { name="East", alias="E", lua="lodge",
	description = "The curtains appear to be open to the east." },
    { name="West", alias="W", lua="lodge",
	description = "The curtains appear to be open to the west." },
}

--------------------------------------------------------------------------------

AddItem( "device", 0, "lodge", "A funny device that might do something.", "useDevice" )

AddItem( "watch", 0, "lodge", "A wearable timepiece.  You might want to use it.", "useWatch" )

--------------------------------------------------------------------------------

ct = 0.0;

function updateCurrentTime()
	ct = 0.0
	if( DeviceUsed == true ) then
		ct = ticksWhenDeviceUsed + 
			(( age_command - ticksWhenDeviceUsed) * 0.1)
	else
		ct = age_command
	end
end

function useWatch()
	print( string.format( "The watch reads 12:59:%02.02f", 50+ct) )
end

function useDevice() 
	if( DeviceUsed == true ) then 
		return
	end
	DeviceUsed = true
	--AddItem( "key", 0, "lodge" )

	ticksWhenDeviceUsed = age_command

	print( [[
You hear a loud noise, and a bright flash of light
appears around you.  Suddenly, things look and feel different.
]] )
	room["description"] = [[
You are standing in a room.
There is a black and white checkered floor.
There are red curtains hanging around the perimeter of the room. 
You can hear some strange music playing.
]]

end

--------------------------------------------------------------------------------

function OnLoad()
	if( moveString == nil ) then
		moveString = ""
	end

	if( moveString == "NNSSWEWE" ) then
		room["description"] = [[
It is the same room, but something feels different.
As though there is some selection available to you now...
]]
		
	exits = {}
	exits = {
	   { name="Curtains", alias="c", lua="win",
		description = "The curtains appear to be open." },
	}
	end
end

function OnUnload()
end

function OnPoll()
	updateCurrentTime()
	if( ct > 10.0 ) then
		print( [[
Suddenly, everything has disappeared into a blinding violet light.

You have died.  :(

]] )
	SendCommand( kPSC_Warp, "death" )

	end
end

function RoomDescription() end

--------------------------------------------------------------------------------

function OnMove( exitname, exitalias ) 
	moveString = moveString .. exitalias

	if( string.len( moveString ) > 8 ) then
		moveString = string.sub( moveString, -8 )
	end
end 


--------------------------------------------------------------------------------
function OnTyped( c, p )
	if( c == "wait" ) then
		print( "You wait.  Nothing happens for a moment." )
		return kOT_Used
	end

	if( c == "listen" ) then
		if( p == "" or p == "music" ) then
			if( p == "" ) then
				print( "(to the music)" )
			end
			if( usedDevice == true ) then
			    print( "It sounds like VERY strange music." )
			else
			    print( "It sounds like strange music." )
			end
			return kOT_Used
		end

		print( "You can't hear that here." )

		return kOT_Used
	end

	--if( c == "use" and p == "device" ) then
	--	useDevice()
	--	return kOT_Used
	--end

	return kOT_Unused
end

looks = 0

function foo( c, p )
	if( c == "look" ) then
		looks = looks + 1
		return kOT_Unused
	end

        if( looks >3 ) then
                room["name"] = "Bedroom"
                room["description"] = "You see a bedroom."
        end

	if( looks > 10 ) then
		return kOT_Veto
	end

	if( looks > 15 ) then
		return kOT_Used
	end

	return kOT_Unused
end

