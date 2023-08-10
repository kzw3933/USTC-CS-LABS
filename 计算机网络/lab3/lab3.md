# lab3实验报告

<center>
    <p>姓名:   柯志伟</p>
    <p>学号: PB20061338</p>
</center>


## 实验目的

- 掌握子网的划分和网络拓扑的构建

- 掌握静态路由、RIP和OSPF的配置


## 实验环境与器材

- 环境
  模拟软件Cisco Packet Tracer 5.2
  Windows

- 器材
  Cisco 2811路由器1台
  1841路由器4台
  2960-24TT交换机4台
  Access Point-PT 1台
  笔记本电脑1台，服务器1台， PC机3台
  Copper Cross-Over（铜芯双绞线）若干


## 实验要求

- 搭建场景：五个校区的网络及校区之间的互连

## 实验过程

### 构建网络拓扑

![](D:\Savefiles\Projects\WebNet\lab3\Snipaste_2022-11-09_08-11-24.png)

### 选定待分配的网段
- 学号为PB20061338,选择网段为13.38.0.0/20

### 子网划分方案



<table text-align="center">
    <tr>
        <td> Campus </td>
        <td colspan="2"> Subnet </td>
    </tr>
    <tr>
        <td> 北区 </td>
        <td colspan="2"> 13.38.0.0/24 </td>
    </tr>
    <tr>
        <td> 东区 </td>
        <td colspan="2"> 13.38.1.0/24 </td>
    </tr>
    <tr>
        <td> 南区 </td>
        <td colspan="2"> 13.38.2.0/24 </td>
    </tr>
    <tr>
        <td> 西区 </td>
        <td colspan="2"> 13.38.3.0/24 </td>
    </tr>
    <tr>
        <td rowspan="5"> 中区 </td>
        <td> 中区-北区路由 </td>
        <td> 13.38.4.0/24 </td>
    </tr>
    <tr>
        <td> 中区-东区路由 </td>
        <td> 13.38.5.0/24 </td>
    </tr>
    <tr>
        <td> 中区-南区路由 </td>
        <td> 13.38.6.0/24 </td>
    </tr>
    <tr>
        <td> 中区-西区路由 </td>
        <td> 13.38.7.0/24 </td>
    </tr>
    <tr>
        <td> 中区路由 </td>
        <td> 13.38.8.0/24 </td>
    </tr>
</table>



### 路由器接口连接及IP地址分配
<table text-align="center">
	<tr>
		<td>Device</td>
		<td>Interface</td>
		<td>IP Address</td>
		<td>Mask</td>
		<td>To Device</td>
		<td>To Interface</td>
	</tr>
	<tr>
		<td rowspan="5">RouterMiddle,Cisic 2811,NM-2FE2W,WIC-1ENET</td>
		<td>FastEthernet0/0</td>
		<td>13.38.4.1</td>
		<td rowspan="13">255.255.255.0</td>
		<td>RouterNorth</td>
		<td>FastEthernet0/0</td>
	</tr>
	<tr>
		<td>FastEthernet0/1</td>
		<td>13.38.5.1</td>
		<td>RouterEast</td>
		<td>FastEthernet0/0</td>
	</tr>
	<tr>
		<td>FastEthernet1/0</td>
		<td>13.38.6.1</td>
		<td>RouterSouth</td>
		<td>FastEthernet0/0</td>
	</tr>
	<tr>
		<td>FastEthernet1/1</td>
		<td>13.38.7.1</td>
		<td>RouterWest</td>
		<td>FastEthernet0/0</td>
	</tr>
	<tr>
		<td>Ethernet0/1/0</td>
		<td>13.38.8.1</td>
		<td>SwitchMiddle</td>
		<td></td>
	</tr>
	<tr>
		<td rowspan="2">RouterNorth,Cisic 1841</td>
		<td>FastEthernet0/0</td>
		<td>13.38.4.2</td>
		<td>RouterMiddle</td>
		<td>FastEthernet0/0</td>
	</tr>
	<tr>
		<td>FastEthernet0/1</td>
		<td>13.38.0.1</td>
		<td>SwitchNorth</td>
		<td></td>
	</tr>
	<tr>
		<td rowspan="2">RouterEast,Cisic 1841</td>
		<td>FastEthernet0/0</td>
		<td>13.38.5.2</td>
		<td>RouterMiddle</td>
		<td>FastEthernet0/1</td>
	</tr>
	<tr>
		<td>FastEthernet0/1</td>
		<td>13.38.1.1</td>
		<td>SwitchEast</td>
		<td></td>
	</tr>
	<tr>
		<td rowspan="2">RouterSouth,Cisic 1841</td>
		<td>FastEthernet0/0</td>
		<td>13.38.6.2</td>
		<td>RouterMiddle</td>
		<td>FastEthernet1/0</td>
	</tr>
	<tr>
		<td>FastEthernet0/1</td>
		<td>13.38.2.1</td>
		<td>SwitchSouth</td>
		<td></td>
	</tr>
	<tr>
		<td rowspan="2">RouterSouth,Cisic 1841</td>
		<td>FastEthernet0/0</td>
		<td>13.38.7.2</td>
		<td>RouterMiddle</td>
		<td>FastEthernet1/1</td>
	</tr>
	<tr>
		<td>FastEthernet0/1</td>
		<td>13.38.3.1</td>
		<td>SwitchWest</td>
		<td></td>
	</tr>
