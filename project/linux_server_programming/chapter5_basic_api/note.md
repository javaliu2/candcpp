# 第五章 Linux网络编程基础API

## 5.8 带外数据

悄悄告诉你，Typora是Type和Opera的组合，前者表示输入或者打字，后者表示作品或者创作。

接收方程序运行结果：

```shell
${path}/build$ ./oob_recv 127.0.0.1 54321
got 5 bytes of normal data '123ab'
got 1 bytes of oob data 'c'
got 3 bytes of normal data '123'
```

发送方程序运行命令：

```shell
${path}/build$ ./oob_send 127.0.0.1 54321
```

tcpdump捕捉到的数据包，注意，捕捉接口选择lo，即本地回环。因为TCP头部有附带选项，因此不是普通的20字节。

```shell
(base) xs@xslab:~$ sudo tcpdump -ntx -i lo port 54321  # x表示以16进制打印每个数据报头部，但不包含数据链路层
tcpdump: verbose output suppressed, use -v[v]... for full protocol decode
listening on lo, link-type EN10MB (Ethernet), snapshot length 262144 bytes
# 以下是三次握手
IP 127.0.0.1.59514 > 127.0.0.1.54321: Flags [S], seq 4186837713, win 65495, options [mss 65495,sackOK,TS val 4153872862 ecr 0,nop,wscale 7], length 0
	0x0000:  4500 003c 17e5 4000 4006 24d5 7f00 0001
	0x0010:  7f00 0001 e87a d431 f98e 12d1 0000 0000
	0x0020:  a002 ffd7 fe30 0000 0204 ffd7 0402 080a
	0x0030:  f797 11de 0000 0000 0103 0307
IP 127.0.0.1.54321 > 127.0.0.1.59514: Flags [S.], seq 1691685456, ack 4186837714, win 65483, options [mss 65495,sackOK,TS val 4153872862 ecr 4153872862,nop,wscale 7], length 0
	0x0000:  4500 003c 0000 4000 4006 3cba 7f00 0001
	0x0010:  7f00 0001 d431 e87a 64d5 1250 f98e 12d2
	0x0020:  a012 ffcb fe30 0000 0204 ffd7 0402 080a
	0x0030:  f797 11de f797 11de 0103 0307
IP 127.0.0.1.59514 > 127.0.0.1.54321: Flags [.], ack 1, win 512, options [nop,nop,TS val 4153872862 ecr 4153872862], length 0
	0x0000:  4500 0034 17e6 4000 4006 24dc 7f00 0001
	0x0010:  7f00 0001 e87a d431 f98e 12d2 64d5 1251
	0x0020:  8010 0200 fe28 0000 0101 080a f797 11de
	0x0030:  f797 11de
# 以下是第一个普通数据包
IP 127.0.0.1.59514 > 127.0.0.1.54321: Flags [P.], seq 1:4, ack 1, win 512, options [nop,nop,TS val 4153872862 ecr 4153872862], length 3
	0x0000:  4500 0037 17e7 4000 4006 24d8 7f00 0001
	0x0010:  7f00 0001 e87a d431 f98e 12d2 64d5 1251
	0x0020:  8018 0200 fe2b 0000 0101 080a f797 11de
	0x0030:  f797 11de 3132 33
IP 127.0.0.1.54321 > 127.0.0.1.59514: Flags [.], ack 4, win 512, options [nop,nop,TS val 4153872862 ecr 4153872862], length 0
	0x0000:  4500 0034 2eeb 4000 4006 0dd7 7f00 0001
	0x0010:  7f00 0001 d431 e87a 64d5 1251 f98e 12d5
	0x0020:  8010 0200 fe28 0000 0101 080a f797 11de
	0x0030:  f797 11de
# 以下是第二个紧急数据包 
IP 127.0.0.1.59514 > 127.0.0.1.54321: Flags [P.U], seq 4:7, ack 1, win 512, urg 3, options [nop,nop,TS val 4153872862 ecr 4153872862], length 3
	0x0000:  4500 0037 17e8 4000 4006 24d7 7f00 0001
	0x0010:  7f00 0001 e87a d431 f98e 12d5 64d5 1251
	0x0020:  8038 0200 fe2b 0003 0101 080a f797 11de
	0x0030:  f797 11de 6162 63
IP 127.0.0.1.54321 > 127.0.0.1.59514: Flags [.], ack 7, win 512, options [nop,nop,TS val 4153872862 ecr 4153872862], length 0
	0x0000:  4500 0034 2eec 4000 4006 0dd6 7f00 0001
	0x0010:  7f00 0001 d431 e87a 64d5 1251 f98e 12d8
	0x0020:  8010 0200 fe28 0000 0101 080a f797 11de
	0x0030:  f797 11de
# 以下是第三个普通数据包
IP 127.0.0.1.59514 > 127.0.0.1.54321: Flags [P.], seq 7:10, ack 1, win 512, options [nop,nop,TS val 4153872863 ecr 4153872862], length 3
	0x0000:  4500 0037 17e9 4000 4006 24d6 7f00 0001
	0x0010:  7f00 0001 e87a d431 f98e 12d8 64d5 1251
	0x0020:  8018 0200 fe2b 0000 0101 080a f797 11df
	0x0030:  f797 11de 3132 33
IP 127.0.0.1.54321 > 127.0.0.1.59514: Flags [.], ack 10, win 512, options [nop,nop,TS val 4153872863 ecr 4153872863], length 0
	0x0000:  4500 0034 2eed 4000 4006 0dd5 7f00 0001
	0x0010:  7f00 0001 d431 e87a 64d5 1251 f98e 12db
	0x0020:  8010 0200 fe28 0000 0101 080a f797 11df
	0x0030:  f797 11df
# 以下是关闭连接过程，接收端同时响应FIN和ACK，所以是三次挥手
IP 127.0.0.1.59514 > 127.0.0.1.54321: Flags [F.], seq 10, ack 1, win 512, options [nop,nop,TS val 4153872863 ecr 4153872863], length 0
	0x0000:  4500 0034 17ea 4000 4006 24d8 7f00 0001
	0x0010:  7f00 0001 e87a d431 f98e 12db 64d5 1251
	0x0020:  8011 0200 fe28 0000 0101 080a f797 11df
	0x0030:  f797 11df
IP 127.0.0.1.54321 > 127.0.0.1.59514: Flags [F.], seq 1, ack 11, win 512, options [nop,nop,TS val 4153872863 ecr 4153872863], length 0
	0x0000:  4500 0034 2eee 4000 4006 0dd4 7f00 0001
	0x0010:  7f00 0001 d431 e87a 64d5 1251 f98e 12dc
	0x0020:  8011 0200 fe28 0000 0101 080a f797 11df
	0x0030:  f797 11df
IP 127.0.0.1.59514 > 127.0.0.1.54321: Flags [.], ack 2, win 512, options [nop,nop,TS val 4153872863 ecr 4153872863], length 0
	0x0000:  4500 0034 17eb 4000 4006 24d7 7f00 0001
	0x0010:  7f00 0001 e87a d431 f98e 12dc 64d5 1252
	0x0020:  8010 0200 fe28 0000 0101 080a f797 11df
	0x0030:  f797 11df

```

