#!/usr/bin/env lua
if (#arg ~= 1) then
    print("must provide file name")
    return
end

local uv = require("luv")
local socket = uv.new_udp()
if (socket == nil) then
    print("FAILED TO MAKE UDP SOCKET")
    return 1
end

socket:bind("0.0.0.0", 7771)
socket:send_buffer_size(65000)
socket:set_broadcast(true)

local f = io.open(arg[1])
local message = f:read("*a")
f:close()

message = message .. '\0'

local i = string.len(message) % 4
if i == 3 then
  message = message .. "\0"
elseif i == 2 then
  message = message .. "\0"
  message = message .. "\0"
elseif i == 1 then
  message = message .. "\0"
  message = message .. "\0"
  message = message .. "\0"
end

message = "/f\0\0,s\0\0" .. message
-- XXX sending big packets to a broadcast address fails
-- investigating multicast....
--
local result = socket:try_send(message, "192.168.78.255", 7770)
--local result = socket:try_send(message, "192.168.78.255", 7770)
--local result = socket:try_send(message, "255.255.255.255", 7770)
if (result == nil) then
    print("FAIL")
    return 1
end
