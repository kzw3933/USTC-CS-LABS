# lab4实验报告

<center>
    <p>姓名:   柯志伟</p>
    <p>学号: PB20061338</p>
</center>


## 实验目的

- 了解VLAN交换机的特性与应用场合

- 掌握VLAN交换机组网的基本配置方法


## 实验环境与器材 

- 环境
  模拟软件Cisco Packet Tracer 5.2
  Windows

- 实验器材
  3560交换机2台
  PC机4台，Laptop2台
  连接线若干


## 实验要求

- 搭建场景:
A1、A2、A3属于技术部
B1、B2、B3属于销售部
要求同一部门的主机在同一个局域网上


![](Snipaste_2022-11-26_21-47-30.png)

## 实验过程(学号为PB20061338,选择switch的18号接口)

### 网络搭建

#### 网络拓扑结构
![](stage1/Snipaste_2022-11-26_21-26-33.png)

#### 交换机端口连接配置
<table text-align="center">
	<tr>
		<td colspan="2">Switch1 Interfaces</td>
		<td colspan="2">Switch2 Interfaces</td>
	</tr>
	<tr>
		<td>From</td>
		<td>To</td>
		<td>From</td>
		<td>To</td>
	</tr>
	<tr>
		<td>FastEthernet0/1</td>
		<td>A1</td>
		<td>FastEthernet0/1</td>
		<td>B1</td>	
	</tr>
	<tr>
		<td>FastEthernet0/2</td>
		<td>A2</td>
		<td>FastEthernet0/2</td>
		<td>B2</td>	
	</tr>
	<tr>
		<td>FastEthernet0/3</td>
		<td>A3</td>
		<td>FastEthernet0/3</td>
		<td>B3</td>	
	</tr>
	<tr>
		<td>FastEthernet0/18</td>
		<td>Switch2 FastEthernet0/18</td>
		<td>FastEthernet0/18</td>
		<td>Switch1 FastEthernet0/18</td>	
	</tr>
</table>


#### 主机IP地址配置

<table align="center">
	<tr>
		<td>主机</td>
		<td>IP地址</td>
		<td>主机</td>
	</tr>
	<tr>
		<td>A1</td>
		<td>192.168.1.11</td>
		<td>255.255.255.0</td>
	</tr>
	<tr>
		<td>A2</td>
		<td>192.168.1.12</td>
		<td>255.255.255.0</td>
	</tr>
	<tr>
		<td>A3</td>
		<td>192.168.1.13</td>
		<td>255.255.255.0</td>
	</tr>
	<tr>
		<td>B1</td>
		<td>192.168.1.21</td>
		<td>255.255.255.0</td>
	</tr>
	<tr>
		<td>B2</td>
		<td>192.168.1.22</td>
		<td>255.255.255.0</td>
	</tr>
	<tr>
		<td>B3</td>
		<td>192.168.1.23</td>
		<td>255.255.255.0</td>
	</tr>
</table>

#### 网络状况展示(以A1为例，其余结果见stage1文件夹下)
![](stage1/Snipaste_2022-11-26_21-39-27.png)



### VLAN配置

#### 网络状况展示

![](stage2/Snipaste_2022-11-26_21-27-09.png)
![](stage2/Snipaste_2022-11-26_21-28-37.png)
![](stage2/Snipaste_2022-11-26_21-28-45.png)
![](stage2/Snipaste_2022-11-26_21-28-55.png)
![](stage2/Snipaste_2022-11-26_21-29-07.png)
![](stage2/Snipaste_2022-11-26_21-29-17.png)

#### Switch1
- `show vlan` 
  接口0/1,0/2被划分到tech vlan下, 接口0/3被划分到sales vlan下
  ![](stage2/Snipaste_2022-11-26_21-31-05.png)
- `show interfaces FastEthernet 0/1 switchport`
  接口0/1的Administrative Mode、Operational Mode均为static access, 被划分到vlan 2(tech)
  ![](stage2/Snipaste_2022-11-26_21-32-00.png)
- `show interfaces FastEthernet 0/2 switchport`
  接口0/2的Administrative Mode、Operational Mode均为static access, 被划分到vlan 2(tech)
  ![](stage2/Snipaste_2022-11-26_21-32-15.png)
- `show interfaces FastEthernet 0/3 switchport`
  接口0/3的Administrative Mode、Operational Mode均为static access, 被划分到vlan 3(sales)
  ![](stage2/Snipaste_2022-11-26_21-32-32.png)
- `show interfaces FastEthernet 0/18 switchport`
  接口0/18的Administrative Mode、Operational Mode均为trunk
  ![](stage2/Snipaste_2022-11-26_21-32-47.png)

- `show mac-address-table`
  接口Fa0/1,Fa0/2,Fa0/3分别连接到A1,A2,B3,Fa0/18连接到Switch2
  ![](stage2/Snipaste_2022-11-29_20-50-45.png)

#### Switch2
- `show vlan`
  接口0/1,0/2被划分到sales vlan下, 接口0/3被划分到tech vlan下
  ![](stage2/Snipaste_2022-11-26_22-13-17.png)
