import argparse

def lab1_l():
    R0 = int(input("请输入被乘数(整数,范围:-32768~32767)：\n"))
    R1 = int(input("请输入乘数(整数,范围:-32768~32767)：\n"))
    if (R1 < 0):
        R1 = -R1
        R0 = -R0
    R7 = 0
    while (R1 > 0):
        R7 += R0
        R1 -= 1
    print("结果为{0}\n".format(R7))

def lab1_p():
    R0 = int(input("请输入被乘数(整数,范围:-32768~32767)：\n"))
    R1 = int(input("请输入乘数(整数,范围:-32768~32767)：\n"))
    R6 = 0                    # 符号位,0代表为正,1代表为负
    if(R1>0):
        if(R0<0):
            R6 = 1
            R0 = -R0
    else:
        R1 = -R1
        if(R0>0):
            R6 = 1
        else:
            R0 = -R0
    if(R0<R1):
        R0,R1 = R1,R0
    R5 = 16 * R0
    R4 = R1
    R7 = 0
    while(True):
        R4 -= 16
        if(R4 < 0 ):
            break
        R7+=R5
        R1-=16
    while(R1>0):
        R7+=R0
        R1-=1
    if(R6):
        R7 = -R7
    print("结果为{0}\n".format(R7))

def lab2():
    R7 = 1
    R1 = 1
    R2 = 1
    R3 = 2
    R4 = 1023
    R0 = int(input("请输入代求数(整数,范围:1~16384)：\n"))
    R0 -= 1;
    if(R0==0):
        print("结果为{0}\n".format(R7))
    while(True):
        R7 = R3
        R0 -=1
        if(R0 == 0):
            print("结果为{0}\n".format(R7))
            break
        else:
            R3 += R1
            R3 += R1
            R1 = R2
            R2 = R7
            R3 = R3&R4

def lab3():
    R0 = int(input("请输入代求数(整数,范围:1~16384)：\n"))
    list = []
    list.append(1)
    list.append(1)
    list.append(2)
    for i in range(3,16385):
        x = (list[i-1]+2*list[i-3])%1024
        list.append(x)
    R7 = list[R0]
    print("结果为{0}\n".format(R7))



def lab4_rec():
    mem = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5] #前十五个placeholder代表代码
    R2 = 15          # mem[15]的位置
    R0,R1,R3,R4,R5,R6 = 0,0,0,0,0,0
    def f():
        nonlocal R7,R0,R1,R2,mem
        mem[R2] = R7
        R2 += 1
        R0 += 1
        R1 = mem[-1]
        R1 -= 1
        mem[-1] = R1

        if (R1 != 0):
            R7 = 12
            f()
        R2 -= 1
        R7 = mem[R2]
    R7 = 3
    f()
    print("R0:{0},R1:{1},R2:{2},R3:{3},R4:{4},R5:{5},R6:{6},R7:{7}".format(R0,R1,R2,R3,R4,R5,R6,R7))

def lab4_mod():
    target = int(input("请输入参数：\n"))
    R0, R1, R2,R3, R4, R5, R6,R7 = 0, 0, 0, 0, 0, 0,0,0
    R1 = target
    def f():
        nonlocal R1,R2,R3,R4,R5
        R2,R3,R4 = 0,0,0
        R2 += 1
        R3 += 8
        while(True):
            R5 = R3&R1
            if(R5!=0):
                R4 += R2
            R2 +=R2
            R3 +=R3
            if(R3>=0b1000000000000000):
                break
    while(True):
        f()
        R2 = R1 & (0b111)
        R1 = R2 + R4
        R0 = R1 - 7
        if (R0 <= 0):
            break
    R0 = R1 -7
    if(R0>=0):
        R1 = R1-7
    print("结果为{0}".format(R1))


def lab5():
    R0 = int(input("请输入参数：\n"))
    def Judge():
       R6 = 2
       R1 = 1
       while(True):
           R4 = 0
           R5 = R6
           while(R5 > 0):
               R4 += R6
               R5 -= 1
           R4 = -R4
           R4 += R0
           if(R4 <0):
               return R1
           else:
               R4 = -R6
               R3 = R0
               while(R3 >0):
                   R3 += R4
               if(R3 == 0):
                   R1 = 0
                   return R1
               R6 += 1
    R1 = Judge()
    print("结果为{0}\n".format(R1))


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--lab',type = str , help = 'choose from 1_l,1_p,2,3,4_rec,4_mod,5')
    args = parser.parse_args()
    if(args.lab == "1_l"):
        while(True):
            lab1_l()
    elif(args.lab == "1_p"):
        while (True):
            lab1_p()
    elif (args.lab == "2"):
        while (True):
            lab2()
    elif (args.lab == "3"):
        while (True):
            lab3()
    elif (args.lab == "4_rec"):
        while (True):
            lab4_rec()
    elif (args.lab == "4_mod"):
        while (True):
            lab4_mod()
    elif (args.lab == "5"):
        while (True):
            lab5()


















