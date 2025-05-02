# CodecHeader的定义



- RTMP 推流FLV Tag 封装格式的数据
- 音频流的CodecHeader是AudioTag封装， 第二个字节的为0， 则是AAC equence header 
- 视频流CodecHeader是VideoTag封装， 第二个为0， 则是AVC sequence header
- Meta的CodecHeader是ScriptTag封装， 第二三个字节固定为0X000A



Meta解析


- Meta数据包有两个AMF object, 一个是一个字符串onMetadata， 另一个是一个MIXEDARRAY，里面是一些音视频相关的属性值
- 视频属性值： width， height， videocodecid， framerate， videodatarate
- 音频属性：audiosmpalerate，audiosamplesize,audiocodecid, audiodatarate
- 其它属性值： duration，encoder， server

Header查找

- 使用将要发送Packet的index查找对应的Header
- index应该比Header的index要大， 查找第一个比index小的Header



video_header_
audio_header_
meta_
meta_version_
video_version_
audio_version_
meta_packets_
video_header_packets_
audio_header_packets_
start_timestamp_


