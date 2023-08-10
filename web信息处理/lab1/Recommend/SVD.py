# -*- coding = utf-8 -*-
# @Time : 2022/11/6 14:47
import csv
import random
import numpy as np
from IR.Recommend.config import *

def DCG(rel_list):

    dcg = 0
    for i,rel in enumerate(rel_list):
        dcg += (2**int(rel)-1)/np.log2(i+2)
    return dcg

class SVD:
    def __init__(self, data, K=20):
        self.data = np.array(data)
        self.K = K
        self.base_m = {}
        self.base_u = {}
        self.degree_m = {}
        self.favor_u = {}
        self.avg = np.mean(self.data[:,2])
        for i in range(self.data.shape[0]):
            user_id = self.data[i, 0]
            movie_id = self.data[i, 1]
            self.base_m.setdefault(movie_id, 0)
            self.base_u.setdefault(user_id, 0)
            self.degree_m.setdefault(movie_id, np.random.random((self.K, 1)) / 10*np.sqrt(self.K))
            self.favor_u.setdefault(user_id, np.random.random((self.K, 1)) / 10*np.sqrt(self.K))

    def predict(self, user_id, movie_id):
        self.base_u.setdefault(user_id, 0)
        self.base_m.setdefault(movie_id, 0)
        self.degree_m.setdefault(movie_id, np.zeros((self.K, 1)))
        self.favor_u.setdefault(user_id, np.zeros((self.K, 1)))
        rating = self.avg + self.base_m[movie_id] + self.base_u[user_id] + np.sum(self.degree_m[movie_id]*self.favor_u[user_id])
        if rating > 5:
            rating = 5
        if rating < 1:
            rating = 1
        return rating

    def train(self, steps=82, gamma=0.001, Lambda=0.01):
        print('train data size', self.data.shape)
        for step in range(steps):
            print('step', step+1, 'is running')
            KK = np.random.permutation(self.data.shape[0])
            rmse = 0.0
            mae = 0
            for i in range(self.data.shape[0]):
                j = KK[i]
                user_id = self.data[j, 0]
                movie_id = self.data[j, 1]
                rating = self.data[j, 2]
                eui = rating - self.predict(user_id, movie_id)
                rmse += eui ** 2
                mae += abs(eui)
                self.base_u[user_id] += gamma * (eui - Lambda * self.base_u[user_id])
                self.base_m[movie_id] += gamma * (eui - Lambda * self.base_m[movie_id])
                tmp = self.degree_m [movie_id]
                self.degree_m[movie_id] += gamma * (eui * self.favor_u[user_id] - Lambda * self.degree_m[movie_id])
                self.favor_u[user_id] += gamma * (eui * tmp - Lambda * self.favor_u[user_id])
            print('rmse is {0:3f}, ase is {1:3f}'.format(np.sqrt(rmse/self.data.shape[0]), mae/self.data.shape[0]))

    def test(self, test_data):
        test_data = np.array(test_data)
        print('test data size', test_data.shape)
        rmse = 0.0
        mae =0
        re = {}
        for i in range(test_data.shape[0]):
            user_id = test_data[i, 0]
            movie_id = test_data[i, 1]
            rating = test_data[i, 2]
            re.setdefault(str(user_id), {})
            re[str(user_id)][self.predict(user_id, movie_id)] = rating
            eui = rating - self.predict(user_id, movie_id)
            rmse += eui ** 2
            mae += abs(eui)
        print('rmse is {0:3f}, ase is {1:3f}'.format(np.sqrt(rmse/self.data.shape[0]), mae/self.data.shape[0]))
        sum = 0
        counter =0
        for item in re:
            dic = re[item]
            score_list = list(dic.values())
            ideal_list = sorted(score_list, key=lambda x: int(x))[::-1]
            idcg = DCG(ideal_list)
            predict_dcg = DCG(score_list)
            if idcg !=0 :
                #if len(score_list) >= 5:
                    sum = sum+predict_dcg/idcg
                    counter = counter + 1
        print('end......')
        print(sum/counter)

def  getdata(file_name):
    data = []
    csvFile = open(file_name)
    reader = csv.reader(csvFile)
    flag = 0;
    for row in reader:
        if flag == 1:
            data.append([int(float(i.strip())) for i in row[:3]])
            flag = 0
        else :
            flag = 1
    random.shuffle(data)
    train_data = data[:int(len(data)*7/10)]
    test_data = data[int(len(data)*7/10):]
    print('load data finished')
    print('total data ', len(data))
    return train_data,test_data

if __name__=='__main__':
    train_data,test_data = getdata(movie_score_path)
    print(train_data[0])
    print(train_data[1])
    s = SVD(train_data, 180)
    s.train()
    s.test(test_data)

