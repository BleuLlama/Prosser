-- this file gets called on crash!
-- do recovery/dump in here.

OnLoad = function()
	print( "crash.lua -- onload\n" );
	print( "\n" );
	print( "Something went horribly wrong.\n" );
	print( "Sorry.\n" );
end
