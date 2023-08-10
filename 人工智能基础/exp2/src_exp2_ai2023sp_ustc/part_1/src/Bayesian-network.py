import numpy as np
import pandas as pd

class BayesianNetwork:
    def __init__(self, n_labels=10, n_pixels=784, n_values=2) -> None:
        '''
        n_labels: number of labels, 10 for digit recognition
        n_pixels: number of pixels, 256 for 16x16 image
        n_values: number of values for each pixel, 0 for black, 1 for white
        '''
        self.n_labels = n_labels
        self.n_pixels = n_pixels
        self.n_values = n_values
        # prior probability
        self.labels_prior = np.zeros(n_labels)
        self.pixels_prior = np.zeros((n_pixels, n_values))
        # conditional probability
        self.pixels_cond_label = np.zeros((n_pixels, n_values, n_labels))
    

    # 利用训练数据获得先验概率和条件概率从而获得模型
    def fit(self, pixels, labels):
        '''
        pixels: (n_samples, n_pixels, )
        labels: (n_samples, )
        '''
        n_samples = len(labels)
        # 计算先验概率
        for i in range(self.n_labels):
            self.labels_prior[i] = np.sum(labels == i) / n_samples
            
        for i in range(self.n_pixels):
            for j in range(self.n_values):
                self.pixels_prior[i, j] = np.sum(pixels[:, i] == j) / n_samples
        
        # 计算条件概率
        for i in range(self.n_pixels):
            for j in range(self.n_values):
                for k in range(self.n_labels):
                    idx = np.where((pixels[:, i] == j) & (labels == k))[0]
                    self.pixels_cond_label[i, j, k] = len(idx) / np.sum(labels == k)
        
        
    

    # predict the labels for new data
    def predict(self, pixels):
        '''
        pixels: (n_samples, n_pixels, )
        return labels: (n_samples, )
        '''
        n_samples = len(pixels)
        labels = np.zeros(n_samples)
        for i in range(n_samples):
            # 使用贝叶斯公式计算后验概率，这里使用先使用log将乘除化为加减最后再通过exp还原
            post_props = np.zeros(self.n_labels)
            for j in range(self.n_labels):
                log_poster_props = np.log(self.labels_prior[j])
                for k in range(self.n_pixels):
                    log_poster_props += np.log(self.pixels_cond_label[k, pixels[i, k], j]) - np.log(self.pixels_prior[k, pixels[i, k]])
                post_props[j] = np.exp(log_poster_props)
            # 选择概率最大的标签
            labels[i] = np.argmax(post_props)

        return labels
    

    # calculate the score (accuracy) of the model
    def score(self, pixels, labels):
        '''
        pixels: (n_samples, n_pixels, )
        labels: (n_samples, )
        '''
        n_samples = len(labels)
        labels_pred = self.predict(pixels)
        return np.sum(labels_pred == labels) / n_samples


if __name__ == '__main__':
    # load data
    train_data = np.loadtxt('../data/train.csv', delimiter=',', dtype=np.uint8)
    test_data = np.loadtxt('../data/test.csv', delimiter=',', dtype=np.uint8)
    pixels_train, labels_train = train_data[:, :-1], train_data[:, -1]
    pixels_test, labels_test = test_data[:, :-1], test_data[:, -1]
    # build bayesian network
    bn = BayesianNetwork()
    bn.fit(pixels_train, labels_train)
    print('test score: %f' % bn.score(pixels_test, labels_test))