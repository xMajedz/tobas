fs = {content = "", lines = {}}

function fs.clear_content()
    fs.content = ""
    fs.lines = {}
end

function fs.readfile(pathtofile)
    if (pathtofile) then
	    fs.content = readfile(pathtofile)
	    fs.content = fs.content:gsub("\r", "")
	    fs.lines = fs.content:split("\n")
        return fs.lines
    end
end

function fs.writefile(filename)
    fs.clear_content()
end

function fs.line(line)
    fs.content = fs.content .. line .. "\n"
end

return fs
