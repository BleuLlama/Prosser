-- This is an example room, with some basic stuff in it
-- you should replace things with your own content

--------------------------------------------------------------------------------
room = {
    short = "itemtest",
    name = "Item test room.",
    description = [[
This room was created to experiment with item tests.
]]
}

exits = {
    { name="Center", alias="c", lua="center000",
	description = "Go back to the 'c'enter." }
}
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- Item class


local ItemClass = {}
ItemClass.__index = ItemClass

setmetatable( ItemClass, {
	__call = function (cls, ... )
		return cls.new(...)
	end,
})

function ItemClass:new( name, hp, location )
	local self = setmetatable( {}, ItemClass  )

	-- name of the item
	self.name = name

	-- hit points if eaten. >0 for good   <0 for hurts
	self.hp = hp

	-- location: (room).lua, 
	--		PLAYER		- player is carrying it
	--		DESTROYED 	- broken, gone
	self.location = location
	self.uses = -1

	return self
end

function ItemClass:Info()
	print( string.format("%10s : (%3d hp) (%2d use) @ %s",
		self:Name(), self.hp, self.uses, self.location ))
end

function ItemClass:Name()
	return( self.name )
end


function ItemClass:Drop( roomname )
	print( "You dropped the " .. self:Name() )
	self.location = roomname
end

function ItemClass:Get()
	print( "You picked up the " .. self:Name() )
	self.location = "PLAYER"
end
	
function ItemClass:Destroy()
	print( "The " .. self:Name() .. " is no longer." )
	self.location = "DESTROYED"
end


function ItemClass:Eat()
	print( "You cannot eat " .. self:Name() )
end


function ItemClass:Use()
	local ret = false

	if( self.uses > 0 ) then
		print( "You used the " .. self:Name() ) 
		self.uses = self.uses - 1
		ret = true

		if( self.uses <= 1 ) then
			print( "The " .. self:Name() .. " is unusable now" )
		end
	end

	print( "You cannot use " .. self:Name() .. " here." )

	return ret;
end

function ItemClass:Fix( fixes, hp )
	self.uses = self.uses + fixes
	self.hp = self.hp + hp
	print( "The " .. self:Name() .. " is repaired." )
end


--------------------------------------------------------------------------------
-- Item list support

function DeleteAllItems()
	if( itemlist == nil ) then
		itemlist = {}
		return
	end
	for k in pairs( itemlist ) do
		itemlist[k] = nil
	end
end


itemlist = {}

function AddItem( name, hp, location )
	if( itemlist[ name ] == nil ) then
		itemlist[name] = ItemClass:new( name, hp, location )
		desc = "It's just a " .. name
		itemlist[name].description = desc
	end
end



AddItem( "carrot", 10, "room0" )
AddItem( "slime", -5, "itemtest" )
AddItem( "cheese", 5, "itemtest" )
AddItem( "key", 0, "PLAYER" )
AddItem( "taco", 1, "chessroom" )

function ItemDump()
	print( "All items:" )
	for k in pairs( itemlist ) do
		itemlist[k]:Info()
	end
end


function ItemExamine( name, thisroom )
	if( itemlist[name] == nil ) then
		print( "There is no " .. name .. " here." )
		return
	end

	if( itemlist[name].location == "PLAYER"
	    or itemlist[name].location == thisroom ) then
		print( itemlist[name].description )
	end
end


function ItemGet( name, thisroom )
	if( itemlist[name] == nil ) then
		print( "There is no " .. name .. " here." )
		return
	end

	if( itemlist[name].location == "PLAYER" ) then
		print( "You already have it." )
		return
	end
	if( itemlist[name].location ~= thisroom ) then
		print( "There is no " .. name .. " here." )
		return
	end

	itemlist[name]:Get()
end

function ItemDrop( name, thisroom )
	if( itemlist[name] == nil ) then
		print( "You have no no " .. name "." )
		return
	end

	if( itemlist[name].location ~= "PLAYER" ) then
		print( "You don't have it." )
		return
	end

	itemlist[name]:Drop( thisroom )
end

function ItemUse( name, thisroom )
	if( itemlist[name] == nil ) then
		print( "You have no no " .. name "." )
		return
	end

	if( itemlist[name].location ~= "PLAYER" ) then
		print( "You don't have it." )
		return
	end

	itemlist[name]:Use( thisroom )
end

function ItemEat( name, thisroom )
	if( itemlist[name] == nil ) then
		print( "You have no no " .. name "." )
		return
	end

	if( itemlist[name].location ~= "PLAYER" ) then
		print( "You don't have it." )
		return
	end

	itemlist[name]:Eat( thisroom )
end

function ItemListing( location )
	if( location == "PLAYER" ) then
		print( "Your inventory:" )
	else
		print( "Items here:" )
	end

	local itemcount = 0
	for k in pairs( itemlist ) do
		if( itemlist[k].location == location ) then
			print( "   " .. k )
			itemcount = itemcount + 1
		end
	end

	if( itemcount == 0 ) then
		print( "   (nothing)")
	end
end

--------------------------------------------------------------------------------

function OnLoad()
    print( "skeleton.lua -- OnLoad\n" )
end

function OnUnload()
    print( "skeleton.lua -- OnUnload\n" )
end

function OnPoll()
    print( "skeleton.lua -- Poll\n" );
    SendCommand( kPSC_Warp, "center000" )
end

function RoomDescription()
	ItemListing( room["short"] )
end

--------------------------------------------------------------------------------
looks = 0

function OnTyped( c, p )

	if( c == "get" ) then
		ItemGet( p, room["short"] )
		return kOT_Used
	end

	if( c == "drop" ) then
		ItemDrop( p, room["short"] )
		return kOT_Used
	end

	if( c == "use" ) then
		ItemUse( p, room["short"] )
		return kOT_Used
	end
	if( c == "eat" ) then
		ItemEat( p, room["short"] )
		return kOT_Used
	end


	if( c == "exa" or c == "look" ) then
		if( p ~= "" ) then
			ItemExamine( p, room["short"] )
			return kOT_Used
		end
	end

	if( c == "i" or c == "inventory" ) then
		ItemListing( "PLAYER" )
		return kOT_Used
	end

	if( c == "items" ) then
		ItemDump()
		return kOT_Used
	end




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
end

