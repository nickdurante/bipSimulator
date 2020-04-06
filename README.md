# bipEmulator

## Introduction

This is a collection of utilities to view and debug BipOS applications on a PC.

## C libraries:

1. [cJSON](https://github.com/DaveGamble/cJSON)
2. [wsServer](https://github.com/Theldus/wsServer)

Both are precompiled in the static library `libws.a`, to compile:

```
cd c_stuff/src
make all
```

To run:
```
cd c_stuff/src/
./bipsim
```

## Run webapp:
Open locally ```index.html``` and check that the websocket server is running on ```localhost:6789```

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
