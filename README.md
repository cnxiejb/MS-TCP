# MS-TCP
adjust tcp for media data using rtmp

1，仍然使用基于tcp协议的rtmp协议，数据包完全遵循两个协议
2，调整修改tcp协议的一些算法，使其符合流媒体实时性要求
3，以后加入udp推流模式

目标：使的推流程序可以根据网络状况自动调节码率、帧率等参数，充分利用网络带宽的同时，避免网络拥塞；同时支持自定义协议和标准rtmp协议
