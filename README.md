# QuoteMocker

Mocker for Tick or Bar data

## pubsub

Publisher-Subscriber mode is not suitable for quote mocker
> it's hard for subscribe with different conditions

```bash
./pub tcp://127.0.0.1:9876
./sub tcp://127.0.0.1:9876
```

## pair1

## heartbeat mechanism

[heartbeat](https://blog.csdn.net/qq_42227818/article/details/103980102)
- method1: 客户端定时向服务器发heartbeat
  - 服务器可以ack
  - 也可以不ack,只是记录客户端last_time（大部分采用）
- method2: 服务端定时向客户器发heartbeat; 客户端立刻ack; 如果服务端没有收到ack(比如5次),则移除客户端信息。使用较少
- method3: 双向发心跳包，使用更少

对于method1心跳配置
- Heartbeat interval(HI): 一般发送heartbeat间隔为30s
- Threshold: 判断alive的threashold: `current_time - last_time > HI * (2~3)`, 一般选 60s
- checking interval: 一般设置为`< 0.5*Threshold`, 20s