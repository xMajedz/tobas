local File = {Content = "", Lines = {}}

function File.ClearContent()
    File.Content = ""
    File.Lines = {}
end

function File.readLines(filename)
    if (filename) then
        f = io.open(filename, "r")
        --File.Content = f:read("*all")
        local line = f:read()
        while line ~= nil do
            line = line:gsub("\n", "")
            line = line:gsub("\r", "")
            table.insert(File.Lines, line)
            line = f:read()
        end
        f:close()
        return File.Lines
    else
        print("Cannot read file.")
        f:close()
        return nil
    end
end

function File.write(filename)
    f = io.open(filename, "w")
    f:write(File.Content)
    f:close()

    File.ClearContent()
end

function File.line(line)
    File.Content = File.Content .. line .. "\n"
end

return File
