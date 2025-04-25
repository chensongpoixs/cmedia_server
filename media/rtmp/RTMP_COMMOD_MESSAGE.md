# RTMP协议之命令消息


命令消息是指通过消息类型为0x11（AMF3编码）或者0x14（AMF0编码），并且需要对端执行某些操作的消息。

服务器和客户端之间使用 AMF 编码的命令消息交互。 一些命令消息被用来发送操作指令，比如 connect，createStream，public，play，pause。另外一些命令消息被用来通知发送方请求命令的状态，比如 onstatus，result 等。


##  1 命令类型

在介绍命令消息之前，先介绍两个类对象：

- NetConnection：服务器和客户端之间进行网络连接的一种高级表示形式。
- NetStream：代表了发送音频流，视频流，或其他相关数据的频道。当然还有一些像播放，暂停之类的命令，用来控制数据流。

这两个类对象规定了作用在其上的命令消息的csid，message stream id，以及回复消息的命令消息。



#### 4.1.1 NetConnection

在握手成功后，需要建立NetConnection来交互命令。所有运行在NetConnection上的命令消息：

- Chunk Stream Id (csid)：固定为3
- Message Stream Id：固定为0

可以运行在NetConnection上的命令：connect，call，close，createStream。

对端收到NetConnection命令消息后，需要对消息进行回复。

命令_result和_error用来回复NetConnection命令消息。结构如下：

```
    +--------------+----------+
    | Field Name   |   Type   |
    +--------------+----------+
    | Command Name |  String  |
    |              |          |
    +--------------+----------+
    | Transaction  |  Number  |
    | ID           |          |
    |              |          |
    +--------------+----------+
    | Properties   |  Object  |
    |              |          |
    |              |          |
    +--------------+----------+
    | Information  |  Object  |
    |              |          |
    |              |          |
    |              |          |
    +--------------+----------+
```

- Command Name：命令名称，出错回复 _error，否则回复_result。
- Transaction ID：交互ID，所要回复的命令消息携带的ID。
- Properties：命令参数。
- Information：回复消息携带的描述信息。Information至少要有三个属性：code``level``description。
	- code：status``error``warning中的一个
	- level：rtmp协议定义的常量属性
	- description：人类可读的信息描述


### 1.1.1 connect
客户端发送连接命令给服务器，建立NetConnection。

命令的结构：

```
    +----------------+---------+---------------------------------------+
    |  Field Name    |  Type   |           Description                 |
    +--------------- +---------+---------------------------------------+
    | Command Name   | String  | Name of the command. Set to "connect".|
    +----------------+---------+---------------------------------------+
    | Transaction ID | Number  | Always set to 1.                      |
    +----------------+---------+---------------------------------------+
    | Command Object | Object  | Command information object which has  |
    |                |         | the name-value pairs.                 |
    +----------------+---------+---------------------------------------+
    | Optional User  | Object  | Any optional information              |
    | Arguments      |         |                                       |
    +----------------+---------+---------------------------------------+
```

connect命令消息的 Command Object包含的键值对：

```
    +-----------+--------+-----------------------------+----------------+
    | Property  |  Type  |        Description          | Example Value  |
    +-----------+--------+-----------------------------+----------------+
    |   app     | String | The Server application name |    testapp     |
    |           |        | the client is connected to. |                |
    +-----------+--------+-----------------------------+----------------+
    | flashver  | String | Flash Player version. It is |    FMSc/1.0    |
    |           |        | the same string as returned |                |
    |           |        | by the ApplicationScript    |                |
    |           |        | getversion () function.     |                |
    +-----------+--------+-----------------------------+----------------+
    |  swfUrl   | String | URL of the source SWF file  | file://C:/     |
    |           |        | making the connection.      | FlvPlayer.swf  |
    +-----------+--------+-----------------------------+----------------+
    |  tcUrl    | String | URL of the Server.          | rtmp://local   |
    |           |        | It has the following format.| host:1935/test |
    |           |        | protocol://servername:port/ | app/instance1  |
    |           |        | appName/appInstance         |                |
    +-----------+--------+-----------------------------+----------------+
    |  fpad     | Boolean| True if proxy is being used.| true or false  |
    +-----------+--------+-----------------------------+----------------+
    |audioCodecs| Number | Indicates what audio codecs | SUPPORT_SND    |
    |           |        | the client supports.        | _MP3           |
    +-----------+--------+-----------------------------+----------------+
    |videoCodecs| Number | Indicates what video codecs | SUPPORT_VID    |
    |           |        | are supported.              | _SORENSON      |
    +-----------+--------+-----------------------------+----------------+
    |videoFunct-| Number | Indicates what special video| SUPPORT_VID    |
    |ion        |        | functions are supported.    | _CLIENT_SEEK   |
    +-----------+--------+-----------------------------+----------------+
    |  pageUrl  | String | URL of the web page from    | http://        |
    |           |        | where the SWF file was      | somehost/      |
    |           |        | loaded.                     | sample.html    |
    +-----------+--------+-----------------------------+----------------+
    | object    | Number | AMF encoding method.        |     AMF3       |
    | Encoding  |        |                             |                |
    +-----------+--------+-----------------------------+----------------+
```

