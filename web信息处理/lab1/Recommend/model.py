import torch.nn as nn
import torch
import torch.nn.functional as F
from IR.Recommend.config import *
import numpy as np



class TextCnn(nn.Module):
    def __init__(self, vocab_max, embed_dim, kernel_num, output_dim, dropout=0.5):
        super(TextCnn, self).__init__()

        self.vocab_max = vocab_max
        self.embed_dim = embed_dim
        self.channel = 1
        self.kernel_num = kernel_num
        self.window_sizes = window_sizes

        self.embed_layer = nn.Embedding(self.vocab_max, self.embed_dim)
        self.convs = nn.ModuleList([
            nn.Conv2d(self.channel, self.kernel_num, (window_size, self.embed_dim)) for window_size in self.window_sizes
        ])
        self.dropout = nn.Dropout(dropout)
        self.fc = nn.Linear(len(self.window_sizes)*self.kernel_num, output_dim)

    def forward(self, x):
        x = self.embed_layer(x)                                             # (N,vocab_max,embed_dim)
        x = x.unsqueeze(1)                                                  # (N,channel,vocab_max,embed_dim)
        x = [F.relu(conv(x)).squeeze(3) for conv in self.convs]             # len(window_sizes)*(N,kernel_num,conved_dim)
        x = [F.max_pool1d(line, line.size(2)).squeeze(2) for line in x]     # len(window_sizes)*(N,kernel_num)
        x = torch.cat(x, 1)                                                 # (N,kernel_num*len(window_sizes))
        x = self.dropout(x)                                                 # (N,kernel_num*len(window_sizes))
        x = self.fc(x)                                                      # (N,output_dim)

        return x
class Model(nn.Module):
    def __init__(self):
        super(Model, self).__init__()
        self.socialtype_embedding_layer = nn.Embedding(socialtype_max+1, embed_dim)
        self.uid_embedding_layer = nn.Embedding(uid_max+1, embed_dim)
        self.movie_types_embedding_layer = nn.Embedding(movie_types_max+2, embed_dim)
        self.movie_id_embedding_layer = nn.Embedding(movie_id_max+1, embed_dim)
        self.movie_comments_layer = TextCnn(movie_ctokens_max+2, embed_dim, kernel_num, movie_comments_dim)
        self.user_fc1 = nn.Linear(2*embed_dim, 32)
        self.user_fc2 = nn.Linear(32, 16)
        self.movie_fc1 = nn.Linear(2*embed_dim, 32)
        self.movie_fc2 = nn.Linear(32+movie_comments_dim, 32)
        self.movie_fc3 = nn.Linear(32, 16)

    def forward(self, x):
        user_ids, user_socialtype, movie_ids,movie_types,movie_comments = x

        user_socialtype = self.socialtype_embedding_layer(user_socialtype).squeeze()
        user_ids = self.uid_embedding_layer(user_ids).squeeze()
        movie_types = self.movie_types_embedding_layer(movie_types)
        movie_types = movie_types.sum(axis=1)
        movie_ids = self.movie_id_embedding_layer(movie_ids).squeeze()
        movie_comments = self.movie_comments_layer(movie_comments)

        user_feature = torch.cat([user_ids,user_socialtype],1)
        user_feature = self.user_fc1(user_feature)
        user_feature = self.user_fc2(user_feature)

        movie_feature = torch.cat([movie_types,movie_ids],1)
        movie_feature = self.movie_fc1(movie_feature)
        movie_feature = torch.cat([movie_feature, movie_comments], 1)
        movie_feature = self.movie_fc2(movie_feature)
        movie_feature = self.movie_fc3(movie_feature)

        ret = torch.sum(torch.mul(movie_feature,user_feature),dim=-1)
        ret = torch.sigmoid(ret)*5
        return ret




if __name__ == '__main__':

    # movie_cnn = TextCnn(1000, 32, 10, 0.5, 10)
    # print(movie_cnn)
    # a = np.random.randint(10, size=(3,4,3))
    # b = np.random.randint(10, size=(3,4,3))
    # a = torch.from_numpy(a)
    # b = torch.from_numpy(b)
    # print(torch.sum(torch.mul(a,b)))

    # movie_comments_layer = TextCnn(movie_ctokens_max, embed_dim, kernel_num, movie_comments_dim)
    # print(movie_comments_layer)

    user_ids = np.random.randint(10,size=(1000,1))
    user_socialtype = np.random.randint(10,size=(1000,1))
    movie_ids = np.random.randint(10,size=(1000,1))
    movie_types = np.random.randint(10,size=(1000,5))
    movie_comments = np.random.randint(10,size=(1000,5))
    x = user_ids, user_socialtype, movie_ids, movie_types, movie_comments
    model = Model()
    for name,parameter in model.named_parameters():
        if parameter.requires_grad == True:
            print(name)
    y = model(x)






