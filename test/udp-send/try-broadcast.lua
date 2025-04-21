#!/usr/bin/env lua

local uv = require("luv")
local socket = uv.new_udp()
if (socket == nil) then
    print("FAILED TO MAKE UDP SOCKET")
    return 1
end

-- have to bind to 0.0.0.0
print(socket:bind("0.0.0.0", 10001))

print(socket:set_broadcast(true))

local result = socket:try_send("message", "255.255.255.255", 9999)
if (result == nil) then
    print("FAIL")
    return 1
end
