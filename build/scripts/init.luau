dofile("scripts/drawstuff/drawstuff.luau")
dofile("scripts/modstuff/modstuff.luau")

--ms.loadmodTBM("lawz_model_evo_02")
--local TBM = ms.parseTBM("lawz_model_evo_02")
--local TBM = ms.parseTBM("tma_ergo_v4")
local TBM = ms.parseTBM("shootboxing15")
TBM["engageheight"] = 0.25 
TBM["engagedistance"] = 1.00
for player_name, p in TBM["player"] do
	for joint_name, j in p["joint"] do
		j["strength"] = 10*10*10*j["strength"]
	end
end
ms.loadTBM(TBM)
--ms.loadmodTBM("tma_ergo_v4")
--ms.loadmodTBM("shootboxing15")
local gameframe = ds.new({
	posX = 800,
	posY = 20,
	fontSize = 32,
})

function gameframe.update_func()
	gameframe.text = get_gameframe()
end
