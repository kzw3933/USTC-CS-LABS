import numpy as np
import matplotlib.pyplot as plt
import cv2
import threading


def read_image(filepath='./data/ustc-cow.png'):
    img = cv2.imread(filepath)  # Replace with the actual path to your image
    # Convert the image from BGR to RGB
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    return img


class KMeans:
    def __init__(self, k=4, max_iter=10):
        self.k = k
        self.max_iter = max_iter

    # Randomly initialize the centers
    def initialize_centers(self, points):
        '''
        points: (n_samples, n_dims,)
        '''
        n, d = points.shape

        centers = np.zeros((self.k, d))
        for k in range(self.k):
            # use more random points to initialize centers, make kmeans more stable
            random_index = np.random.choice(n, size=10, replace=False)
            centers[k] = points[random_index].mean(axis=0)

        return centers

    # Assign each point to the closest center
    def assign_points(self, centers, points):
        '''
        centers: (n_clusters, n_dims,)
        points: (n_samples, n_dims,)
        return labels: (n_samples, )
        '''
        n_samples, n_dims = points.shape
        labels = np.zeros(n_samples)
        # Compute the distance between each point and each center
        # and Assign each point to the closest center
        for i in range(n_samples):
            distances = np.linalg.norm(points[i] - centers, axis=1)
            labels[i] = np.argmin(distances)

        return labels

    # Update the centers based on the new assignment of points
    def update_centers(self, centers, labels, points):
        '''
        centers: (n_clusters, n_dims,)
        labels: (n_samples, )
        points: (n_samples, n_dims,)
        return centers: (n_clusters, n_dims,)
        '''
        for k in range(self.k):
            # 获得该中心点对应的所有样本点
            idx = np.where(labels == k)[0]
            if len(idx) > 0:
                # 更新中心点的坐标
                centers[k] = points[idx].mean(axis=0)

        return centers

    # k-means clustering
    def fit(self, points):
        '''
        points: (n_samples, n_dims,)
        return centers: (n_clusters, n_dims,)
        '''
        # 初始化k个中心
        centers = self.initialize_centers(points)

        # 迭代更新k个中心点的坐标
        for i in range(self.max_iter):
            # 将每个样本点分配到最近的中心
            labels = self.assign_points(centers, points)
            # 更新中心点的坐标
            centers_new = self.update_centers(centers, labels, points)
            # 如果足够好可以终止迭代
            if np.allclose(centers, centers_new):
                break
            centers = centers_new

        return centers

    def compress(self, img):
        '''
        img: (width, height, 3)
        return compressed img: (width, height, 3)
        '''
        # flatten the image pixels
        points = img.reshape((-1, img.shape[-1]))
        # fit the points
        centers = self.fit(points).astype(np.int)
        # Replace each pixel value with its nearby center value
        labels = self.assign_points(centers, points)
        compressed_points = centers[labels.astype(np.int)]
        compressed_img = compressed_points.reshape(img.shape).clip(0, 255)

        return compressed_img


def save_compressed_image(k, img):
    kmeans = KMeans(k=k, max_iter=10)
    compressed_img = kmeans.compress(img).round().astype(np.uint8)
    plt.figure(figsize=(10, 10))
    plt.imshow(compressed_img)
    plt.title(f'Compressed Image (k={k})')
    plt.axis('off')
    plt.savefig(f'./compressed_image_{k}.png')


if __name__ == '__main__':
    img = read_image(filepath='../data/ustc-cow.png')
    # 为每个k值开辟一个线程
    threads = []
    for k in [2, 4, 8, 16, 32]:
        thread = threading.Thread(target=save_compressed_image, args=(k, img))
        threads.append(thread)
        thread.start()
    # 启动所有线程
    for thread in threads:
        thread.join()