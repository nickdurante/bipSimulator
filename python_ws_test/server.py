#!/usr/bin/env python

# WS server example that synchronizes state across clients

import asyncio
import json
import websockets

IP = "localhost"
PORT = 6789
USERS = set()


async def broadcast_message(message):
    await asyncio.wait([user.send(message) for user in USERS])


async def ws_server(websocket, path):
    # register(websocket) sends user_event() to websocket
    print("Running server at: " + IP + " " + str(PORT))
    # add connected ws to set
    USERS.add(websocket)
    try:
        #await websocket.send(state_event())
        async for message in websocket:
            data = json.loads(message)
            print(data)
            await broadcast_message(message)

    except:
        print("uh oh")
    finally:
        USERS.remove(websocket)

def main():
    start_server = websockets.serve(ws_server, IP, PORT)
    asyncio.get_event_loop().run_until_complete(start_server)
    asyncio.get_event_loop().run_forever()


if __name__ == '__main__':
    main()