音频编码属性的可选值：

原始 PCM，ADPCM，MP3，NellyMoser（5，8,11，16，22，44kHz），AAC，Speex。

```
    +----------------------+----------------------------+--------------+
    |      Codec Flag      |          Usage             |     Value    |
    +----------------------+----------------------------+--------------+
    |  SUPPORT_SND_NONE    | Raw sound, no compression  |    0x0001    |
    +----------------------+----------------------------+--------------+
    |  SUPPORT_SND_ADPCM   | ADPCM compression          |    0x0002    |
    +----------------------+----------------------------+--------------+
    |  SUPPORT_SND_MP3     | mp3 compression            |    0x0004    |
    +----------------------+----------------------------+--------------+
    |  SUPPORT_SND_INTEL   | Not used                   |    0x0008    |
    +----------------------+----------------------------+--------------+
    |  SUPPORT_SND_UNUSED  | Not used                   |    0x0010    |
    +----------------------+----------------------------+--------------+
    |  SUPPORT_SND_NELLY8  | NellyMoser at 8-kHz        |    0x0020    |
    |                      | compression                |              |
    +----------------------+----------------------------+--------------+
    |  SUPPORT_SND_NELLY   | NellyMoser compression     |    0x0040    |
    |                      | (5, 11, 22, and 44 kHz)    |              |
    +----------------------+----------------------------+--------------+
    |  SUPPORT_SND_G711A   | G711A sound compression    |    0x0080    |
    |                      | (Flash Media Server only)  |              |
    +----------------------+----------------------------+--------------+
    |  SUPPORT_SND_G711U   | G711U sound compression    |    0x0100    |
    |                      | (Flash Media Server only)  |              |
    +----------------------+----------------------------+--------------+
    |  SUPPORT_SND_NELLY16 | NellyMouser at 16-kHz      |    0x0200    |
    |                      | compression                |              |
    +----------------------+----------------------------+--------------+
    |  SUPPORT_SND_AAC     | Advanced audio coding      |    0x0400    |
    |                      | (AAC) codec                |              |
    +----------------------+----------------------------+--------------+
    |  SUPPORT_SND_SPEEX   | Speex Audio                |    0x0800    |
    +----------------------+----------------------------+--------------+
    | SUPPORT_SND_ALL      | All RTMP-supported audio   |    0x0FFF    |
    |                      | codecs                     |              |
    +----------------------+----------------------------+--------------+
```

视频编码属性的可选值：

Sorenson，V1，On2，V2，H264.

```
    +----------------------+----------------------------+--------------+
    |      Codec Flag      |            Usage           |    Value     |
    +----------------------+----------------------------+--------------+
    |  SUPPORT_VID_UNUSED  | Obsolete value             |    0x0001    |
    +----------------------+----------------------------+--------------+
    |  SUPPORT_VID_JPEG    | Obsolete value             |    0x0002    |
    +----------------------+----------------------------+--------------+
    | SUPPORT_VID_SORENSON | Sorenson Flash video       |    0x0004    |
    +----------------------+----------------------------+--------------+
    | SUPPORT_VID_HOMEBREW | V1 screen sharing          |    0x0008    |
    +----------------------+----------------------------+--------------+
    | SUPPORT_VID_VP6 (On2)| On2 video (Flash 8+)       |    0x0010    |
    +----------------------+----------------------------+--------------+
    | SUPPORT_VID_VP6ALPHA | On2 video with alpha       |    0x0020    |
    | (On2 with alpha      | channel                    |              |
    | channel)             |                            |              |
    +----------------------+----------------------------+--------------+
    | SUPPORT_VID_HOMEBREWV| Screen sharing version 2   |    0x0040    |
    | (screensharing v2)   | (Flash 8+)                 |              |
    +----------------------+----------------------------+--------------+
    | SUPPORT_VID_H264     | H264 video                 |    0x0080    |
    +----------------------+----------------------------+--------------+
    | SUPPORT_VID_ALL      | All RTMP-supported video   |    0x00FF    |
    |                      | codecs                     |              |
    +----------------------+----------------------------+--------------+
```

视频函数属性的可选值：


```
+----------------------+----------------------------+--------------+
    |    Function Flag     |           Usage            |     Value    |
    +----------------------+----------------------------+--------------+
    | SUPPORT_VID_CLIENT   | Indicates that the client  |       1      |
    | _SEEK                | can perform frame-accurate |              |
    |                      | seeks.                     |              |
    +----------------------+----------------------------+--------------+
```

对象编码属性的可选值：

```
    +----------------------+----------------------------+--------------+
    |    Encoding Type     |           Usage            |    Value     |
    +----------------------+----------------------------+--------------+
    |        AMF0          | AMF0 object encoding       |      0       |
    |                      | supported by Flash 6 and   |              |
    |                      | later                      |              |
    +----------------------+----------------------------+--------------+
    |        AMF3          | AMF3 encoding from         |      3       |
    |                      | Flash 9 (AS3)              |              |
    +----------------------+----------------------------+--------------+
```

