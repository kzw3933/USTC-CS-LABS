import numpy as np
import pickle

from IR.Recommend.config import predict_userscore_path, user_scoreformovies_path, userid_path

def DCG(rel_list):

    dcg = 0
    for i,rel in enumerate(rel_list):
        dcg += (2**int(rel)-1)/np.log2(i+2)
    return dcg

if __name__ == '__main__':

    save_file = open("C:\\Users\\Administrator\\Desktop\\IR\\NDCG.txt","w")

    with open(userid_path, "rb") as f:
        user2id, id2user = pickle.load(f)

    with open(predict_userscore_path,"rb") as f:
        predict_userscore = pickle.load(f)

    with open(user_scoreformovies_path,"rb") as f:
        user_scoreformovies = pickle.load(f)

    for user,scores in predict_userscore.items():
        user_name = id2user[str(user)]

        score_list = list(user_scoreformovies[str(user)].values())
        ideal_list = sorted(score_list, key= lambda x: int(x))[::-1]
        idcg = DCG(ideal_list)

        predict_scores_list = list(scores.values())
        score_list = list(user_scoreformovies[str(user)].values())
        predict_list = sorted(score_list, key=lambda x: predict_scores_list[score_list.index(x)])[::-1]
        predict_dcg = DCG(predict_list)

        if idcg == 0:
            ndcg = 0
        else:
            ndcg = predict_dcg / idcg

        save_file.write("id: "+ user_name+" ndcg: "+str(ndcg)+"\n")

    save_file.close()





