以第一个数据报为例说明IP头部、TCP头部

![IPv4-header](./img/ip_header)

```shell
IP 127.0.0.1.59514 > 127.0.0.1.54321: Flags [S], seq 4186837713, win 65495, options [mss 65495,sackOK,TS val 4153872862 ecr 0,nop,wscale 7], length 0
	0x0000:  4500 003c 17e5 4000 4006 24d5 7f00 0001
	0x0010:  7f00 0001 # 前面20字节是IP头部，
	# IP第一行（32位）：4表示IP版本为4，5表示头部长度5*4==20字节，00表示服务类型，003c表示数据总长度，即IP头部+payload数据，这里是3c，即60字节，这一个数据报确实是60字节；
	# IP第二行：17e5表示该IP数据报的唯一标识，三位标志010，第一位保留，第二位表示是否设置分片，这里是1，表示不分片，第三位表示是否还有更多分片，01000000 00000000，所以是4000
	# IP第三行：TTL被设置为64，06表示IP携带的上层协议是TCP，24d5是头部校验和
	# IP第四行：7f000001是127.0.0.1的32位整数值，表示源IP地址
	# IP第五行：目标IP地址
	0x0010:            e87a d431 f98e 12d1 0000 0000
	0x0020:  a002 ffd7 fe30 0000 0204 ffd7 0402 080a
	0x0030:  f797 11de 0000 0000 0103 0307
```

