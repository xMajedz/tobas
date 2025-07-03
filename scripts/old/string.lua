function string.explode(str, sstr)
    if str ~= "" and sstr ~= "" then
        local parts = {}
        local a = 1
        local b = 1
        while true do
            b = str:find(sstr, a)
            if b == nil then
                table.insert(parts, str:sub(a, str:len()))
                break
            else
                table.insert(parts, str:sub(a, b - sstr:len()))
            end
            a = b + sstr:len()
        end
        return parts
    end
    return nil
end