connect 命令执行过程：

```
    +--------------+                              +-------------+
    |    Client    |             |                |    Server   |
    +------+-------+             |                +------+------+
           |              Handshaking done               |
           |                     |                       |
           |                     |                       |
           |                     |                       |
           |                     |                       |
           |----------- Command Message(connect) ------->|
           |                                             |
           |<------- Window Acknowledgement Size --------|
           |                                             |
           |<----------- Set Peer Bandwidth -------------|
           |                                             |
           |-------- Window Acknowledgement Size ------->|
           |                                             |
           |<------ User Control Message(StreamBegin) ---|
           |                                             |
           |<------------ Command Message ---------------|
           |       (_result- connect response)           |
           |                                             |
                 Message flow in the connect command
 
```

## 1.1.2 call

NetConnection中包含的 call 方法，会在接收端执行远程过程调用（RPC）。被调用的 RPC 方法名作为 call 方法的参数传输。

Command Object 的结构：

```
    +--------------+----------+----------------------------------------+
    |Field Name    |   Type   |             Description                |
    +--------------+----------+----------------------------------------+
    | Procedure    |  String  | Name of the remote procedure that is   |
    | Name         |          | called.                                |
    +--------------+----------+----------------------------------------+
    | Transaction  |  Number  | If a response is expected we give a    |
    |              |          | transaction Id. Else we pass a value of|
    | ID           |          | 0                                      |
    +--------------+----------+----------------------------------------+
    | Command      |  Object  | If there exists any command info this  |
    | Object       |          | is set, else this is set to null type. |
    +--------------+----------+----------------------------------------+
    | Optional     |  Object  | Any optional arguments to be provided  |
    | Arguments    |          |                                        |
    +--------------+----------+----------------------------------------+
````

应答的命令结构如下：

```
    +--------------+----------+----------------------------------------+
    | Field Name   |   Type   |             Description                |
    +--------------+----------+----------------------------------------+
    | Command Name |  String  | Name of the command.                   |
    |              |          |                                        |
    +--------------+----------+----------------------------------------+
    | Transaction  |  Number  | ID of the command, to which the        |
    | ID           |          | response belongs.
    +--------------+----------+----------------------------------------+
    | Command      |  Object  | If there exists any command info this  |
    | Object       |          | is set, else this is set to null type. |
    +--------------+----------+----------------------------------------+
    | Response     | Object   | Response from the method that was      |
    |              |          | called.                                |
    +------------------------------------------------------------------+
```

### 1.1.3 close

close命令用于关闭NetConnection。

### 1.1.4 createStream

客户端通过发送此消息给服务器来创建一个NetStream。音频，视频，和元数据都是通过 createStream 命令创建的流通道发布出去的。

从客户端发送给服务器的命令结构如下：

```
    +--------------+----------+----------------------------------------+
    | Field Name   |   Type   |             Description                |
    +--------------+----------+----------------------------------------+
    | Command Name |  String  | Name of the command. Set to            |
    |              |          | "createStream".                        |
    +--------------+----------+----------------------------------------+
    | Transaction  |  Number  | Transaction ID of the command.         |
    | ID           |          |                                        |
    +--------------+----------+----------------------------------------+
    | Command      |  Object  | If there exists any command info this  |
    | Object       |          | is set, else this is set to null type. |
    +--------------+----------+----------------------------------------+
```

从服务器发送给客户端的命令结构：

```
    +--------------+----------+----------------------------------------+
    | Field Name   |   Type   |             Description                |
    +--------------+----------+----------------------------------------+
    | Command Name |  String  | _result or _error; indicates whether   |
    |              |          | the response is result or error.       |
    +--------------+----------+----------------------------------------+
    | Transaction  |  Number  | ID of the command that response belongs|
    | ID           |          | to.                                    |
    +--------------+----------+----------------------------------------+
    | Command      |  Object  | If there exists any command info this  |
    | Object       |          | is set, else this is set to null type. |
    +--------------+----------+----------------------------------------+
    | Stream       |  Number  | The return value is either a stream ID |
    | ID           |          | or an error information object.        |
    +--------------+----------+----------------------------------------+