![TCP-header](./img/tcp_header)

选项字段格式（G-bro笔记）：

TCP选项字段的格式是根据 TCP 报文头部的 **数据偏移量（Data Offset）** 来定义的，并且这个字段是可选的，长度不固定，通常位于 **TCP头部的尾部**。它用于扩展 TCP 协议的功能，例如最大报文段大小（MSS）、时间戳、窗口缩放因子等。

### TCP选项字段的基本格式：

每个TCP选项都有一个特定的格式，通常包括以下几个部分：

1. **选项类型**（Option Type）：1字节，标识选项的类型。
2. **选项长度**（Option Length）：1字节，表示该选项所占的字节数（包括类型和长度字段）。
3. **选项数据**（Option Data）：可变长度，包含该选项的具体信息。

#### 选项的典型类型：

1. **最大报文段大小（MSS - Maximum Segment Size）**

   - **选项类型**：0x02（2）
   - **选项长度**：4（包含类型和长度字段）
   - **数据**：2字节，表示最大报文段大小。

   **示例**：

   ```
   复制代码
   02 04 05 b4
   ```

   解释：

   - `02`：选项类型 MSS。
   - `04`：选项长度（包含类型和长度字段，总共4字节）。
   - `05 b4`：最大报文段大小，1460字节。

2. **时间戳（Timestamp）**

   - **选项类型**：0x08（8）

   - **选项长度**：10（包含类型、长度和数据）

   - 数据

     ：

     - 4字节：时间戳值（TSval），表示发送方的时间戳。
     - 4字节：回显时间戳（TSecr），表示接收方的时间戳。

   **示例**：

   ```
   复制代码
   08 0a f7 97 11 de 00 00 00 00
   ```

   解释：

   - `08`：选项类型 Timestamp。
   - `0a`：选项长度，总共10字节。
   - `f7 97 11 de`：发送方的时间戳（TSval）。
   - `00 00 00 00`：回显时间戳（TSecr）。

3. **选择性确认（SACK）**

   - **选项类型**：0x05（5）
   - **选项长度**：4或8字节（可变，根据选择性确认块的数量）。
   - **数据**：表示在数据包中收到的确认信息，通常用于表示某些数据已被成功接收。

   **示例**：

   ```
   复制代码
   05 02 00 00
   ```

   解释：

   - `05`：选项类型 SACK。
   - `02`：选项长度。
   - `00 00`：表示某个确认的包。

4. **窗口缩放（Window Scale）**

   - **选项类型**：0x03（3）
   - **选项长度**：3字节。
   - **数据**：一个字节，表示窗口缩放因子。

   **示例**：

   ```
   复制代码
   03 03 07
   ```

   解释：

   - `03`：选项类型 Window Scale。
   - `03`：选项长度。
   - `07`：窗口缩放因子值。

#### TCP选项字段的完整格式（按字节排列）：

```
+------------------+------------------+--------------------+
| Option Type (1B) | Option Length (1B) | Option Data (Variable) |
+------------------+------------------+--------------------+
```

#### 常见的TCP选项字段及其描述：

1. **MSS（Maximum Segment Size）**:
   - 用于告知对方双方通信的最大数据报文段大小。
   - 最大报文段大小限制了每次数据传输时可以发送的数据量。
2. **Timestamp（时间戳）**:
   - 用于实现时间戳选项，帮助估算往返时间（RTT）。
   - 时间戳选项通常用于高延迟的网络中，以更好地控制流量。
3. **SACK（Selective Acknowledgement）**:
   - 用于更精确地指定哪些数据已经成功接收。
   - SACK 允许接收方在一个数据包中指定多个不连续的数据段，避免了传统的累计确认方式。
4. **Window Scale（窗口缩放因子）**:
   - 用于支持大窗口，以便在高带宽延迟网络中提高吞吐量。
   - 该选项与 `MSS` 一起使用，可以帮助更高效地管理流量。
5. **NOP（No-Operation）**:
   - 无操作选项，仅用于填充。
   - NOP 通常出现在选项字段中用于对齐其他选项或作为填充。
6. **EOL（End of Option List）**:
   - 结束选项列表，用于标识选项部分的结束。

