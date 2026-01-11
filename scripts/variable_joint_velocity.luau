local screenWidth, screenHeight = API.GetWindowSize()

local gear_count = 5
local current_gear_alt = gear_count
local current_gear = gear_count

RAYGUI["_VAR"]["GuiSlider"] = 1.00

function API.Update.JVJ(dt)
	if Game.IsFreeze() and Game.IsMode(Game["MODE_FREEPLAY"]) and Game.IsSelectedJointValid() then
		if not RAYLIB.IsKeyDown(RAYLIB["KEY_LEFT_CONTROL"]) and RAYLIB.IsKeyPressed(RAYLIB["KEY_Z"]) then
			Game.Refreeze()

			if RAYLIB.IsKeyDown(RAYLIB["KEY_LEFT_SHIFT"]) then
				local vel = Game.GetSelectedJointVelocityAlt() * RAYGUI["_VAR"]["GuiSlider"]
				Expermental.ToggleSelectedJointActiveStateAlt(vel)
			else
				local vel = Game.GetSelectedJointVelocity() * RAYGUI["_VAR"]["GuiSlider"]
				Expermental.ToggleSelectedJointActiveState(vel)
			end
		end

		if RAYLIB.IsKeyPressed(RAYLIB["KEY_N"]) then
			Game.Refreeze()

			if RAYLIB.IsKeyDown(RAYLIB["KEY_LEFT_SHIFT"]) then
				if current_gear_alt > 1  then
					current_gear_alt = current_gear_alt - 1
				end

				local ratio = current_gear_alt/gear_count

				RAYGUI["_VAR"]["GuiSlider"] = ratio

				Expermental.TriggerSelectedJointActiveStateAlt(Game.GetSelectedJointVelocityAlt() * ratio)
			else
				if current_gear > 1 then
					current_gear = current_gear - 1
				end

				local ratio = current_gear/gear_count

				RAYGUI["_VAR"]["GuiSlider"] = ratio

				Expermental.TriggerSelectedJointActiveState(Game.GetSelectedJointVelocity() * ratio)
			end

		end

		if RAYLIB.IsKeyPressed(RAYLIB["KEY_M"]) then
			Game.Refreeze()

			if RAYLIB.IsKeyDown(RAYLIB["KEY_LEFT_SHIFT"]) then
				if current_gear_alt < gear_count  then
					current_gear_alt = current_gear_alt + 1
				end

				local ratio = current_gear_alt/gear_count

				RAYGUI["_VAR"]["GuiSlider"] = ratio

				Expermental.TriggerSelectedJointActiveStateAlt(Game.GetSelectedJointVelocityAlt() * ratio)
			else
				if current_gear < gear_count then
					current_gear = current_gear + 1
				end

				local ratio = current_gear/gear_count

				RAYGUI["_VAR"]["GuiSlider"] = ratio

				Expermental.TriggerSelectedJointActiveState(Game.GetSelectedJointVelocity() * ratio)
			end
		end
	end
end

function API.Draw.VJV()
	if not Game.IsMode(Game["MODE_REPLAY"]) then
		if RAYGUI.GuiSlider((screenWidth-200)/2, screenHeight-80, screenWidth/4, 20, "Min", "Max", 0.00, 1.00) then
			Game.Refreeze()
		end
	end
end

