#将视频取帧并将每张图片的每个像素点的R,G,B转到0-3,组成6位二进制的低6位，高位补零后存其对应的ASCII码
import cv2
from icecream.icecream import  ic
import os
import numpy as np
import argparse
# 从视频取帧
def Video2Png(Video_path):

    VideoCapture = cv2.VideoCapture(Video_path)

    intervel = 3
    count = 0
    frame_count = 0

    if VideoCapture.isOpened():
        ret,frame = VideoCapture.read()
    else:
        ret = 0

    while ret:
        ret,frame = VideoCapture.read()
        img_path = './raw_pictures/{}.jpg'.format(str(frame_count))
        count+=1;
        if(count%intervel == 0) :
            cv2.imwrite(img_path,frame)
            frame_count +=1;
        cv2.waitKey(1)

    VideoCapture.release()

 #图像RGB值压缩编码为ASCII码
def Png2ascII(img_path,asc_path):
    img = cv2.imread(img_path)
    img = cv2.resize(img, (200, 150))
    img_B,img_G, img_R  = cv2.split(img)
    img_B = np.floor(img_B/64)*1
    img_G = np.floor(img_G/64)*4
    img_R = np.floor(img_R/64)*16
    matrix = img_B+img_R+img_G
    array = []
    for i in matrix:
        for j in i:
            array.append(int(j))
    content = ''.join([chr(i) for i in array])
    with open(asc_path,"a") as f:
        f.write(content)

def Video2ascII(Video_p,Txt_p):
    img_dir = "./raw_pictures/"
    Video2Png(Video_p)
    imgs = os.listdir(img_dir)
    indices = []
    for img in imgs:
        i = img.split('.')[0]
        indices.append(int(i))
    indices.sort()
    for i in indices:
        img_path = img_dir + str(i) + '.jpg'
        Png2ascII(img_path, Txt_p)



if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-video_p', '--path1', help="path of video")
    parser.add_argument('-asc_p', '--path2', help="path of asc_txt")
    args = parser.parse_args()
    Video2ascII(args.path1, args.path2)