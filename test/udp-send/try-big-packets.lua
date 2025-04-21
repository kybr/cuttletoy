#!/usr/bin/env lua

local uv = require("luv")
local socket = uv.new_udp()
if (socket == nil) then
    print("FAILED TO MAKE UDP SOCKET")
    return 1
end

-- XXX setting the send buffer size does not work until you use the socket
-- by binding or ending
--print(socket:bind("127.0.0.1", 10001))

-- these fail
print(socket:send_buffer_size())
print(socket:send_buffer_size(65000))
print(socket:send_buffer_size())

local bytes = socket:try_send("message", "127.0.0.1", 9999)
if (bytes < 0) then
    print("FAIL")
    return 1
end

-- but, after the try_send these at least print numbers
print(socket:send_buffer_size())
print(socket:send_buffer_size(65000))
print(socket:send_buffer_size())

-- but setting the send buffer size seems to fail

local message = ''
for i = 1,65000,1 do
    message = message .. 'o'
end

if (0 > socket:try_send(message, "127.0.0.1", 9999)) then
    print("FAIL")
    return 1
end
