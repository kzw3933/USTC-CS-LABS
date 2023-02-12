import torch
from torch.utils.data import DataLoader
from torch.autograd import Variable

import pickle

from IR.Recommend.config import model_save_path, dataset_root, predict_userscore_path
from IR.Recommend.dataset import MovieData


def predict():
    model = torch.load(model_save_path)

    data = MovieData(dataset_root, test=True)
    data_loader = DataLoader(data, batch_size=1000)

    user_scoreformovies = {}

    for i, (user_id, user_type, movie_id, movie_type, movie_comments, label) in enumerate(data_loader):

        user_str = [str(i.detach().numpy()) for i in user_id]
        movie_str = [str(i.detach().numpy()) for i in movie_id]

        for i in user_str:
            if i not in user_scoreformovies:
                user_scoreformovies[i] = {}

        user_id = Variable(user_id)
        user_type = Variable(user_type)

        movie_id = Variable(movie_id)
        movie_type = Variable(movie_type)
        movie_comments = Variable(movie_comments)

        user_id = user_id.cuda()
        user_type = user_type.cuda()
        movie_id = movie_id.cuda()
        movie_type = movie_type.cuda()
        movie_comments = movie_comments.cuda()

        input = (user_id, user_type, movie_id, movie_type, movie_comments)

        score = model(input)

        for i,j in enumerate(score):
            user_scoreformovies[user_str[i]][movie_str[i]] = j.item()

    with open(predict_userscore_path, "wb") as f:
        pickle.dump(user_scoreformovies, f)

if __name__ == '__main__':

    predict()

