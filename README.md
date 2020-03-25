# bipEmulator

## Run webapp:

```
cd webapp/
source venv/bin/activate
python3 manage.py runserver
```


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
