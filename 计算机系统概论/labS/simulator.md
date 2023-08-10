<div style="text-align:center;font-size:2em;font-weight:bold">中国科学技术大学计算机学院</div>

<div style="text-align:center;font-size:2em;font-weight:bold">《ICS实验报告》</div>


<img src="logo.jpg" style="zoom: 50%;" />





<div style="display: flex;flex-direction: column;align-items: center;font-size:2em">
<div>
<p>实验题目：LC3模拟器</p>
<p>学生姓名：柯志伟</p>
<p>学生学号：PB20061338</p>
<p>完成时间：2022年1月17日</p>
</div>
</div>

<div style="page-break-after:always"></div>


## 实验题目

>   LC3模拟器


## 实验目的
>   用高级语言(C++)实现对LC3机器码执行效果的模拟，熟悉计算机执行机器码的流程

## 实验环境
>   Visual Studio 2022


## 实验过程

计算机执行LC3机器码的流程：从指定的内存开始处(程序开始位置)，逐一取指令，由于其内部的电路逻辑实现对应的功能(ISA所定义的功能)，用高级语言实现时，只需将翻译出每行指令对应的功能并用高级语言直接实现即可

通读助教给的代码，整体框架已完备，程序接受两个文件的输入，一个是作为寄存器开始状态，另一个是内存中的数据(含代码)，在接受两个文件输入完成内存，寄存器的初始化后，即开始main函数的逻辑：逐指令读取并"翻译"、执行，在补充完整具体的功能模块(如VM_STR、VM_LEA、VM_RTI等)后，即完成

部分补全的代码如下：
- main函数
```c++
int main(int argc, char **argv) {
    po::options_description desc{"\e[1mLC3 SIMULATOR\e[0m\n\n\e[1mOptions\e[0m"};
    desc.add_options()                                                                             //
        ("help,h", "Help screen")                                                                  //
        ("file,f", po::value<std::string>()->default_value("input.txt"), "Input file")             //
        ("register,r", po::value<std::string>()->default_value("register.txt"), "Register Status") //
        ("single,s", "Single Step Mode")                                                           //
        ("begin,b", po::value<int>()->default_value(0x3000), "Begin address (0x3000)")
        ("output,o", po::value<std::string>()->default_value(""), "Output file")
        ("detail,d", "Detailed Mode");

    po::variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }
    if (vm.count("file")) {
        gInputFileName = vm["file"].as<std::string>();
    }
    if (vm.count("register")) {
        gRegisterStatusFileName = vm["register"].as<std::string>();
    }
    if (vm.count("single")) {
        gIsSingleStepMode = true;
    }
    if (vm.count("begin")) {
        gBeginningAddress = vm["begin"].as<int>();
    }
    if (vm.count("output")) {
        gOutputFileName = vm["output"].as<std::string>();
    }
    if (vm.count("detail")) {
        gIsDetailedMode = true;
    }

    virtual_machine_tp virtual_machine(gBeginningAddress,gInputFileName,gRegisterStatusFileName);
    int halt_flag = true;
    int time_flag = 0;
    while(halt_flag) {
        // Single step
        // TO BE DONE
        int16_t next_addr = virtual_machine.NextStep();
        ++time_flag;
        if (next_addr == 0) 
            halt_flag = false;
       
        if (gIsDetailedMode)
            std::cout << virtual_machine.reg << std::endl;

        if (gIsSingleStepMode)
            system("pause"); 
       
    }

    std::cout << virtual_machine.reg << std::endl;
    std::cout << "cycle = " << time_flag << std::endl;
    return 0;
}


```
- 寄存器初始化
```c++

virtual_machine_tp::virtual_machine_tp(const int16_t address, const std::string &memfile, const std::string &regfile) {
    // Read memory
    if (memfile != ""){
        mem.ReadMemoryFromFile(memfile);
    }
    
    // Read registers
    std::ifstream input_file;
    input_file.open(regfile);
    if (input_file.is_open()) {
        int line_count = std::count(std::istreambuf_iterator<char>(input_file), std::istreambuf_iterator<char>(), '\n');
        input_file.close();
        input_file.open(regfile);
        if (line_count >= 8) {
            for (int index = R_R0; index <= R_R7; ++index) {
                input_file >> reg[index];
            }
        } else {
            for (int index = R_R0; index <= R_R7; ++index) {
                reg[index] = 0;
            }
        }
        input_file.close();
    } else {
        for (int index = R_R0; index <= R_R7; ++index) {
            reg[index] = 0;
        }
    }

    // Set address
    reg[R_PC] = address;
    reg[R_COND] = 0;
}


```


