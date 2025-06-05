function table.clone(t)
	local result = {}
	for key, value in pairs(t) do
		if type(value) == "table" then
			result[key] = table.clone(value)
		else
			result[key] = value
		end
	end
	return result
end
