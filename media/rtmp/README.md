﻿#  RTMP协议格式实现



## 一、  RTMP协议格式


![RTMP格式](https://github.com/chensongpoixs/cmedia_server/blob/master/media/rtmp/rtmp_%E5%8D%8F%E8%AE%AE%E6%A0%BC%E5%BC%8F.jpeg?raw=true)






### 1.1 Rtmp Header 中fmt类型


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


#### 1.1.1 fmt = 0 代表含义

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

#### 1.1.2 fmt = 1 代表含义

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

#### 1.1.3 fmt = 2 代表含义

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


使用是前一个包msg_type,msg_sid和ts， 


#### 1.1.4 当fmt=3时，表示当前chunk跟同csid的上一个chunk头信息一样，所以这里就没有Message Header。


全部使用前面的一个包 timestamp，msg_len ， msg_type, msg_sid




### 1.2 Message 发送数据

#### 1.2.1 Message Header 封装实现

- 当前Message跟前一个Message对比， 确定fmt的值
- 根据fmt的值，封装Message Header
- 通过csid缓存Message Header， 给下一次封装提供信息
- 通过csid 缓存delta值， 用于确定当前Message是否使用fmt3


#### 1.2.2 Extended Timestamp

- 启用条件： Message的Timestamp值大于0XFFFFFF
- 存放在Message Header后的4个字节


#### 1.2.3 Message Body发送

- Message Body是Chunk Data的承载内容
- 封装完Message Header和Extended Timestamp后， Chunk程下的大小（out_chunk_size-Chunk Header）存放部分Message Body
- 程下的Message Body封装成多个fmt3的Chunk发送


## 二、rtmp协议控制消息和用户控制消息

### 2.1 rtmp协议控制消息

rtmp协议控制消息是指设置Chunk Stream参数的消息。协议控制消息的消息类型为 1，2，3，5，6，消息的Message Stream ID固定为0，Chunk Stream ID固定为2。

下面分别介绍这几个控制消息

####  2.1.1 设置块大小

![chunk size (4096)](https://github.com/chensongpoixs/cmedia_server/blob/master/media/rtmp/rtmp_control_use_chunk_size.png?raw=true)

消息<font color='red'>设置块大小</font> 用来通知对方传输的Chunk的最大值。默认最大的块大小为 128 字节，客户端和服务器可以使用此消息来修改默认的块大小。

消息<font color='red'>设置块大小</font>的消息类型为1。

消息<font color='red'>设置块大小</font>的结构：


```
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |0|                     chunk size (31 bits)                    |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            Payload for the ‘Set Chunk Size’ protocol message
```

- 0：当前比特位必须为零。
- chunk size (31 bits)：本字段标识了新的最大块大小，以字节为单位，发送端之后将使用此值作为最大的块大小。

####  2.1.2 终止消息



![Abort Message](https://github.com/chensongpoixs/cmedia_server/blob/master/media/rtmp/rtmp_control_use_stream_id.png?raw=true)

<font color='red'>终止消息</font>用于通知对端，如果正在等待一条消息的部分块(已经接收了一部分)，那么可以丢弃之前已经接收到的块。对端将接收到的块流ID作为当前协议控制消息的有效负载。应用程序可能会在关闭的时候发送这个消息以指示不需要进一步对这个消息的处理了。

<font color='red'>终止消息</font>的消息类型为2。

<font color='red'>终止消息</font>的结构：

```
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                       chunk stream id (32 bits)               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            Payload for the ‘Abort Message’ protocol message
```

- chunk stream id (32 bits)：本字段包含了块流ID，用来标识哪个块流ID的消息将被丢弃

####  2.1.3 确认消息


![ack message](https://github.com/chensongpoixs/cmedia_server/blob/master/media/rtmp/rtmp_control_ack_seq.png?raw=true)

客户端或者服务器在接收到等同于窗口大小的字节之后必须发送给对端一个确认消息。窗口大小是指发送者在没有收到接收者确认消息之前发送的最大字节数。这个消息定义了序列号，也就是到目前为止接收到的字节数。

<font color='red'>确认消息</font>的消息类型为3。

<font color='red'>确认消息</font>的的结构：

```
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                        sequence number (4 bytes)              |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
           Payload for the ‘Acknowledgement’ protocol message
```

- sequence number (4 bytes)：本字段包含了截止目前接收到的数据总和，以字节为单位。

####  2.1.4 确认窗口大小消息


![acknowledgement window size](https://github.com/chensongpoixs/cmedia_server/blob/master/media/rtmp/rtmp_control_ack_window_size.png?raw=true)

客户端或服务端发送本消息来通知对方发送确认消息的窗口大小。

<font color='red'>确认窗口大小</font>的消息类型为5。

<font color='red'>确认窗口大小</font>的的结构：


```
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                   Acknowledgement Window size (4 bytes)       |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     Payload for the ‘Window Acknowledgement Size’ protocol message

```

- Acknowledgement Window size (4 bytes)：确认窗口的大小，以字节为单位。


####  2.1.5 设置对端带宽消息

![Set Peer Bandwidth](https://github.com/chensongpoixs/cmedia_server/blob/master/media/rtmp/rtmp_control_set_peer_bandwidth.png?raw=true)

客户端或服务端发送本消息更新对等端的输出带宽。输出带宽值与窗口大小值相同。如果对等端在本消息中收到的值与窗口大小不相同，则发回确认窗口大小消息。

<font color='red'>设置对端带宽消息</font>的消息类型为6。

<font color='red'>设置对端带宽消息</font>的的结构：

```
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                   Acknowledgement Window size                 |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |  Limit Type   |
    +-+-+-+-+-+-+-+-+
          Payload for the ‘Set Peer Bandwidth’ protocol message
```

- Acknowledgement Window size (4 bytes)：确认窗口的大小，以字节为单位。
- Limit Type：限制类型，取值：硬（0），软（1），或者动态（2）。

	如果是硬限制，对等端必须按提供的带宽发送数据。

	如果是软限制，对等端可以灵活决定带宽，发送端可以限制带宽。

	如果是动态限制，带宽既可以是硬限制也可以是软限制。


### 2.2.2 用户控制消息

用户控制消息用于告知对方执行该信息中包含的用户控制事件，消息类型为4，并且Message Stream ID使用0，Chunk Stream ID固定使用2。

用户控制消息在接收到后，需要马上生效。

结构：

```
    +------------------------------+------------------------
    |     Event Type (16 bits)     | Event Data
    +------------------------------+-------------------------
         Payload for the ‘User Control’ protocol message
```


- Event Type (16 bits)：事件类型。
- Event Data：事件数据。


|事件Event Type|描述|
|:---:|:---|
|Stream Begin (=0)|	服务器发送这个事件来通知客户端一个流已就绪并可以用来通信。默认情况下，这一事件在成功接收到客户端的连接命令之后以ID=0发送。事件数据为4字节，代表了已就绪流的流ID。|
|Stream EOF (=1)	|服务器发送这一事件来通知客户端请求的流的数据回放已经结束。在发送额外的命令之前不再发送任何数据。客户端将丢弃接收到的这个流的消息。事件数据为4字节，代表了回放已结束的流的流 ID。|
|StreamDry (=2)|	服务器发送这一事件来通知客户端当前流中已没有数据。当服务器在一段时间内没有检测到任何消息，它可以通知相关客户端当前流已经没数据了。这一事件数据为4字节，代表了已没数据的流的流 ID。|
|SetBuffer Length (=3)|	客户端发送这一事件来通知服务器缓冲区大小 (以毫秒为单位)，这个缓冲区用于缓存来自流的任何数据。此事件在服务器开始处理流之前就发送。事件数据的前4个字节代表了流ID，紧接其后的4个字节代表了以毫秒为单位的缓冲区的长度。|
|Streams Recorded (=4)|	服务器发送这一事件来通知客户端当前流是一个录制流。事件数据为4字节，代表了录制流的流 ID。|
|PingRequest (=6)	|服务器端发送这一事件用于测试客户端是否可达。事件数据是为一个4字节的时间戳，代表了服务器端发送这一命令时的服务器本地时间。客户端在接收到这一消息后会立即发送 PingResponse 回复。|
|PingResponse（=7）	|客户端发送这一事件用于回复服务器的PingRequest。事件数据是为一个4字节的时间戳，该时间戳是从接收到的PingRequest的事件数据中获取的。|


## 三、 AMF0和AMF3介绍

AMF 是随Flash Player 6 一起引入的，该版本称为 AMF0。它一直保持不变，直到 Flash Player 9 和ActionScript 3.0发布为止，当时新的数据类型和语言功能促使更新，称为 AMF3。

在Rtmp中，Message Type类型为0x14的包带的是AMF0序列化数据，而Message Type类型为0x11的包，带的是AMF3的数据。AMF3的Object类型中，body也是用AMF0来序列化，只是第一个字节是0x00，不起作用。因此，AMF3的Object，去掉第一个字节，就可以按AMF0的方式去解析。所以，这里就详细介绍AMF0的类型。

AMF的存储结构：一个字节的类型+N个字节的内容。不同的类型，内容不一样。AMF使用大端字节序存储数据。

AMF0类型：

|类型|	类型值	|说明|
|:---|:---:|:---|
|Number|	0×00|	数字，编码为 IEEE 64 位双精度浮点数|
|Boolean|	0×01|	布尔值，编码为单字节值 0x00 或 0x01|
|String|	0×02|	字符串，16 位整数字符串长度，UTF-8 字符串|
|Object|	0×03|	对象，键/值对集|
|Null|	0×05|	空值|
|MixedArray|	0×08	|ECMA 数组，32位的数组长度|
|EndOfObject	|0×09	|对象结束，前面是一个空的 16 位字符串长度（0x00 00）|
|Array	|0×0a	|严格数组，32位的数组个数|
|Date	|0×0b	|日期，编码为 IEEE 64 位双精度浮点数，具有 16 位整数时区偏移量|
|LongString	|0×0c	|长字符串，32 位整数字符串长度|
'

### 3.1.1 Number类型

Number类型是双精度的浮点数。

Number类型 = 0x00 + DOUBLE

其中DOUBLE是大端字节序存储的编码为 IEEE 64 位双精度浮点数。

### 3.1.2.Boolean类型

Boolean类型是布尔值true或者false。

Boolean类型 = 0x01 + 一个非负字节

其中非负字节的值0表示false，非0表示true

### 3.1.3.String类型

String类型是使用UTF-8编码的字符串。

String类型 = 0x02 + UTF-8

其中UTF-8由16位的长度+字符串内容组成。字符串的内容长度由前面的16位长度确定，最长65536。

### 3.1.4.Object类型

Object类型用来编码匿名对象。

Object类型 = 0x03 + Object-Property

Object-Property 由键值对组成：键名由16位的长度+字符串组成，键值是Object类型或者其他类型。

Object总是以0x000009结束。

### 3.1.5.Null类型

Null类型是空值。

Null类型=0x05

Null类型没有内容。

### 3.1.6.MixedArray类型

MixedArray类型跟Object一样，由键值对组成。

MixedArray类型=0x08 + count*Object-Property

count是4字节的整数，表示键值对的数量。

Object-Property 也是由键值对组成。

MixedArray也是以0x000009结束。

### 3.1.7.EndOfObject类型

EndOfObject类型表示Object结束。

EndOfObject类型=0x09

EndOfObject类型后面没有内容，前面有两个字节0x0000，总共3个字节0x000009指示Object结束。

### 3.1.8.Array类型

Array类型是指严格数组。

Array类型=0×0a + array-cout*value-type

array-cout是4字节数组长度

value-type指类型值

### 3.1.9.Date类型

Date，日期。

Date类型表示从1970-01-01T00:00:00到现在的毫秒数

Date类型=0x0b + DOUBLE + time-zone

DOUBLE为大端字节序存储的编码为 IEEE 64 位双精度浮点数

time-zone为16位的有符号整数

### 3.1.10.LongString类型

LongString类型是使用UTF-8编码的字符串。

LongString类型 = 0x0c + UTF-8

其中UTF-8由32位的长度+字符串内容组成。字符串的内容长度由前面的32位长度确定。




