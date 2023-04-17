## Lab04 Reveal Yourself

[实验目的] 
- task1  

  根据给定的LC3机器码程序猜测其功能，从而补充其中丢失的4个bit

- task2 

  补充所给的LC3机器码程序中丢失的15个bit，使其能完成模7的功能
  

[实验过程]

task1:

- 根据机器码写出对应的汇编码
  <img src="D:\Code\ICS\lab4\Snipaste_2021-12-23_21-45-21.png" style="zoom:60%;" />
- 用C程序模拟其行为观察其功能
  <img src="D:\Code\ICS\lab4\Snipaste_2021-12-24_10-53-48.png" style="zoom:60%;" />
  发现其实它的功能就是把value中存的值最后放入R0

- 补充完整后如下
<img src="D:\Code\ICS\lab4\Snipaste_2021-12-24_10-53-38.png" style="zoom:60%;" />

task2:

> 思路：
>
> 求一个数模7的余数，用小学知识可以定义为一直减7直到减不动为止，但这样太慢，更快的方法是采用这样一个知识
>
> l = 8*m+n (0<=n<8)  
>
> ​	l mod 7 = (m+n) mod 7
>
> 观机器码，发现其正采用此方法，可根据给定的信息，补充完代码

- 根据机器码写出对应的汇编码
<img src="D:\Code\ICS\lab4\Snipaste_2021-12-24_19-01-08.png" style="zoom:60%;" />
- 用C程序模拟其行为观察其功能
<img src="D:\Code\ICS\lab4\Snipaste_2021-12-24_16-12-12.png" style="zoom:60%;" />
- 补充完整后如下
<img src="D:\Code\ICS\lab4\Snipaste_2021-12-24_19-02-15.png" style="zoom:60%;" />

[实验总结]

​				这次实验主要锻炼了读机器码，理解机器码实现功能的能力，同时对理解高级语言中函数，指针的具体实现以及它们与地址之间的联系有了更深的理解，同时锻炼了对机器代码功能的分析，每一步具体实现怎样的功能，理解它们与高级语言相比的不同之处。