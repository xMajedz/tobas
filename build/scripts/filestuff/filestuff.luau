fs = {content = "", lines = {}}

function fs.clear_content()
    fs.content = ""
    fs.lines = {}
end

function fs.readfile(pathtofile)
    if (pathtofile) then
	    fs.content = RL.LoadFileText(pathtofile)
	    fs.content = fs.content:gsub("\r", "") 
        return fs.content:split("\n")
    end
end

function fs.writefile(filename)
    fs.clear_content()
end

function fs.line(line)
    fs.content = fs.content .. line .. "\n"
end

return fs
