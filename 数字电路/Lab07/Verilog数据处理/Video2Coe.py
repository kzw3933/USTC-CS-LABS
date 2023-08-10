#将视频取帧并将每张图片的每个像素点的值转为0或1,存入coe文件

import cv2
import os
import argparse

def Video2Coe(Video_path,Coe_path):
    img_dir = "./raw_pictures/"
    Video2Png(Video_path)
    with open(Coe_path, "a") as f:
        f.write("memory_initialization_radix=16;\n")
        f.write("memory_initialization_vector=")
        f.close()
    imgs = os.listdir(img_dir)
    indices = []
    for img in imgs:
        i = img.split('.')[0]
        indices.append(i)
    indices.sort()
    for i in indices:
        img_path = img_dir+str(i)+'.jpg'
        Array2Coe(img_path,Coe_path)
    # img_path = img_dir + str(20) + '.jpg'
    # Array2Coe(img_path,Coe_path)
    with open(Coe_path, "a") as f:
        f.write(";")
        f.close()



def Video2Png(Video_path):

    VideoCapture = cv2.VideoCapture(Video_path)

    intervel = 6
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
        # if frame_count >=150:
        #     break
        # cv2.waitKey(1)

    VideoCapture.release()


def Array2Coe(img_path,Coe_path):
    img = cv2.imread(img_path)
    img = cv2.resize(img,(200,150))
    img_gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    ret,img_thresh = cv2.threshold(img_gray,0.5,1,cv2.THRESH_BINARY)
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
    parser.add_argument('-video_p', '--path1', help="path of video")
    parser.add_argument('-coe_p', '--path2', help="path of coe")
    args = parser.parse_args()
    Video2Coe(args.path1,args.path2)