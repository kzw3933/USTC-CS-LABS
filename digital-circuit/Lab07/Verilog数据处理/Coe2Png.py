#将视频取帧并将每张图片的每个像素点的值转为0或1,存入coe文件



import cv2
import os
import argparse
from icecream.icecream import  ic

def Png2Coe(img_path,Coe_path):
    with open(Coe_path, "a") as f:
        f.write("memory_initialization_radix=16;\n")
        f.write("memory_initialization_vector=")
        f.close()
    Array2Coe(img_path,Coe_path)
    with open(Coe_path, "a") as f:
        f.write(";")
        f.close()





def Array2Coe(img_path,Coe_path):
    img = cv2.imread(img_path)
    img = cv2.resize(img,(200,150))
    img_gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    # for i in img_gray:
    #     for j in i:
    #         ic(j)
    # cv2.imshow("im",img_gray)
    # cv2.imwrite("./1.jpg",img_gray)
    ret,img_thresh = cv2.threshold(img_gray,200,1,cv2.THRESH_BINARY)
    # for i in img_thresh:
    #     ic(i)
    mid = []
    for array in img_thresh:
        for i in array:
            mid.append(i)
    with open(Coe_path,"a") as f:
        content = ' '.join([str(i) for i in mid])
        f.write(content)
        f.write(' ')
        f.close()

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-img_p', '--path1', help="path of picture")
    parser.add_argument('-coe_p', '--path2', help="path of coe")
    args = parser.parse_args()
    Png2Coe(args.path1,args.path2)