</table>



### 终端设备接口连接及IP地址

<table text-align="center">
	<tr>
		<td>Device</td>
		<td>IP address</td>
		<td>Mask</td>
		<td>Gateway</td>
	</tr>
	<tr>
		<td>PCNorth</td>
		<td>13.38.0.2</td>
		<td rowspan="6">255.255.255.0</td>
		<td>13.38.0.1</td>
	</tr>
	<tr>
		<td>ServerEast</td>
		<td>13.38.1.2</td>
		<td>13.38.1.1</td>
	</tr>
	<tr>
		<td>PCEast</td>
		<td>13.38.1.3</td>
		<td>13.38.1.1</td>
	</tr>
	<tr>
		<td>PCSouth</td>
		<td>13.38.2.2</td>
		<td>13.38.2.1</td>
	</tr>
	<tr>
		<td>LaptopWestLib,Linksys-WPC300N </td>
		<td>13.38.3.2</td>
		<td>13.38.3.1</td>
	</tr>
	<tr>
		<td>PCMiddle </td>
		<td>13.38.8.2</td>
		<td>13.38.8.1</td>
	</tr>
</table>


### 配置静态路由

#### 路由表

<table text-align="center">
	<tr>
		<td>Router</td>
		<td>Network</td>
		<td>Mask</td>
		<td>Next Hop</td>
	</tr>
	<tr>
		<td rowspan="4">RouterMiddle</td>
		<td>13.38.0.0</td>
		<td rowspan="4">255.255.255.0</td>
		<td>13.38.4.2</td>
	</tr>
	<tr>
		<td>13.38.1.0</td>
		<td>13.38.5.2</td>
	</tr>
	<tr>
		<td>13.38.2.0</td>
		<td>13.38.6.2</td>
	</tr>
	<tr>
		<td>13.38.3.0</td>
		<td>13.38.7.2</td>
	</tr>
</table>
<table text-align="center">
	<tr>
		<td>Router</td>
		<td>Network</td>
		<td>Mask</td>
		<td>Next Hop</td>
	</tr>
	<tr>
		<td rowspan="4">RouterNorth</td>
		<td>13.38.1.0</td>
		<td rowspan="4">255.255.255.0</td>
	</tr>
	<tr>
		<td>13.38.2.0</td>
		<td rowsapn="4">13.38.4.1</td>
	</tr>
	<tr>
		<td>13.38.3.0</td>
	</tr>
	<tr>
		<td>13.38.8.0</td>
	</tr>