```shell
0x0010:            e87a d431 f98e 12d1 0000 0000
0x0020:  a002 ffd7 fe30 0000 0204 ffd7 0402 080a
0x0030:  f797 11de 0000 0000 0103 0307
# TCP第一行（32位）：e87a表示源端口，十进制值为14*16^3+8*16^2+7*16+10==59514，d431是目的端口，十进制值为54321
# TCP第二行：f98e 12d1序列号，十进制值为4186837713
# TCP第三行：0000 0000确认号，为0
# TCP第四行：头部长度a，即10，10*4==40字节；02表示flag位，SYN位置1，其他位为0；ffd7表示窗口大小为65495
# TCP第五行：fe30表示校验和；0000表示紧急指针
# TCP选项字段：0204 ffd7：表示这是MSS，最大报文段大小为ffd7，即65495
# 0402：支持SACK
# 080a：时间戳，源：f797 11de，目的地：0000 0000
# 0103：No Operation，无操作
# 0307：窗口缩放因子7
```

分析紧急数据包 紧急指针 项：

```shell
IP 127.0.0.1.59514 > 127.0.0.1.54321: Flags [P.U], seq 4:7, ack 1, win 512, urg 3, options [nop,nop,TS val 4153872862 ecr 4153872862], length 3
0x0000:  4500 0037 17e8 4000 4006 24d7 7f00 0001
0x0010:  7f00 0001 
                   e87a d431 f98e 12d5 64d5 1251
0x0020:  8038 0200 fe2b 0003 0101 080a f797 11de  # 0003为紧急指针项所占16位，值为3，表示相对于该报文序列号的偏移
0x0030:  f797 11de 6162 63
```

## 5.11 socket选项

设置client的发送缓冲区大小，设置server的接收缓冲区大小

运行server：

```shell
./recvbuf_server 127.0.0.1 54321 50
the tcp receive buffer size after setting is 2304
accepted one connnection
recv from client: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
```

运行client：

```shell
./sendbuf_client 127.0.0.1 54321 2000
the tcp send buffer size after setting is 4608
```

可以发现，操作系统实际设置的buffer大小比我们提供的要大。TCP报文中的win size肯定比buffer size要小，因为有好几个win，已经确认的win，啥的。

```shell
(base) xs@xslab:~$ sudo tcpdump -nt -i lo port 54321
tcpdump: verbose output suppressed, use -v[v]... for full protocol decode
listening on lo, link-type EN10MB (Ethernet), snapshot length 262144 bytes
# 三次挥手建立连接
IP 127.0.0.1.52424 > 127.0.0.1.54321: Flags [S], seq 1684785479, win 65495, options [mss 65495,sackOK,TS val 4172211700 ecr 0,nop,wscale 7], length 0
IP 127.0.0.1.54321 > 127.0.0.1.52424: Flags [S.], seq 3722465560, ack 1684785480, win 1152, options [mss 65495,sackOK,TS val 4172211700 ecr 4172211700,nop,wscale 0], length 0  # server win size: 1152
IP 127.0.0.1.52424 > 127.0.0.1.54321: Flags [.], ack 1, win 512, options [nop,nop,TS val 4172211700 ecr 4172211700], length 0  # client win size: 512
# 传输数据
IP 127.0.0.1.52424 > 127.0.0.1.54321: Flags [P.], seq 1:513, ack 1, win 512, options [nop,nop,TS val 4172211700 ecr 4172211700], length 512  # 发送521字节个数据
IP 127.0.0.1.54321 > 127.0.0.1.52424: Flags [.], ack 513, win 640, options [nop,nop,TS val 4172211700 ecr 4172211700], length 0
# 四次挥手断开连接
IP 127.0.0.1.52424 > 127.0.0.1.54321: Flags [F.], seq 513, ack 1, win 512, options [nop,nop,TS val 4172211700 ecr 4172211700], length 0  # 女：我要走了。
IP 127.0.0.1.54321 > 127.0.0.1.52424: Flags [F.], seq 1, ack 514, win 640, options [nop,nop,TS val 4172211700 ecr 4172211700], length 0  # 男：好，你走吧。
IP 127.0.0.1.52424 > 127.0.0.1.54321: Flags [.], ack 2, win 512, options [nop,nop,TS val 4172211700 ecr 4172211700], length 0  # 女：我真的走了。
```

