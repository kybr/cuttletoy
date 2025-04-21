#!/usr/bin/env luajit

local uv = require("luv")
local socket = uv.new_udp()
if (socket == nil) then
    print("FAILED TO MAKE UDP SOCKET")
    return 1
end

local function receive(err, data, addr, flags)
    if (data) then
        print(data)
    end
end

local result = socket:bind("0.0.0.0", 7770)
if (result) then
    socket:set_membership("224.0.7.23", nil, "join")
    if (0 == socket:recv_start(receive)) then
        print("listening on 0.0.0.0:7770")
    else
        print("NOT LISTENING")
        return
    end
else
    print("FAIL TO BIND")
    return
end

uv.run()