- 内存初始化
```c++
void memory_tp::ReadMemoryFromFile(std::string filename, int beginning_address) {
        // Read from the file
        // TO BE DONE
        std::ifstream input_file;
        std::string str;
        input_file.open(filename);
           if (input_file.is_open()) {
            int line_count = std::count(std::istreambuf_iterator<char>(input_file), std::istreambuf_iterator<char>(), '\n');
            input_file.close();
            input_file.open(filename);
            for (int index = 0; index < line_count; ++index) {
                input_file >> str;
                memory[beginning_address + index] = 0;
                for (int i = 0; i < str.size(); i++)
                    memory[beginning_address + index] += (str[i]-'0') * pow(2, str.size()-i-1);
            }
            input_file.close();
        }
        else {
            ;//PASS
        }
    }

    int16_t memory_tp::GetContent(int address) const {
        // get the content
        // TO BE DONE
        return memory[address];
    }

    int16_t& memory_tp::operator[](int address) {
        // get the content
        // TO BE DONE
        return memory[address];
    }    

```

- 具体功能模块
```c++

namespace virtual_machine_nsp {
template <typename T, unsigned B>
inline T SignExtend(const T x) {
    // Extend the number
    // TO BE DONE
    T signal = x & (1 << (B - 1));
    T y = x;
    if (signal) {
        for (int i = 0; B + i < sizeof(T) * 8; i++) {
            y = y + (1 << (B + i));
        }
    }
    return y;

}

void virtual_machine_tp::UpdateCondRegister(int regname) {
    // Update the condition register
    // TO BE DONE
    if (reg[regname] > 0) reg[R_COND] = 0x0001;
    else if(reg[regname] < 0) reg[R_COND] = 0x0004;
    else reg[R_COND] = 0x0002;
}

void virtual_machine_tp::VM_ADD(int16_t inst) {
    int flag = inst & 0b100000;
    int dr = (inst >> 9) & 0x7;
    int sr1 = (inst >> 6) & 0x7;
    if (flag) {
        // add inst number
        int16_t imm = SignExtend<int16_t, 5>(inst & 0b11111);
        reg[dr] = reg[sr1] + imm;
    } else {
        // add register
        int sr2 = inst & 0x7;
        reg[dr] = reg[sr1] + reg[sr2];
    }
    // Update condition register
    UpdateCondRegister(dr);
}

void virtual_machine_tp::VM_AND(int16_t inst) {
    // TO BE DONE
    int flag = inst & 0b100000;
    int dr = (inst >> 9) & 0x7;
    int sr1 = (inst >> 6) & 0x7;
    if (flag) {
        // and inst number
        int16_t imm = SignExtend<int16_t, 5>(inst & 0b11111);
        reg[dr] = reg[sr1] & imm;
    }
    else {
        // and register
        int sr2 = inst & 0x7;
        reg[dr] = reg[sr1] & reg[sr2];
    }
    // Update condition register
    UpdateCondRegister(dr);
}

void virtual_machine_tp::VM_BR(int16_t inst) {
    int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
    int16_t cond_flag = (inst >> 9) & 0x7;
    if (gIsDetailedMode) {
        std::cout << reg[R_PC] << std::endl;
        std::cout << pc_offset << std::endl;
    }
    if (cond_flag & reg[R_COND]) {
        reg[R_PC] += pc_offset;
    }
}

void virtual_machine_tp::VM_JMP(int16_t inst) {
    // TO BE DONE
    int BaseR = (inst >> 6) & 0x7;
    reg[R_PC] = reg[BaseR];
}

void virtual_machine_tp::VM_JSR(int16_t inst) {
    // TO BE DONE
    reg[R_R7] = reg[R_PC];
    int flag = inst & 0b100000000000;
    if (flag) {
        int16_t pc_offset = SignExtend<int16_t, 11>(inst & 0x7FF);
        reg[R_PC] += pc_offset;
    }
    else {
        int BaseR = (inst >> 6) & 0x7;
        reg[R_PC] = reg[BaseR];
    }
}

void virtual_machine_tp::VM_LD(int16_t inst) {
    int16_t dr = (inst >> 9) & 0x7;
    int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
    reg[dr] = mem[reg[R_PC] + pc_offset];
    UpdateCondRegister(dr);
}

void virtual_machine_tp::VM_LDI(int16_t inst) {
    // TO BE DONE
    int16_t dr = (inst >> 9) & 0x7;
    int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
    reg[dr] = mem[mem[reg[R_PC] + pc_offset]];
    UpdateCondRegister(dr);
}

void virtual_machine_tp::VM_LDR(int16_t inst) {
    // TO BE DONE
    int16_t dr = (inst >> 9) & 0x7;
    int16_t BaseR = (inst >> 6) & 0x7;
    int16_t pc_offset = SignExtend<int16_t, 6>(inst & 0x3F);
    reg[dr] = mem[reg[BaseR] + pc_offset];
    UpdateCondRegister(dr);
}

void virtual_machine_tp::VM_LEA(int16_t inst) {
    // TO BE DONE
    int16_t dr = (inst >> 9) & 0x7;
    int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
    reg[dr] = reg[R_PC] + pc_offset;
    UpdateCondRegister(dr);
}

void virtual_machine_tp::VM_NOT(int16_t inst) {
    // TO BE DONE
    int16_t dr = (inst >> 9) & 0x7;
    int16_t sr = (inst >> 6) & 0x7;
    reg[dr] = ~reg[sr];
    UpdateCondRegister(dr);
}

void virtual_machine_tp::VM_RTI(int16_t inst) {
    ; // PASS
}

void virtual_machine_tp::VM_ST(int16_t inst) {
    // TO BE DONE
    int16_t sr = (inst >> 9) & 0x7;
    int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
    mem[reg[R_PC] + pc_offset] = reg[sr];
}

void virtual_machine_tp::VM_STI(int16_t inst) {
    // TO BE DONE
    int16_t sr = (inst >> 9) & 0x7;
    int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
    mem[mem[reg[R_PC] + pc_offset]] = reg[sr];
}

void virtual_machine_tp::VM_STR(int16_t inst) {
    // TO BE DONE
    int16_t sr = (inst >> 9) & 0x7;
    int16_t BaseR = (inst >> 6) & 0x7;
    int16_t pc_offset = SignExtend<int16_t, 6>(inst & 0x3F);
    mem[reg[BaseR] + pc_offset] = reg[sr];
}

void virtual_machine_tp::VM_TRAP(int16_t inst) {
    int trapnum = inst & 0xFF;
    if (trapnum == 0x25)
        ;
    // TODO: build trap program
    else if (trapnum == 0x20){
        char ch = getchar();
        int16_t num = 0x00FF & ch;
        reg[R_R0] = num;

    }                  
    else if (trapnum == 0x21)
    {
        char ch = reg[R_R0] & 0xFF;
        putchar(ch);
    }                 
    else if (trapnum == 0x22)
    {
        char ch;
        int16_t addr = reg[R_R0];
        for (; ch = mem[addr] & 0xFF; addr++) {
            putchar(ch);
        }
    }                  
    else if (trapnum == 0x23)
    {
        printf(">> ");
        char ch = getchar();
        int16_t num = 0x00FF & ch;
        reg[R_R0] = num;
        putchar(ch);
    }
    else if (trapnum == 0x24)
    {
        char ch;
        int16_t addr = reg[R_R0];
        bool odd = true;
        int16_t num,num_tmp ;
        while (ch = getchar()) {
            if (odd) {
                num = ch & 0xFF;
                odd = false;
            }
            else {
                num_tmp = ch & 0xFF;
                num = num + num << 8;
                odd = true;
            }
            if (odd) {
                mem[addr] = num;
                addr++;
            }
        }
        if (!odd) {
            mem[addr] = num;
            addr++;
        }
        mem[addr] = 0x0000;

    }
}



```
## 总结与思考
​			通过本次实验，更清楚计算机执行机器码的流程，虽然代码主要框架由助教所写(这里感谢助教为我们减轻了难度)，但补全代码然有很多收获；另一方面，通过这次实验算是第一次用C++写代码，读代码(之前没学过，看来寒假要认真地完整学一下了)，万事开头难，其实在课下也零零散散看了一些C++的语法，但始终没实战过，这次实验算是开了个头，感觉收获还是很大的，这里，再次感谢学长这一学期的帮助。