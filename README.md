# bipEmulator

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
