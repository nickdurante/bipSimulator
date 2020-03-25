#!/usr/bin/env python
import asyncio
import websockets
import json
from sys import argv

IP = "192.168.4.1"
PORT = "6789"
async def hello():
    uri = "ws://"+ IP + ":" + PORT
    async with websockets.connect(uri) as websocket:
        await websocket.send(json.dumps({"type": "message", "from": "RPi3", "content": argv[1]}))

asyncio.get_event_loop().run_until_complete(hello())
