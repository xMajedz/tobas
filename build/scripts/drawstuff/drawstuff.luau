local ds = {
	objects = {},
	interactive_objects = {},
}
ds.__index = ds

local ds_internal = {}
ds["DS_COLOR_RED"] = {0, 255, 0 ,255}
ds["DS_COLOR_GREEN"] = {0, 255, 0 ,255}
ds["DS_COLOR_BLUE"] = {0, 0, 255 ,255}
ds["DS_COLOR_BLACK"] = {0, 0, 0 ,255}

function ds.new(o)
	o = o or {}
	o.shape = o.shape or "rectangle"
	o.posX = o.posX or 0
	o.posY = o.posY or 0
	o.width = o.width or 0
	o.height = o.height or 0
	o.color = o.color or ds.DS_COLOR_GREEN
	o.borderColor = o.borderColor
	o.text = o.text
	o.textColor = o.textColor or ds.DS_COLOR_BLACK
	o.fontSize = o.fontSize or 1.00
	o.update_callback = o.update_callback
	o.interactive = o.interactive
	o.click_callback = o.click_callback
	o.pressed = false
	o.released = true
	o.clicked = false
	o.mouse_down = false
	o.mouse_up = true
	o.children = o.children or {}
	setmetatable(o, ds)
	if o.interactive then
		table.insert(ds.interactive_objects, o)
	end
	if o.parent then
		table.insert(o.parent.children, o)
		return o
	end
	table.insert(ds.objects, o)
	return o
end

function ds:new_child(o)
	o.parent = o.parent or self
	return ds.new(o)
end

function ds:update()
	if self.update_callback then
		self:update_callback()
	end
end

function ds:draw()
	if self.shape == "rectangle" then
		RL.DrawRectangle(
			self.posX,
			self.posY,
			self.width,
			self.height,
			self.color
		)
	end
	if self.borderColor then
		RL.DrawRectangleLines(
			self.posX,
			self.posY,
			self.width,
			self.height,
			self.borderColor
		)
	end
	if self.text then
		RL.DrawText(
			self.text,
			self.posX,
			self.posY,
			self.fontSize,
			self.textColor
		)
	end
	for i, o in self.children do
		o:update()
		o:draw()
	end
end

function ds:drawstuff()
	for i, o in self.objects do
		o:update()
		o:draw()
	end
end

function EVENT.Draw2D.DS()
	ds:drawstuff()
end

function EVENT.MouseButtonPressed.DS()
	local MousePosition = RL.GetMousePosition()
	for i, o in ds.interactive_objects do
		local conditionX = MousePosition.x > o.posX and MousePosition.x < o.posX + o.width
		local conditionY = MousePosition.y > o.posY and MousePosition.y < o.posY + o.height
		if conditionX and conditionY then
			o:click_callback()
		end
	end
end

return ds