</table>
<table text-align="center">
	<tr>
		<td>Router</td>
		<td>Network</td>
		<td>Mask</td>
		<td>Next Hop</td>
	</tr>
	<tr>
		<td rowspan="4">RouterEast</td>
		<td>13.38.0.0</td>
		<td rowspan="4">255.255.255.0</td>
	</tr>
	<tr>
		<td>13.38.2.0</td>
		<td rowsapn="4">13.38.5.1</td>
	</tr>
	<tr>
		<td>13.38.3.0</td>
	</tr>
	<tr>
		<td>13.38.8.0</td>
	</tr>
</table>
<table text-align="center">
	<tr>
		<td>Router</td>
		<td>Network</td>
		<td>Mask</td>
		<td>Next Hop</td>
	</tr>
​	<tr>
		<td rowspan="4">RouterSouth</td>
		<td>13.38.0.0</td>
		<td rowspan="4">255.255.255.0</td>
	</tr>
	<tr>
		<td>13.38.1.0</td>
		<td rowsapn="4">13.38.6.1</td>
	</tr>
	<tr>
		<td>13.38.3.0</td>
	</tr>
	<tr>
		<td>13.38.8.0</td>
	</tr>
</table>
<table text-align="center">
	<tr>
		<td>Router</td>
		<td>Network</td>
		<td>Mask</td>
		<td>Next Hop</td>
	</tr>
	<tr>
		<td rowspan="4">RouterWest</td>
		<td>13.38.0.0</td>
		<td rowspan="4">255.255.255.0</td>
	</tr>
	<tr>
		<td>13.38.1.0</td>
		<td rowsapn="4">13.38.7.1</td>
	</tr>
	<tr>
		<td>13.38.2.0</td>
	</tr>
	<tr>
		<td>13.38.8.0</td>
	</tr>
</table>

#### 路由配置
![](D:\Savefiles\Projects\WebNet\lab3\Snipaste_2022-11-09_08-13-48.png)
![](D:\Savefiles\Projects\WebNet\lab3\Snipaste_2022-11-09_08-14-13.png)
![](D:\Savefiles\Projects\WebNet\lab3\Snipaste_2022-11-09_08-14-28.png)
![](D:\Savefiles\Projects\WebNet\lab3\Snipaste_2022-11-09_08-14-45.png)
![](D:\Savefiles\Projects\WebNet\lab3\Snipaste_2022-11-09_08-15-00.png)

#### 效果展示(以北区为例，其余见文件夹static下)
![](D:\Savefiles\Projects\WebNet\lab3\static\Snipaste_2022-11-09_07-46-45.png)
![](D:\Savefiles\Projects\WebNet\lab3\static\Snipaste_2022-11-09_07-47-43.png)
![](D:\Savefiles\Projects\WebNet\lab3\static\Snipaste_2022-11-09_07-48-13.png)
![](D:\Savefiles\Projects\WebNet\lab3\static\Snipaste_2022-11-09_07-48-36.png)

### 配置动态路由(OSPF)

#### 路由配置
![](D:\Savefiles\Projects\WebNet\lab3\Snipaste_2022-11-09_08-17-17.png)
![](D:\Savefiles\Projects\WebNet\lab3\Snipaste_2022-11-09_08-17-54.png)
![](D:\Savefiles\Projects\WebNet\lab3\Snipaste_2022-11-09_08-18-18.png)
![](D:\Savefiles\Projects\WebNet\lab3\Snipaste_2022-11-09_08-18-47.png)
![](D:\Savefiles\Projects\WebNet\lab3\Snipaste_2022-11-09_08-19-10.png)

#### 效果展示(以北区为例，其余见文件夹ospf下)

![](D:\Savefiles\Projects\WebNet\lab3\ospf\Snipaste_2022-11-09_08-00-12.png)
![](D:\Savefiles\Projects\WebNet\lab3\ospf\Snipaste_2022-11-09_08-00-30.png)
![](D:\Savefiles\Projects\WebNet\lab3\ospf\Snipaste_2022-11-09_08-00-55.png)
![](D:\Savefiles\Projects\WebNet\lab3\ospf\Snipaste_2022-11-09_08-01-18.png)





