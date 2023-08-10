from PIL import Image
import numpy as np
import argparse
from icecream.icecream import ic

import cv2
import os

def dec2hex(string_num):
    base = [str(x) for x in range(10)] + [chr(x) for x in range(ord('A'), ord('A') + 6)]
    num = int(string_num)
    mid1= []
    _,rem=divmod(num,16)
    mid1.append(base[rem])
    return ''.join([str(x) for x in mid1[::-1]])


def pix2hex(pixs):
    mid2 = []
    for pix in pixs:
        mid2.append(dec2hex(pix))
    return ''.join([str(x) for x in mid2])


def Png2Coe(img_path,coe_path):
    img = Image.open(img_path)
    img = img.resize((200, 150), Image.ANTIALIAS)
    img_matrix = np.asarray(img)
    ic(img_matrix)
    img_array = np.reshape(img_matrix, (-1, 3))
    img_array = np.floor(img_array / 16)
    mid3 = []
    for array in img_array:
        mid3.append(pix2hex(array))
    content = ' '.join([str(i) for i in mid3])
    with open(coe_path, "w") as f:
        f.write("memory_initialization_radix=16;\n")
        f.write("memory_initialization_vector=")
        f.write(content)
        f.write(";")
        f.close()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-img_p', '--path1', help="path of video")
    parser.add_argument('-coe_p', '--path2', help="path of asc_txt")
    args = parser.parse_args()
    Png2Coe(args.path1,args.path2)



#将视频取帧并将每张图片的每个像素点的值转为0或1,存入coe文件




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