```

## 1.2 NetStream

在NetConnection建立后，调用createStream就创建了NetStream。NetStream用来传输音视频流以及相关的命令。所

有运行在NetStream上的命令消息：

- Chunk Stream Id (csid)：固定为3
- Message Stream Id：固定为1
- 可以运行在NetStream上的命令：play，play2，closeStream，deleteStream，receiveAudio，receiveVideo，publish，seek，pause。

服务器通过发送 onStatus 命令给客户端来通知网络流状态的更新。

```
    +--------------+----------+----------------------------------------+
    | Field Name   |   Type   |             Description                |
    +--------------+----------+----------------------------------------+
    | Command Name |  String  | The command name "onStatus".           |
    +--------------+----------+----------------------------------------+
    | Transaction  |  Number  | Transaction ID set to 0.               |
    | ID           |          |                                        |
    +--------------+----------+----------------------------------------+
    | Command      |  Null    | There is no command object for         |
    | Object       |          | onStatus messages.                     |
    +--------------+----------+----------------------------------------+
    | Info Object  | Object   | An AMF object having at least the      |
    |              |          | following three properties: "level"    |
    |              |          | (String): the level for this message,  |
    |              |          | one of "warning", "status", or "error";|
    |              |          | "code" (String): the message code, for |
    |              |          | example "NetStream.Play.Start"; and    |
    |              |          | "description" (String): a human-       |
    |              |          | readable description of the message.   |
    |              |          | The Info object MAY contain other      |
    |              |          | properties as appropriate to the code. |
    +--------------+----------+----------------------------------------+
               Format of NetStream status message commands.
```

### 1.2.1 play

客户端发送此命令来通知服务器开始播放流。多次使用此命令可以创建一个播放列表。如果想要创建一个动态播放列表来在不同的直播或点播流之间切换，可以通过多次调用播放命令，同时将 Reset 字段设置为 false。相反，如果想要立即播放指定的流，先清理掉之前的播放队列，再调用播放命令，同时将 Reset 字段设置true。

从客户端发送给服务器的命令结构如下：

```
    +--------------+----------+-----------------------------------------+
    | Field Name   |   Type   |             Description                 |
    +--------------+----------+-----------------------------------------+
    | Command Name |  String  | Name of the command. Set to "play".     |
    +--------------+----------+-----------------------------------------+
    | Transaction  |  Number  | Transaction ID set to 0.                |
    | ID           |          |                                         |
    +--------------+----------+-----------------------------------------+
    | Command      |   Null   | Command information does not exist.     |
    | Object       |          | Set to null type.                       |
    +--------------+----------+-----------------------------------------+
    | Stream Name  |  String  | Name of the stream to play.             |
    |              |          | To play video (FLV) files, specify the  |
    |              |          | name of the stream without a file       |
    |              |          | extension (for example, "sample"). To   |
    |              |          | play back MP3 or ID3 tags, you must     |
    |              |          | precede the stream name with mp3:       |
    |              |          | for example, "mp3:sample". To play     |
    |              |          | H.264/AAC files, you must precede the   |
    |              |          | stream name with mp4: and specify the   |
    |              |          | file extension. For example, to play the|
    |              |          | file sample.m4v,specify "mp4:sample.m4v"|
    |              |          |                                         |
    +--------------+----------+-----------------------------------------+
    | Start        |  Number  | An optional parameter that specifies    |
    |              |          | the start time in seconds. The default  |
    |              |          | value is -2, which means the subscriber |
    |              |          | first tries to play the live stream     |
    |              |          | specified in the Stream Name field. If a|
    |              |          | live stream of that name is not found,it|
    |              |          | plays the recorded stream of the same   |
    |              |          | name. If there is no recorded stream    |
    |              |          | with that name, the subscriber waits for|
    |              |          | a new live stream with that name and    |
    |              |          | plays it when available. If you pass -1 |
    |              |          | in the Start field, only the live stream|
    |              |          | specified in the Stream Name field is   |
    |              |          | played. If you pass 0 or a positive     |
    |              |          | number in the Start field, a recorded   |
    |              |          | stream specified in the Stream Name     |
    |              |          | field is played beginning from the time |
    |              |          | specified in the Start field. If no     |
    |              |          | recorded stream is found, the next item |
    |              |          | in the playlist is played.              |
    +--------------+----------+-----------------------------------------+
    | Duration     |  Number  | An optional parameter that specifies the|
    |              |          | duration of playback in seconds. The    |
    |              |          | default value is -1. The -1 value means |
    |              |          | a live stream is played until it is no  |
    |              |          | longer available or a recorded stream is|
    |              |          | played until it ends. If you pass 0, it |
    |              |          | plays the single frame since the time   |
    |              |          | specified in the Start field from the   |
    |              |          | beginning of a recorded stream. It is   |
    |              |          | assumed that the value specified in     |
    |              |          | the Start field is equal to or greater  |
    |              |          | than 0. If you pass a positive number,  |
    |              |          | it plays a live stream for              |
    |              |          | the time period specified in the        |
    |              |          | Duration field. After that it becomes   |
    |              |          | available or plays a recorded stream    |
    |              |          | for the time specified in the Duration  |
    |              |          | field. (If a stream ends before the     |
    |              |          | time specified in the Duration field,   |
    |              |          | playback ends when the stream ends.)    |
    |              |          | If you pass a negative number other     |
    |              |          | than -1 in the Duration field, it       |
    |              |          | interprets the value as if it were -1.  |
    +--------------+----------+-----------------------------------------+
    | Reset        | Boolean  | An optional Boolean value or number     |
    |              |          | that specifies whether to flush any     |
    |              |          | previous playlist.                      |
    +--------------+----------+-----------------------------------------+
