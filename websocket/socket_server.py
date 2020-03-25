import serial
import asyncio
import websockets
import json

IP = "192.168.4.1"
PORT = "6789"
ser = serial.Serial('/dev/ttyS0', 9600, timeout=1)

def main():
    while True:
        while ser.in_waiting:
            message_in = ser.readline().decode()
            print("Read: " + message_in )
            asyncio.get_event_loop().run_until_complete(hello("Arduino", message_in))


async def hello(sender, message):
    uri = "ws://"+ IP + ":" + PORT
    async with websockets.connect(uri) as websocket:
        await websocket.send(json.dumps({"type": "message", "from": sender, "content": message}))


if __name__ == '__main__':
    main()
