# kcp_client.py
from kcp import KCPClientSync
import time
from Messages.Message import Message
from Messages.Payload import Payload
from Messages.ErrorData import ErrorData


client = KCPClientSync(
    "127.0.0.1",
    6666,
    conv_id=287454020,
    no_delay=False,
    update_interval=40,
    resend_count=0,
    no_congestion_control=False,
    receive_window_size=32,
    send_window_size=32,
)


@client.on_data
def handle_data(data: bytes) -> None:
    msg = Message.GetRootAsMessage(data, 0)
    payload_type = msg.PayloadType()
    payload = msg.Payload()
    if payload_type == Payload.ErrorData:
        err = ErrorData()
        err.Init(payload.Bytes, payload.Pos)
        print("Error:", err.Text().decode())


@client.on_start
def on_start() -> None:
    print("Connected to server!")

    i = 0
    while True:
        client.send(f"Data{i}".encode())
        time.sleep(1)
        i += 1


client.start()
