import csv
import pickle
from IR.Recommend.config import  user_social_path, userid_path, movieid_path, \
                                 movie_tag_path, mtypeid_path, ctokenid_path, \
                                 movie_score_path, usertype_path, dataset_root, \
                                 mtype_path, movie_comments_len, movie_types_len,\
                                 movie_types_max, movie_ctokens_max, user_scoreformovies_path


def get_user_id():
    ## 获取user2id字典并保存
    user2id = {}
    id2user = {}
    with open(movie_score_path, "r", encoding='utf-8') as f:

        f_csv = csv.DictReader(f)
        movie_list = []
        for row in f_csv:
            movie_list.append(row['user_id'])

        for j in movie_list:
            if j not in user2id:
                index = len(id2user)
                id2user[str(index)] = j
                user2id[j] = index

    with open(userid_path, "wb") as f:
        pickle.dump((user2id, id2user), f)


def get_user_socialtype():

    user2type = {}
    types = []

    with open(userid_path, "rb") as f:
        user2id, _ = pickle.load(f)

    with open(user_social_path, "r") as f:
        for i in f.readlines():
            user_list = []
            i = i.strip('\n')

            main_user = i.split(':')[0]
            other_users = i.split(':')[1].split(',')

            if main_user not in user2id:
                continue
            user_list.append(main_user)
            other_users = [i for i in other_users if i in user2id]
            user_list.extend(other_users)

            utype = None

            for j in user_list:
                if j in user2type:
                    utype = user2type[j]
                    break

            if utype:
                for j in user_list:
                    user2type[j] = utype
            else:
                utype = len(types)
                types.append(utype)
                for j in user_list:
                    user2type[j] = utype

    with open(usertype_path, "wb") as f:
        pickle.dump(user2type, f)

def get_movie_id():

    ## 获取movie2id字典并保存
    movie2id = {}
    id2movie = {}
    with open(movie_score_path,"r",encoding='utf-8') as f:
        f_csv = csv.DictReader(f)
        movie_list = []
        for row in f_csv:
            movie_list.append(row['movie_id'])

        for j in movie_list:
            if j not in movie2id:
                index = len(id2movie)
                id2movie[str(index)] = j
                movie2id[j] = index

    with open(movieid_path,"wb") as f:
        pickle.dump((movie2id, id2movie),f)

def get_movie_typesid():

    ## 获取mtype2id字典并保存
    mtype2id = {}
    id2mtype = {}
    with open(movie_tag_path,"r",encoding='utf-8') as f:

        f_csv = csv.DictReader(f)
        movie_list = []
        for row in f_csv:
            movie_list.extend(row['tag'].split(','))

        for j in movie_list:
            if j not in mtype2id:
                index = len(id2mtype)
                id2mtype[str(index)] = j
                mtype2id[j] = index
    with open(mtypeid_path,"wb") as f:
        pickle.dump((mtype2id, id2mtype),f)


def get_comment_tokenid():
    ## 获取ctoken2id字典并保存
    ctoken2id = {}
    id2ctoken = {}
    with open(movie_score_path, "r", encoding='utf-8') as f:

        f_csv = csv.DictReader(f)
        movie_list = []
        for row in f_csv:
            movie_list.extend(row['tag'].split(','))

        for j in movie_list:
            if j not in ctoken2id:
                index = len(id2ctoken)
                id2ctoken[str(index)] = j
                ctoken2id[j] = index

    with open(ctokenid_path, "wb") as f:
        pickle.dump((ctoken2id, id2ctoken), f)


def get_movie_type():
    ## 获取movie2types字典并保存
    movie2types ={}

    with open(mtypeid_path, "rb") as f:
        mtype2id,_ = pickle.load(f)

    with open(movie_tag_path, "r", encoding='utf-8') as f:

        f_csv = csv.DictReader(f)
        for row in f_csv:
            if row['id'] not in movie2types:
                movie2types[row['id']] = [mtype2id[i] for i in row['tag'].split(',')]

    with open(mtype_path, "wb") as f:
        pickle.dump(movie2types, f)

def get_dataset():

    with open(userid_path,"rb") as f:
        user2id,_ = pickle.load(f)
    with open(usertype_path,"rb") as f:
        user2type = pickle.load(f)

    with open(movieid_path, "rb") as f:
        movie2id,_  = pickle.load(f)
    with open(ctokenid_path, "rb") as f:
        ctoken2id,_ = pickle.load(f)
    with open(mtype_path, "rb") as f:
        mtype = pickle.load(f)


    with open(movie_score_path, "r", encoding='utf-8') as f:
        totals = len(f.readlines())

    with open(movie_score_path,"r",encoding='utf-8') as f:
        f_csv = csv.DictReader(f)
        for i,row in enumerate(f_csv):
            user_id = user2id[row['user_id']]
            user_type = user2type[row['user_id']]
            movie_id = movie2id[row['movie_id']]
            movie_type = mtype[row['movie_id']]

            if len(movie_type) < movie_types_len:
                movie_type.extend([movie_types_max+1]*(movie_types_len-len(movie_type)))
            else:
                movie_type = movie_type[0:movie_types_len]

            movie_comments = [ctoken2id[i] for i in row['tag'].split(',')]
            if len(movie_comments) < movie_comments_len:
                movie_comments.extend([movie_ctokens_max+1]*(movie_comments_len-len(movie_comments)))
            else:
                movie_comments = movie_comments[0:movie_comments_len]

            movie_score = row['movie_score']

            with open(dataset_root + str(i) + ".txt", "w") as b:
                b.write(str(((user_id, user_type, movie_id, movie_type, movie_comments), movie_score)))

def get_user_scoreformovies():

    with open(userid_path,"rb") as f:
        user2id,_ = pickle.load(f)

    with open(movieid_path, "rb") as f:
        movie2id,_  = pickle.load(f)

    with open(movie_score_path,"r",encoding='utf-8') as f:
        f_csv = csv.DictReader(f)
        user_scoreformovies = {}
        for i,row in enumerate(f_csv):
            user_id = user2id[row['user_id']]
            movie_id = movie2id[row['movie_id']]
            movie_score = row['movie_score']

            if str(user_id) not in user_scoreformovies:
                user_scoreformovies[str(user_id)] = {}
                user_scoreformovies[str(user_id)][str(movie_id)]= movie_score
            else:
                user_scoreformovies[str(user_id)][str(movie_id)]= movie_score


    with open(user_scoreformovies_path, "wb") as f:
        pickle.dump(user_scoreformovies, f)




if __name__ == '__main__':
    # get_user_id()
    # get_user_socialtype()
    # get_movie_id()
    # get_movie_typesid()
    # get_movie_type()
    # get_comment_tokenid()
    # get_dataset()
    get_user_scoreformovies()

