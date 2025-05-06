#!/usr/bin/env python3.7

import iterm2

async def main(connection):
    app = await iterm2.async_get_app(connection)

    window = await iterm2.Window.async_create(connection)
    if window is None:
        return

    session = [[[],[],[],[]],[[],[],[],[]],[[],[],[],[]],[[],[],[],[]]]
    session[0][0] = app.current_terminal_window.current_tab.current_session

    session[0][3] = await session[0][0].async_split_pane()
    session[0][2] = await session[0][0].async_split_pane()
    session[0][1] = await session[0][0].async_split_pane()

    session[3][0] = await session[0][0].async_split_pane(True)
    session[2][0] = await session[0][0].async_split_pane(True)
    session[1][0] = await session[0][0].async_split_pane(True)

    session[3][1] = await session[0][1].async_split_pane(True)
    session[2][1] = await session[0][1].async_split_pane(True)
    session[1][1] = await session[0][1].async_split_pane(True)

    session[3][2] = await session[0][2].async_split_pane(True)
    session[2][2] = await session[0][2].async_split_pane(True)
    session[1][2] = await session[0][2].async_split_pane(True)

    session[3][3] = await session[0][3].async_split_pane(True)
    session[2][3] = await session[0][3].async_split_pane(True)
    session[1][3] = await session[0][3].async_split_pane(True)

    domain = iterm2.broadcast.BroadcastDomain()

    for column in range(4):
        for row in range(4):
            domain.add_session(session[column][row])
            await session[column][row].async_send_text("ssh pi@{}{}.cf".format(row, column))

    await iterm2.async_set_broadcast_domains(connection, [domain])

iterm2.run_until_complete(main)