```



play 播放命令执行流程：


```
         +-------------+                            +----------+
         | Play Client |             |              |   Server |
         +------+------+             |              +-----+----+
                |        Handshaking and Application       |
                |             connect done                 |
                |                    |                     |
                |                    |                     |
                |                    |                     |
                |                    |                     |
       ---+---- |------Command Message(createStream) ----->|
    Create|     |                                          |
    Stream|     |                                          |
       ---+---- |<---------- Command Message --------------|
                |     (_result- createStream response)     |
                |                                          |
       ---+---- |------ Command Message (play) ----------->|
          |     |                                          |
          |     |<------------  SetChunkSize --------------|
          |     |                                          |
          |     |<---- User Control (StreamIsRecorded) ----|
     Play |     |                                          |
          |     |<---- User Control (StreamBegin) ---------|
          |     |                                          |
          |     |<--Command Message(onStatus-play reset) --|
          |     |                                          |
          |     |<--Command Message(onStatus-play start) --|
          |     |                                          |
          |     |<-------------Audio Message---------------|
          |     |                                          |
          |     |<-------------Video Message---------------|
          |     |                    |                     |
                                     |
             Keep receiving audio and video stream till finishes
                 Message flow in the play command
```


play命令执行过程中的消息流如下：

- 当客户端接收到服务器返回的 createStream 成功的消息时，开始发送播放命令。
- 服务器接收到播放命令后，发送设置块大小的消息。
- 服务器发送一条用户控制消息，消息内包含了 StreamlsRecorded 事件和流ID。事件类型位于消息的前 2 个字节，流 ID位于消息的最后 4 个字节。
- 服务器发送一条用户控制消息，消息内包含了 StreamBegin 事件，用于通知客户端开始播放流。
- 如果客户端已经成功发送了播放命令，那么服务器发送两条 onStatus 命令给客户端，命令的内容为 NetStream.- Play.Start 和 NetStream.Play.Reset。服务器只有在客户端发送了设置有重置标签的播放命令后，才能发送 NetStream.Play.Reset命令。如果服务器找不到客户端请求播放的流，那么发送 NetStream.Play.StreamNotFound命令给客户端。

- 服务器发送音频和视频数据给客户端。



### 1.2.2 play2

与play命令的不同之处在于，play2 命令可以在不修改播放内容时间线的前提下切换到一个不同码率的流。服务器包含了多个不同码率的流文件用于支持客户端的play2 请求。

从客户端发送给服务器的命令结构如下：

```
    The command structure from the client to the server is as follows:
    +--------------+----------+----------------------------------------+
    | Field Name   |   Type   |             Description                |
    +--------------+----------+----------------------------------------+
    | Command Name |  String  | Name of the command, set to "play2".   |
    +--------------+----------+----------------------------------------+
    | Transaction  |  Number  | Transaction ID set to 0.               |
    | ID           |          |                                        |
    +--------------+----------+----------------------------------------+
    | Command      |   Null   | Command information does not exist.    |
    | Object       |          | Set to null type.                      |
    +--------------+----------+----------------------------------------+
    | Parameters   |  Object  | An AMF encoded object whose properties |
    |              |          | are the public properties described    |
    |              |          | for the flash.net.NetStreamPlayOptions |
    |              |          | ActionScript object.                   |
    +--------------+----------+----------------------------------------+
```


有关 NetStreamPlayOptions 对象的公开属性的说明详见 AS3 语言的文档。

此命令的消息流如下所示：

```
        +--------------+                          +-------------+
        | Play2 Client |              |           |    Server   |
        +--------+-----+              |           +------+------+
                 |      Handshaking and Application      |
                 |               connect done            |
                 |                    |                  |
                 |                    |                  |
                 |                    |                  |
                 |                    |                  |
        ---+---- |---- Command Message(createStream) --->|
    Create |     |                                       |
    Stream |     |                                       |
        ---+---- |<---- Command Message (_result) -------|
                 |                                       |
        ---+---- |------ Command Message (play) -------->|
           |     |                                       |
           |     |<------------ SetChunkSize ------------|
           |     |                                       |
           |     |<--- UserControl (StreamIsRecorded)----|
      Play |     |                                       |
           |     |<------- UserControl (StreamBegin)-----|
           |     |                                       |
           |     |<--Command Message(onStatus-playstart)-|
           |     |                                       |
           |     |<---------- Audio Message -------------|
           |     |                                       |
           |     |<---------- Video Message -------------|
           |     |                                       |
                 |                                       |
        ---+---- |-------- Command Message(play2) ------>|
           |     |                                       |
           |     |<------- Audio Message (new rate) -----|
     Play2 |     |                                       |
           |     |<------- Video Message (new rate) -----|
           |     |                    |                  |
           |     |                    |                  |
           |  Keep receiving audio and video stream till finishes
                                      |
                  Message flow in the play2 command
