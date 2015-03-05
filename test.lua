local xls = require("xlsread.core")
t = xls.read("data_maze.xls","UTF-8")

for k,v in pairs(t) do
    print("sheet:",k)
    for kk,vv in pairs(v) do
        for kkk,vvv in pairs(vv) do
            io.write(vvv,"(",type(vvv),")\t")
        end
        io.write("\n")
    end
    io.write("\n")
end

