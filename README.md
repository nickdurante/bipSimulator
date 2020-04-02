# bipEmulator

## Introduction

This is a collection of utilities to view and debug BipOS programs on a PC.

## C libraries:

1. [cJSON](https://github.com/DaveGamble/cJSON)
2. [wsServer](https://github.com/Theldus/wsServer)

Both are precompiled in the static library `libws.a`, to compile:

```
cd c_stuff/src
make
```

To run: 
```
./simulator
```

## Run webapp:

```
cd webapp/
source venv/bin/activate
python3 manage.py runserver
```

Now it is running at [http://localhost:8000/debug/](http://localhost:8000/debug/)

## Run ws socket

```
cd websocket/
python3 server.py
```

## Send a message to ws socket

```
cd websocket/
python3 send_message.py '{"type": "message", "content": "rocknroll"}'
```
