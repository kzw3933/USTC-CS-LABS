import torch.nn
from torch.utils.data import DataLoader
from torch.autograd import Variable

from IR.Recommend.model import Model
from IR.Recommend.dataset import MovieData
from IR.Recommend.config import dataset_root, batch_size, learn_rate, \
                                    weight_decay, max_epoch, model_save_path, \
                                    train_log_dir, last_best_loss, load_old_loss

from tensorboard_logger import Logger

import time


def train():

    # 1. 模型
    try:
        model = torch.load(model_save_path)
    except:
        model = Model()

    model = model.cuda()
    # 2. 数据
    train_data = MovieData(dataset_root, train=True, test=True)
    test_data = MovieData(dataset_root, train=False)

    train_data_loader = DataLoader(train_data, batch_size=batch_size, shuffle=True)
    test_data_loader = DataLoader(test_data, batch_size=batch_size, shuffle=False)

    # 3. 目标函数和优化器
    criterion = torch.nn.MSELoss()
    lr = learn_rate

    loss = 0
    best_loss = last_best_loss if load_old_loss else 1.2
    optimizer = torch.optim.Adam(model.parameters(), lr=lr, weight_decay=weight_decay)

    logger = Logger(logdir=train_log_dir, flush_secs=2)

    # 4. 训练
    for epoch in range(max_epoch):
        for i, (user_id, user_type, movie_id, movie_type, movie_comments, label) in enumerate(train_data_loader):
            user_id = Variable(user_id)
            user_type = Variable(user_type)

            movie_id = Variable(movie_id)
            movie_type = Variable(movie_type)
            movie_comments = Variable(movie_comments)

            target = Variable(label)
            target = target.to(torch.float32)

            user_id = user_id.cuda()
            user_type = user_type.cuda()
            movie_id = movie_id.cuda()
            movie_type = movie_type.cuda()
            movie_comments = movie_comments.cuda()
            target = target.cuda()

            input = (user_id, user_type, movie_id, movie_type, movie_comments)

            optimizer.zero_grad()

            score = model(input)
            loss = criterion(score, target)
            loss.backward()
            optimizer.step()

            # 可视化训练过程同时保存模型

            if i % 50 == 0:
                print(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time()))+"\t\t\t"+"\033[31mbest "
                                                                                           "loss is: "+str(best_loss)+"\033[0m")

            if i % 5 == 0:
                logger.log_value('loss', loss, step=i)
                print("epoch: " + str(epoch)+"\t"+"iteration: "+str(i)+"\t"+"loss: "+str(loss))

        if epoch > 10 :
            if loss < best_loss:
                best_loss = loss
                torch.save(model,model_save_path)

    print()

if __name__ == '__main__':

    train()