```

### 1.2.3 deleteStream

如果需要销毁网络流对象，可以通过NetStream发送删除流消息给服务器。

客户端发送给服务器的命令结构如下：


```
    The command structure from the client to the server is as follows:
    +--------------+----------+----------------------------------------+
    | Field Name   |   Type   |             Description                |
    +--------------+----------+----------------------------------------+
    | Command Name |  String  | Name of the command, set to            |
    |              |          | "deleteStream".                        |
    +--------------+----------+----------------------------------------+
    | Transaction  |  Number  | Transaction ID set to 0.               |
    | ID           |          |                                        |
    +--------------+----------+----------------------------------------+
    | Command      |  Null    | Command information object does not    |
    | Object       |          | exist. Set to null type.               |
    +--------------+----------+----------------------------------------+
    | Stream ID    |  Number  | The ID of the stream that is destroyed |
    |              |          | on the server.                         |
    +--------------+----------+----------------------------------------+
```

服务器接收到此消息后，不做任何回复。

### 1.2.4 receiveAudio

NetStream发送此消息通知服务器，是否要发送音频数据给客户端。

```
    The command structure from the client to the server is as follows:
    +--------------+----------+----------------------------------------+
    | Field Name   |   Type   |             Description                |
    +--------------+----------+----------------------------------------+
    | Command Name |  String  | Name of the command, set to            |
    |              |          | "receiveAudio".                        |
    +--------------+----------+----------------------------------------+
    | Transaction  |  Number  | Transaction ID set to 0.               |
    | ID           |          |                                        |
    +--------------+----------+----------------------------------------+
    | Command      |  Null    | Command information object does not    |
    | Object       |          | exist. Set to null type.               |
    +--------------+----------+----------------------------------------+
    | Bool Flag    |  Boolean | true or false to indicate whether to   |
    |              |          | receive audio or not.                  |
    +--------------+----------+----------------------------------------+

```

- 如果服务器接收到带有 fasle 标签的消息后，不做任何回复。
- 如果接收到带有 true 标签的消息，服务器回复带有NetStream.Seek.Notify 和 NetStream.Play.Start 的消息给客户端。

### 1.2.5 receiveVideo

NetStream发送此消息通知服务器，是否要发送视频数据给客户端。

客户端发送给服务器的命令结构如下：

```
    +--------------+----------+----------------------------------------+
    | Field Name   |   Type   |             Description                |
    +--------------+----------+----------------------------------------+
    | Command Name |  String  | Name of the command, set to            |
    |              |          | "receiveVideo".                        |
    +--------------+----------+----------------------------------------+
    | Transaction  |  Number  | Transaction ID set to 0.               |
    | ID           |          |                                        |
    +--------------+----------+----------------------------------------+
    | Command      |  Null    | Command information object does not    |
    | Object       |          | exist. Set to null type.               |
    +--------------+----------+----------------------------------------+
    | Bool Flag    |  Boolean | true or false to indicate whether to   |
    |              |          | receive video or not.                  |
    +--------------+----------+----------------------------------------+

```

如果服务器接收到带有 false 标签的消息后，不做任何回复。如果接收到带有 true 标签的消息，服务器回复带有
NetStream.Seek.Notify 和 NetStream.Play.Start 的消息给客户端。

1.2.6 publish
客户端发送此消息，用来发布一个有名字的流到服务器。其他客户端可以使用此流名来播放流，接收发布的音频，视频，以及其他数据消息。

客户端发送给服务器的命令结构如下：

```
    +--------------+----------+----------------------------------------+
    | Field Name   |   Type   |             Description                |
    +--------------+----------+----------------------------------------+
    | Command Name |  String  | Name of the command, set to "publish". |
    +--------------+----------+----------------------------------------+
    | Transaction  |  Number  | Transaction ID set to 0.               |
    | ID           |          |                                        |
    +--------------+----------+----------------------------------------+
    | Command      |  Null    | Command information object does not    |
    | Object       |          | exist. Set to null type.               |
    +--------------+----------+----------------------------------------+
    | Publishing   |  String  | Name with which the stream is          |
    | Name         |          | published.                             |
    +--------------+----------+----------------------------------------+
    | Publishing   |  String  | Type of publishing. Set to "live",     |
    | Type         |          | "record", or "append".                 |
    |              |          | record: The stream is published and the|
    |              |          | data is recorded to a new file.The file|
    |              |          | is stored on the server in a           |
    |              |          | subdirectory within the directory that |
    |              |          | contains the server application. If the|
    |              |          | file already exists, it is overwritten.|
    |              |          | append: The stream is published and the|
    |              |          | data is appended to a file. If no file |
    |              |          | is found, it is created.               |
    |              |          | live: Live data is published without   |
    |              |          | recording it in a file.                |
    +--------------+----------+----------------------------------------+

