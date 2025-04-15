--
-- NeoVim Lua plugin for Raspberry-Pi-Live-Shader
--
-- Karl Yerkes
-- 2022-05-15
--


local socket_send = nil

local shader = { address = "127.0.0.1", port = 7770 }

local _version = 0 -- XXX make this a dictionary


--------------------------------------------------------------------------------
-- Listen for buffer changes; Spurt contents via UDP ---------------------------
--------------------------------------------------------------------------------


local function send_code()


  -- get all the lines in the current buffer and make a single string
  --
  local lines = vim.api.nvim_buf_get_lines(0, 0, -1, true)

  local text = table.concat(lines, "\n")

  -- OSC strings are null terminated
  --
  text = text .. "\0"

  -- OSC strings are filled out with nulls until they are a multiple of 4 bytes
  --
  local i = string.len(text) % 4
  if i == 3 then
    text = text .. "\0"
  elseif i == 2 then
    text = text .. "\0"
    text = text .. "\0"
  elseif i == 1 then
    text = text .. "\0"
    text = text .. "\0"
    text = text .. "\0"
  end

  local data = "/frag\0\0\0,s\0\0" .. text


  -- check that the string is no more than 65507 bytes total
  --
  if (string.len(data) > 65507) then
    vim.api.nvim_command(string.format('echo "%s"', "FAIL: code too big"))
    return
  end


  -- make a new socket if necessary
  --
  if (socket_send == nil) then
    socket_send = vim.loop.new_udp()
  end


  -- send a datagram with the content
  --
  local bytes = vim.loop.udp_try_send(socket_send, data, shader.address, shader.port)
  if (bytes == nil) then
    -- depends on OS settings; fails at 9216 bytes for me
    vim.api.nvim_command(string.format('echo "%s"', "FAIL: udp send"))
  end
end



--------------------------------------------------------------------------------
-- Listen for status information on UDP; Show on the status line ---------------
--------------------------------------------------------------------------------


local function status_update(err, data, addr, flags)

  -- XXX: this callback is called twice for each packet (why?)
  -- and gets no arguments the second time it is called; return
  if (data == nil) then
    return
  end

  -- TODO: strip OSC metadata shaped like these 16 bytes:
  -- XX XX XX XX NN NN NN NN CC BB 00 00 ZZ ZZ ZZ ZZ
  --  r  p  l  y \0 \0 \0 \0  ,  b \0 \0 (data size)
  -- data size: big-endian 32-bit int

--  if (not string.find(data, "rply")) then
--    return
--  end

  -- actually, we just ignore the first 16 bytes; we know the size
  local content = string.sub(data, 15)

  -- TODO: join multiple lines?


  vim.schedule(function()
    vim.api.nvim_command(string.format('echo "%s"', content))
  end)
  

  -- TODO:
  -- * check for failure above
  -- * put data on statusline rather than echo line
  --   + vim.api.nvim_set_option('statusline', addr)
  -- * make statusline sub-variable to work within AirLine plugin
  -- * parse and validate data?
end

-- export the API of this plugin
-- 
return {
  send_code = send_code,
}
