﻿#  RTMP协议格式实现



## 一、  RTMP协议格式


![RTMP格式](https://github.com/chensongpoixs/cmedia_server/blob/master/media/rtmp/rtmp_%E5%8D%8F%E8%AE%AE%E6%A0%BC%E5%BC%8F.jpeg?raw=true)









Basic Header由chunk type(fmt)和chunk stream id(csid)组成。
		  
Basic Header至少一个字节，结构如下图：

```javascript
			 0 1 2 3 4 5 6 7
			+-+-+-+-+-+-+-+-+
			|fmt|   cs id   |
			+-+-+-+-+-+-+-+-+
```

fmt 占用2位，取值0，1，2，3，分别表示Message Header的4种长度：11 bytes，7 bytes，3 bytes，0 bytes。

csid占用6位，采用小端存储，用来唯一标识一个流通道。csid取值[4,63]，Basic Header就只占用一个字节。csid取值0，1时，表示真正的csid存储接下来的字节中。csid取值2时，代表该 chunk 是控制信息和一些命令信息。


### 1.1 fmt = 0 代表含义

![fmt = 0](https://github.com/chensongpoixs/cmedia_server/blob/master/media/rtmp/rtmp_fmt_0.png?raw=true)

当fmt=0时，Message Header的长度为11 bytes，Message Header结构如下：

```
	       0               1               2               3
				 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
				+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
				|                    timestamp                  |message length |
				+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
				|    message length (coutinue)  |message type id| msg stream id |
				+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
				|                  msg stream id                |
				+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```
			
   - timestamp（时间戳）：占用3个字节，因此它最多能表示到 2^24-1。当它的值超过这个最大值时，这三个字节都置为1，这样实际的timestamp会转存到 Extended Timestamp 字段中。接收端在判断timestamp字段为0xFFFFFFFF时就会去Extended Timestamp中解析实际的时间戳。
   - message length（消息数据长度）：占用3个字节，表示实际发送的消息的长度，单位是字节。注意这里是Message的长度，也就是chunk属于的Message的总长度，而不是chunk本身的长度。
   - message type id(消息的类型id)：1个字节，表示实际发送的数据的类型。
   - message stream id(消息的流id)：4个字节，表示该消息所在的流的ID，和Basic Header的csid一样，它采用小端存储方式。

### 1.2 fmt = 1 代表含义

![fmt = 1](https://github.com/chensongpoixs/cmedia_server/blob/master/media/rtmp/rtmp_fmt_1.png?raw=true)

当fmt=1时，Message Header的长度为7 bytes，跟fmt=0的结构相比，少了message stream id字段，新结构如下：

```
   0               1               2               3
   0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |               timestamp delta                 |message length |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |    message length (coutinue)  |message type id|
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```
    
- timestamp delta：3 bytes，这里和type=0时不同，存储的是和上一个chunk的时间差。当它的值超过3个字节所能表示的最大值时，三个字节都置为1，实际的时间戳差值就会转存到Extended Timestamp字段中。接收端在判断timestamp delta字段为0xFFFFFFFF时就需要去Extended Timestamp 中解析实际的值。
- 其他的字段跟fmt=0时一样。

### 1.3 fmt = 2 代表含义

![fmt = 2](https://github.com/chensongpoixs/cmedia_server/blob/master/media/rtmp/rtmp_fmt_2.png?raw=true)
 
当fmt=2时，Message Header的长度为3 bytes，跟fmt=1的结构相比，少了message length字段和message type id字段，新结构如下：

```javascript

					0               1               2
					0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
					+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
					|               timestamp delta                 |
					+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

- timestamp delta跟fmt=1时意义一样。


### 1.4 当fmt=3时，表示当前chunk跟同csid的上一个chunk头信息一样，所以这里就没有Message Header。