```

服务器接收到此消息后，回复 onStatus 命令来标记发布的开始。

publish流程：


```
         +--------------------+                     +-----------+
         |  Publisher Client  |        |            |    Server |
         +----------+---------+        |            +-----+-----+
                    |           Handshaking Done          |
                    |                  |                  |
                    |                  |                  |
           ---+---- |----- Command Message(connect) ----->|
              |     |                                     |
              |     |<----- Window Acknowledge Size ------|
      Connect |     |                                     |
              |     |<-------Set Peer BandWidth ----------|
              |     |                                     |
              |     |------ Window Acknowledge Size ----->|
              |     |                                     |
              |     |<------User Control(StreamBegin)-----|
              |     |                                     |
           ---+---- |<---------Command Message -----------|
                    |   (_result- connect response)       |
                    |                                     |
           ---+---- |--- Command Message(createStream)--->|
       Create |     |                                     |
       Stream |     |                                     |
           ---+---- |<------- Command Message ------------|
                    | (_result- createStream response)    |
                    |                                     |
           ---+---- |---- Command Message(publish) ------>|
              |     |                                     |
              |     |<------User Control(StreamBegin)-----|
              |     |                                     |
              |     |-----Data Message (Metadata)-------->|
              |     |                                     |
    Publishing|     |------------ Audio Data ------------>|
      Content |     |                                     |
              |     |------------ SetChunkSize ---------->|
              |     |                                     |
              |     |<----------Command Message ----------|
              |     |      (_result- publish result)      |
              |     |                                     |
              |     |------------- Video Data ----------->|
              |     |                  |                  |
              |     |                  |                  |
                    |    Until the stream is complete     |
                    |                  |                  |
              Message flow in publishing a video stream
```

publish命令执行过程中的消息流如下：

- 当客户端接收到服务器返回的 createStream 成功的消息时，开始publish命令。
- 服务器接收到publish命令后，发送一条用户控制消息，消息内包含了 NetStream.Publish.Start 事件，用于通知客户端开始推流。
- 客户端收到publish命令的回复后，发送SetChunkSize命令
- 客户端接着发送Meta数据，音频和视频数据给服务端，直到把数据发送完，或者主动断开。

### 1.2.7 seek

客户端发送此消息来定位多媒体文件或播放列表的偏移（以毫秒为单位）。

客户端发送给服务器的命令结构如下：

```
    +--------------+----------+----------------------------------------+
    | Field Name   |   Type   |             Description                |
    +--------------+----------+----------------------------------------+
    | Command Name |  String  | Name of the command, set to "seek".    |
    +--------------+----------+----------------------------------------+
    | Transaction  |  Number  | Transaction ID set to 0.               |
    | ID           |          |                                        |
    +--------------+----------+----------------------------------------+
    | Command      |  Null    | There is no command information object |
    | Object       |          | for this command. Set to null type.    |
    +--------------+----------+----------------------------------------+
    | milliSeconds |  Number  | Number of milliseconds to seek into    |
    |              |          | the playlist.                          |
    +--------------+----------+----------------------------------------+
```

当定位完成后，服务器回复 NetStream.Seek.Notify 状态消息给客户端。

如果定位失败，将回复 _error 消息。

### 1.2.7 pause

客户端发送此消息来通知服务器暂停或开始播放。

客户端发送给服务器的命令结构如下：


```
    +--------------+----------+----------------------------------------+
    | Field Name   |   Type   |             Description                |
    +--------------+----------+----------------------------------------+
    | Command Name |  String  | Name of the command, set to "pause".   |
    +--------------+----------+----------------------------------------+
    | Transaction  |  Number  | There is no transaction ID for this    |
    | ID           |          | command. Set to 0.                     |
    +--------------+----------+----------------------------------------+
    | Command      |  Null    | Command information object does not    |
    | Object       |          | exist. Set to null type.               |
    +--------------+----------+----------------------------------------+
    |Pause/Unpause |  Boolean | true or false, to indicate pausing or  |
    | Flag         |          | resuming play                          |
    +--------------+----------+----------------------------------------+
    | milliSeconds |  Number  | Number of milliseconds at which the    |
    |              |          | the stream is paused or play resumed.  |
    |              |          | This is the current stream time at the |
    |              |          | Client when stream was paused. When the|
    |              |          | playback is resumed, the server will   |
    |              |          | only send messages with timestamps     |
    |              |          | greater than this value.               |
    +--------------+----------+----------------------------------------+