- `show interfaces FastEthernet 0/1 switchport`
  接口0/1的Administrative Mode、Operational Mode均为static access, 被划分到vlan 3(sales)
  ![](stage2/Snipaste_2022-11-26_21-33-46.png)
- `show interfaces FastEthernet 0/2 switchport`
  接口0/1的Administrative Mode、Operational Mode均为static access, 被划分到vlan 3(sales)
  ![](stage2/Snipaste_2022-11-26_21-34-00.png)
- `show interfaces FastEthernet 0/3 switchport`
  接口0/1的Administrative Mode、Operational Mode均为static access, 被划分到vlan 2(tech)
  ![](stage2/Snipaste_2022-11-26_21-34-12.png)
- `show interfaces FastEthernet 0/18 switchport`
  接口0/18的Administrative Mode、Operational Mode均为trunk
  ![](stage2/Snipaste_2022-11-26_21-34-26.png)
- `show mac-address-table`
  接口Fa0/1,Fa0/2,Fa0/3分别连接到B1,B2,A3,Fa0/18连接到Switch1
  ![](stage2/Snipaste_2022-11-29_20-51-20.png)


### DTP配置

#### 结果展示
- switch1 `show interfaces FastEthernet 0/18 switchport`
![](stage3/Snipaste_2022-11-26_22-20-21.png)
- switch2 `show interfaces FastEthernet 0/18 switchport`
![](stage3/Snipaste_2022-11-26_22-21-16.png)


### 问题

#### 问题1

正如前面`网络搭建`中的`网络状况展示`一样, 主机之间可以相互ping通, 所有主机仅通过交换机互联,交换机仅能隔离冲突域,无法隔离广播域,发送广播包或者直接向目标主机发包均能被交换机转发到目的地

#### 问题2
正如前面`VLAN配置`中的`网络状况展示`一样, A1,A2,A3可以相互ping通;B1,B2,B3可以相互ping通,因为经过vlan配置后,A1,A2,A3同属于vlan tech,B1,B2,B3同属于vlan sales,不属于同一vlan,由于各主机仅通过switch连通,无法在不同vlan间转发数据包

#### VLAN下的广播包处理过程
![](stage2/Snipaste_2022-12-01_20-15-24.png)
![](stage2/Snipaste_2022-12-01_20-16-08.png)
![](stage2/Snipaste_2022-12-01_20-16-17.png)

1. A1主机上运行`ping -n 1 255.255.255.255`后,发送ICMP报文到网络中,该报文的IP地址和链路层地址均为广播地址,该报文首先到达交换机switch1,交换机将其转发到和A1在同一VLAN中的A2和交换机switch2,switch2再将其转发到和A1同一VLAN下的A3;
2. A2接受到ICMP报文后,需要向源即A1发送ICMP报文回复,但A2不知道A1的链路层地址,于是发送ARP报文到网络中,经交换机转发到同一VLAN下除A2的各端口,当ARP报文到达A1,A1发现该报文是给自己的,就发送ARP报文给A2回复自己的链路层地址(该到达A3后,因为A3不是目的地,A3不回复),经交换机switch1转发给A2(此时switch1因此前报文知道A2的链路层地址,直接转发给A2)后,A2知道A1的MAC地址后可以将ICMP报文发送给A1;A3同理不再赘述

#### 802.1Q帧封装消息
![](stage2/Snipaste_2022-12-01_20-16-47.png)

1. `PREAMBLE、 SFD(8 字节)`: 该字段为前同步码，用于“唤醒”接受适配器，并且将他们的时钟和发送方的时钟同步. SFD 标志后面的内容为有效的内容
2. `DEST ADDR(6 字节`: 目的地址.该字段包含目的适配器的 MAC 地址.上图所示的目的地址是广播地址
3. `SRC ADDR(6 字节)`: 源地址. 该字段包含源适配器的 MAC 地址. 上图所示地址为 A2的 MAC 地址
4. `TPID(2 字节)`: 标签协议标识符.具有固定的值 0x8100
5. `TCI(2 字节)`: 标签控制信息
6. `TYPE(4 字节)`: 类型.告诉目的地址的适配器它应该将数据字段内容传递给哪个网络层协议
7. `DATA(46~1500 字节)`: 数据.这个字段承载了 IP 数据报
8. `FCS(4 字节)`: 帧校验序列. 包含帧的 32 位循环冗余校验 (CRC)

#### 表格1(Switch1:negotiate 模式 Switch2:negotiate 模式)

|	|	Switch2 trunk | Switch2 dynamic desirable |	Switch2 dynamic auto|
|:--: | :--: | :--: |:--:|
| Switch1 trunk | 	可以	|	可以	|	可以	|
| Switch1 dynamic desirable | 	可以	|	可以	|	可以	|
| Switch1 dynamic auto | 	可以	|	可以	|	不可以,static access	|


#### 表格2(Switch1:nonegotiate 模式 Switch2:negotiate 模式)

|	|	Switch2 trunk | Switch2 dynamic desirable |	Switch2 dynamic auto|
|:--: | :--: | :--: |:--:|
| Switch1 trunk | 	可以	|	可以	|	可以	|
| Switch2 dynamic desirable | 	不存在	|	不存在	| 不存在 |
| Switch2 dynamic auto | 	不存在	|	不存在	|	不存在	|















































