# kcp_client.py
from kcp import KCPClientSync
from Messages import Message, Topics, Topic, Payload, QuoteType, ErrorData, Kline, HeartBeat
import flatbuffers
import time

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


def create_topics_message() -> bytes:
    subscribes = {
        QuoteType.QuoteType.K1min: "AAPL|GOOGL",
        QuoteType.QuoteType.K1d: "AAPL|MSFT",
    }

    builder = flatbuffers.Builder(1024)

    topic_offsets = []
    for quote_type in subscribes:
        # Serialize the 'symbols' string
        symbols = builder.CreateString(subscribes[quote_type])
        # Create Topic object
        Topic.Start(builder)
        Topic.AddType(builder, type=quote_type)
        Topic.AddSymbols(builder, symbols)
        topic_offsets.append(Topic.End(builder))

    # Create a vector of topics
    Topics.StartTopicsVector(builder, numElems=len(subscribes))
    for offset in topic_offsets:
        builder.PrependUOffsetTRelative(offset)
    topics_vector = builder.EndVector()

    # Create Topics object
    Topics.Start(builder)
    Topics.AddTopics(builder, topics_vector)
    Topics.AddDtStart(builder, 1633024800000)
    Topics.AddDtEnd(builder, 1633111200000)
    topics_offset = Topics.End(builder)

    # Create Message object
    Message.Start(builder)
    Message.AddPayloadType(builder, Payload.Payload.Subscribe)
    Message.AddPayload(builder, topics_offset)
    message_offset = Message.End(builder)

    builder.Finish(message_offset)
    buf = builder.Output()
    return bytes(buf)


def parse_message(buf):
    msg = Message.Message.GetRootAsMessage(buf, 0)
    payload_type = msg.PayloadType()
    payload = msg.Payload()

    if payload_type == Payload.Payload.K1min:
        kline = Kline.Kline()
        kline.Init(payload.Bytes, payload.Pos)

        # Access the fields
        symbol = kline.Symbol().decode("utf-8")
        timestamp = kline.Timestamp()
        amount = kline.Amount()
        volume = kline.Volume()
        num_trades = kline.NumTrades()
        preclose = kline.Preclose()
        open_price = kline.Open()
        high = kline.High()
        low = kline.Low()
        close = kline.Close()

        # Print or process the data
        print(f"k1min {symbol}, dt={timestamp}, amt={amount}, vol={volume}, ntrd={num_trades}, preclose={preclose}, open={open_price}, high={high}, low={low}, close={close}")
    elif payload_type == Payload.Payload.K1d:
        # Initialize the Kline object
        kline = Kline.Kline()
        kline.Init(payload.Bytes, payload.Pos)

        # Access the fields
        symbol = kline.Symbol().decode("utf-8")
        timestamp = kline.Timestamp()
        amount = kline.Amount()
        volume = kline.Volume()
        num_trades = kline.NumTrades()
        preclose = kline.Preclose()
        open_price = kline.Open()
        high = kline.High()
        low = kline.Low()
        close = kline.Close()

        # Print or process the data
        print(f"k1d {symbol}, dt={timestamp}, amt={amount}, vol={volume}, ntrd={num_trades}, preclose={preclose}, open={open_price}, high={high}, low={low}, close={close}")
    elif payload_type == Payload.Payload.ErrorData:
        error = ErrorData.ErrorData()
        error.Init(payload.Bytes, payload.Pos)
        print("Error:", error.Text().decode())
    else:
        print("Unsupported payload type")


def create_heartbeat() -> bytes:
    builder = flatbuffers.Builder(64)
    HeartBeat.Start(builder)
    hb = HeartBeat.End(builder)

    Message.Start(builder)
    Message.AddPayloadType(builder, Payload.Payload.HeartBeat)
    Message.AddPayload(builder, hb)
    msg = Message.End(builder)
    builder.Finish(msg)
    return bytes(builder.Output())


@client.on_data
def handle_data(data: bytes) -> None:
    parse_message(data)


heartbeat_buf = create_heartbeat()


@client.on_start
def on_start() -> None:
    print("Connected to server!")
    topics_buf = create_topics_message()
    client.send(topics_buf)

    # send heartbeat
    while True:
        client.send(heartbeat_buf)
        time.sleep(5)


client.start()
