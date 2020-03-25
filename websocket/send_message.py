#!/usr/bin/env python
import asyncio
import websockets
import json
from sys import argv


"""
    Takes argv[1] and sends it to the server sepcified
    python3 send_message.py '{"type": "message", "content": "rocknroll"}'
"""
IP = "192.168.4.1"
PORT = "6789"
uri = "ws://"+ IP + ":" + PORT

async def send_message():

    async with websockets.connect(uri) as websocket:
        await websocket.send(argv[1])


def main():
    if len(argv) != 2:
        print("Invalid number of inputs")
        return
    try:
        json.loads(argv[1])
    except:
        print("Invalid JSON input")
        return

    asyncio.get_event_loop().run_until_complete(send_message())

if __name__ == '__main__':
    main()