```

当流暂停成功，服务器发送 NetStream.Pause.Notify 状态消息给客户端，如果流未暂停，服务器发送
NetStream.Unpause.Notify 状态消息给客户端。

如果暂停失败，则发送 _error 消息。

## 1.3 level和code

|level|	code|	说明|
|:---:|:---:|:---|
NetStream.Buffer.Empty|	status|	数据的接收速度不足以填充缓冲区。数据流将在缓冲区重新填充前中断，此时将发送 NetStream.Buffer.Full 消息，并且该流将重新开始播放
NetStream.Buffer.Full|	status|	缓冲区已满并且流将开始播放
NetStream.Buffer.Flush	|status|	数据已完成流式处理，剩余的缓冲区将被清空
NetStream.Failed|	error	|仅限 Flash Media Server 发生了错误，在其它事件代码中没有列出此错误的原因
NetStream.Publish.Start	|status	|已经成功发布
NetStream.Publish.BadName	|error	|试图发布已经被他人发布的流
NetStream.Publish.Idle	|status|	流发布者空闲而没有在传输数据
NetStream.Unpublish.Success	|status	|已成功执行取消发布操作
NetStream.Play.Start|	status|	播放已开始
NetStream.Play.Stop	|status	|播放已结束
NetStream.Play.Failed	|error|	出于此表中列出的原因之外的某一原因（例如订阅者没有读取权限），播放发生了错误
NetStream.Play.StreamNotFound	|error	|无法找到传递给 play() 方法的 FLV
NetStream.Play.Reset	|status|	由播放列表重置导致
NetStream.Play.PublishNotify|	status	|到流的初始发布被发送到所有的订阅者
NetStream.Play.UnpublishNotify	|status|	从流取消的发布被发送到所有的订阅者
NetStream.Play.InsufficientBW	|warning	|仅限 Flash Media Server 客户端没有足够的带宽，无法以正常速度播放数据
NetStream.Play.FileStructureInvalid	|error	|应用程序检测到无效的文件结构并且将不会尝试播放这种类型的文件。用于 AIR 和 Flash Player 9.0.115.0 及更高版本
NetStream.Play.NoSupportedTrackFoun	|error|	应用程序未检测到任何支持的音轨（视频、音频或数据）并且将不会尝试播放该文件。用于 AIR 和 Flash Player 9.0.115.0 及更高版本
NetStream.Play.Transition	|status	|仅限 Flash Media Server 由位速率流切换而导致的到其它位置的流过渡。此代码指示用于启动流切换的 NetStream.play2() 调用的成功状态事件。如果切换失败，则服务器将改为发送 NetStream.Play.Failed 事件。用于 Flash Player 10 及更高版本
NetStream.Play.Transition	|status	|仅限 Flash Media Server 3.5 和更高版本。服务器收到因位速率流切换而需要过渡到其他流的命令。此代码指示用于启动流切换的 NetStream.play2() 调用的成功状态事件。如果切换失败，则服务器将改为发送 NetStream.Play.Failed 事件。当发生流切换时，将调度带有代码“NetStream.Play.TransitionComplete”的 onPlayStatus 事件。用于 Flash Player 10 及更高版本
NetStream.Pause.Notify	|status|	流已暂停
NetStream.Unpause.Notify	|status|	流已恢复
NetStream.Record.Start	|status	|录制已开始
NetStream.Record.NoAccess	|error|	试图录制仍处于播放状态的流或客户端没有访问权限的流
NetStream.Record.Stop	|status|	录制已停止
NetStream.Record.Failed	|error|	尝试录制流失败
NetStream.Seek.Failed	|error|	搜索失败，如果流处于不可搜索状态，则会发生搜索失败
NetStream.Seek.InvalidTime|	error|	对于使用渐进式下载方式下载的视频，用户已尝试跳过到目前为止已下载的视频数据的结尾或在整个文件已下载后跳过视频的结尾进行搜寻或播放 message.details 属性包含一个时间代码，该代码指出用户可以搜寻的最后一个有效位置
NetStream.Seek.Notify	|status|	搜寻操作完成
NetConnection.Call.BadVersion	|error	|以不能识别的格式编码的数据包
NetConnection.Call.Failed	|error	|NetConnection.call 方法无法调用服务器端的方法或命令
NetConnection.Call.Prohibited	|error|	Action Message Format (AMF) 操作因安全原因而被阻止。AMF URL 与文件（其中包含调用 NetConnection.call() 方法的代码）不在同一个域中，或者 AMF 服务器没有信任文件（其中包含调用 NetConnection.call() 方法的代码）所在域的策略文件
NetConnection.Connect.Closed	|status	|成功关闭连接
NetConnection.Connect.Failed	|error	|连接尝试失败
NetConnection.Connect.Success	|status|	连接尝试成功
NetConnection.Connect.Rejected	|error	|连接尝试没有访问应用程序的权限
NetStream.Connect.Closed	|status	|成功关闭 P2P 连接
NetStream.Connect.Failed	|error	|P2P 连接尝试失败
NetStream.Connect.Success	|status	|P2P 连接尝试成功
NetStream.Connect.Rejected	|error	|P2P 连接尝试没有访问另一个对等方的权限
NetConnection.Connect.AppShutdown	|error	|正在关闭指定的应用程序
NetConnection.Connect.InvalidApp	|error|	连接时指定的应用程序名无效
SharedObject.Flush.Success	|status|	“待定”状态已解析并且 SharedObject.flush() 调用成功
SharedObject.Flush.Failed	|error	|“待定”状态已解析，但 SharedObject.flush() 失败
SharedObject.BadPersistence	|error	|使用永久性标志对共享对象进行了请求，但请求无法被批准，因为已经使用其它标记创建了该对象
SharedObject.UriMismatch	|error	|试图连接到拥有与共享对象不同的 URI (URL) 的 NetConnection 对象