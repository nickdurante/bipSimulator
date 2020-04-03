#!/usr/bin/env python

# WS server example that synchronizes state across clients

import asyncio
import json
import websockets
import serial

IP = "localhost"
PORT = 6789


STATE = {"value": 0}
USERS = set()
MESSAGE_LIST = []

def state_event():
    return json.dumps({"type": "state", **STATE})


def users_event():
    return json.dumps({"type": "users", "count": len(USERS)})

def serial_event():
    last_message = MESSAGE_LIST[len(MESSAGE_LIST) - 1]

    return json.dumps(last_message)


async def notify_state():
    if USERS:  # asyncio.wait doesn't accept an empty list
        message = state_event()
        await asyncio.wait([user.send(message) for user in USERS])


async def notify_users():
    if USERS:  # asyncio.wait doesn't accept an empty list
        message = users_event()
        await asyncio.wait([user.send(message) for user in USERS])

async def broadcast_message(message):
    await asyncio.wait([user.send(message) for user in USERS])


async def register(websocket):
    USERS.add(websocket)
    await notify_users()


async def unregister(websocket):
    USERS.remove(websocket)
    await notify_users()


async def counter(websocket, path):
    # register(websocket) sends user_event() to websocket
    print("Running server at: " + IP + " " + str(PORT))
    await register(websocket)
    try:
        await websocket.send(state_event())
        async for message in websocket:
            data = json.loads(message)
            print(data)
            await broadcast_message(message)
    finally:
        await unregister(websocket)


start_server = websockets.serve(counter, IP, PORT)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
