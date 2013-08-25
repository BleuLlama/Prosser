-- Item class


local ItemClass = {}
ItemClass.__index = ItemClass

setmetatable( ItemClass, {
	__call = function (cls, ... )
		return cls.new(...)
	end,
})

function ItemClass:new( name, hp, location, desc, onuse )
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

	-- description string
	self.description = desc

	-- function to call for when it gets used
	self.onuse = onuse

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
	_G[self.onuse]()
end


	-- for consumables, the following is useful

function ItemClass:consumeableUse()
	
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

function AddItem( name, hp, location, desc, use )
	if( itemlist[ name ] == nil ) then
		itemlist[name] = ItemClass:new( name, hp, location, desc, use )
	end
end

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
	-- count the number of items
	local itemcount = 0
	for k in pairs( itemlist ) do
		if( itemlist[k].location == location ) then
			itemcount = itemcount + 1
		end
	end

	-- check for zero items
	if( itemcount == 0 ) then
		if( location == "PLAYER" ) then
			print( "You have nothing." )
			return
		end

		return
	end

	-- print the header
	if( location == "PLAYER" ) then
		print( "Your inventory:" )
	else
		print( "Items here:" )
	end

	-- and print the list
	for k in pairs( itemlist ) do
		if( itemlist[k].location == location ) then
			print( "   " .. k )
		end
	end

	if( itemcount == 0 ) then
		print( "   (nothing)")
	end
end
