-- a simple library to help with ui stuff
DS_COLOR_RED = {0, 255, 0 ,255}
DS_COLOR_GREEN = {0, 255, 0 ,255}
DS_COLOR_BLUE = {0, 255, 0 ,255}
DS_COLOR_BLACK = {0, 0, 0 ,255}

ds = {objects = {}}
ds.__index = ds

function ds.new(o)
	o = o or {}
	o.shape = o.shape or "rectangle"
	o.posX = o.posX or 0
	o.posY = o.posY or 0
	o.width = o.width or 0
	o.height = o.height or 0
	o.color = o.color or DS_COLOR_GREEN
	o.text = o.text
	o.textColor = o.textColor or DS_COLOR_BLACK
	o.fontSize = o.fontSize or 1.00
	o.update_func = o.update_func
	setmetatable(o, ds)
	table.insert(ds.objects, o)
	return o
end

function ds:update()
	if self.update_func then
		self.update_func()
	end
end

function ds:draw()
	if self.shape == "rectangle" then
		DrawRectangle(
			self.posX,
			self.posY,
			self.width,
			self.height,
			self.color
		)
	end
	if self.text then
		DrawText(
			self.text,
			self.posX,
			self.posY,
			self.fontSize,
			self.textColor
		)
	end
end

function ds:drawstuff()
	for i, o in ds.objects do
		o:update()
		o:draw()
	end
end

function draw2d()
	ds.drawstuff()
end

return ds
