import argparse
import os

data_base_path = "/home/jack/workspace/algorithm-labs/lab1/5-柯志伟-PB20061338-project1/ex1/"
def generate_verify_result(radix):
    raw_datas = []
    count = 0
    nums = 1 << radix
    with open(data_base_path+"input/input.txt","r") as f:
        for i in f.readlines():
            count = count+1
            raw_datas.append(int(i))
            if(count == nums):
                break
    
    raw_datas.sort()

    with open(data_base_path+"input/verify"+str(radix)+".txt","w") as f:
        for i in raw_datas:
            f.write(str(i)+"\n")

def verify_sort_data(alg):
    algs = {
        "mergesort":"/home/jack/workspace/algorithm-labs/lab1/5-柯志伟-PB20061338-project1/ex1/output/mergesort/",
        "countingsort":"/home/jack/workspace/algorithm-labs/lab1/5-柯志伟-PB20061338-project1/ex1/output/countingsort/",
        "quicksort":"/home/jack/workspace/algorithm-labs/lab1/5-柯志伟-PB20061338-project1/ex1/output/quicksort/",
        "heapsort":"/home/jack/workspace/algorithm-labs/lab1/5-柯志伟-PB20061338-project1/ex1/output/heapsort/"
    }   
    verify_dir = "/home/jack/workspace/algorithm-labs/lab1/scripts/"
    for file in os.listdir(algs[alg]):
        try:
            index = file.split(".")[0].split('_')[1]
        except:
            continue
        f1 = open(algs[alg]+file,"r")
        f2 = open(verify_dir+"verify"+str(index)+".txt","r")
        for i in f2.readlines():
            j = f1.readline()
            if i.strip("\n") != j.strip("\n"):
                print(alg+" is wrong\n")
                print("The index is "+str(index))
                return
        print(alg+" is right\n")
        

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--mode",type=str,default="generate")
    parser.add_argument("--alg",type=str,default=None)
    args = parser.parse_args()
    if(args.mode == "generate"):
        generate_verify_result(3)
        generate_verify_result(6)
        generate_verify_result(9)
        generate_verify_result(12)
        generate_verify_result(15)
        generate_verify_result(18)
    elif(args.mode == "verify"):
        verify_sort_data(args.alg)


    
    