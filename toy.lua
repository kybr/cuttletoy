--
-- NeoVim Lua plugin for...
--
-- Karl Yerkes
-- 2025-04-18
--


local socket = nil

local remote = { address = "127.0.0.1", port = 7770 }
local port = 7771

--------------------------------------------------------------------------------
-- Listen for buffer changes; Spurt contents via UDP ---------------------------
--------------------------------------------------------------------------------

--local count = 0
local function receive(err, data, addr, flags)
    -- TODO:
    -- * put data on statusline rather than echo line
    --   + vim.api.nvim_set_option('statusline', addr)
    -- * make statusline sub-variable to work within AirLine plugin
    -- * parse and validate data?

    if (data) then
        --local message = ""
        --for k,v in pairs(addr) do
        --  message = message .. k .. ":"
        --  message = message .. v .. "\n"
        --end
        --vim.print(message)
        --vim.print(count .. "|" .. addr.ip .. ":" .. addr.port .. "|" .. data)
        vim.print(data)
    end
    --count = 1 + count
end

local function send_code()
    if (socket == nil) then
        socket = vim.uv.new_udp()
        if (socket == nil) then
            vim.print("failed to create udp socket")
            return
        end
        local bind = socket:bind("0.0.0.0", port);
        if (bind == nil) then
            vim.print("failed to bind udp socket")
            return
        end
        socket:send_buffer_size(65507)
        local size = socket:send_buffer_size()
        if (size ~= 65507) then
            vim.print("failed resize send buffer")
            return
        end
        socket:recv_start(receive)
        vim.print("listening on " .. port)
    end

    local lines = vim.api.nvim_buf_get_lines(0, 0, -1, true)
    local text = table.concat(lines, "\n") .. '\0'

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

    local header = "/f\0\0,s\0\0"
    local bytes = socket:try_send(header .. text, remote.address, remote.port)
    if (bytes < 0) then
        vim.print("failed send buffer")
        return
    end
end

return {
  send_code = send_code,
}
