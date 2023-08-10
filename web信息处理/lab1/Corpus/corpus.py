import jieba
from IR.Corpus.config import *
import sys
import pickle
import os
sys.path.append("D:\\Savefiles\\Projects\\Python\\IR")

# 语料库类(包含token2id,id2token以及倒排索引，均以字典为数据结构)
class Corpus:

    def __init__(self, ctype, preload=True):
        self.token2id = dict()
        self.id2token = dict()
        self.passage_list = dict()
        self.dictionary = dict()
        self.invert_indice = dict()
        self.type = ctype
        self.preload = preload
        self._prepare()


    def add(self, text_id, text_name, text=None, tokens=None):
        if text_name not in self.passage_list and text:
            self.passage_list[str(text_id)] = (text_name, text)

        all_tokens = []
        if not text and not tokens:
            return
        elif text:
            split_tokens = Corpus.splitTokens(text)
            all_tokens.extend(split_tokens)
            if tokens:
                all_tokens.extend(tokens)
        elif tokens:
            all_tokens.extend(tokens)
        all_tokens = Corpus.removeStopWords(all_tokens)

        self._updateTokens(all_tokens)
        token_ids = [self.token2id[token] for token in all_tokens]
        self._updateInvertIndice(text_id, token_ids)

    def save(self):
        if self.type == 'movie':
            with open(pre_load_movie_corpus_path,"wb") as f:
                pickle.dump((self.dictionary,self.token2id,self.passage_list, self.id2token, self.invert_indice), f)
        elif self.type == 'book':
            with open(pre_load_book_corpus_path,"wb") as f:
                pickle.dump((self.dictionary,self.token2id,self.passage_list, self.id2token, self.invert_indice), f)
    def getPassageList(self, id_list):

        return [self.passage_list[str(i)] for i in id_list]



    def _prepare(self):
        if self.type == 'movie':
            if self.preload:
                if os.path.exists(pre_load_movie_corpus_path):
                    with open(pre_load_movie_corpus_path,"rb") as f:
                        self.dictionary,self.token2id,self.passage_list,self.id2token,self.invert_indice = pickle.load(f)
                else:
                    print(pre_load_movie_corpus_path+" not found!")
                    sys.exit(-1)
                return
            with open(M_title_file,"r",encoding='utf-8') as f:
                title_list = [i.strip('\n').split(':')[-1] for i in f.readlines()]
            with open(M_type_file,"r",encoding='utf-8') as f:
                type_list = [i.strip('\n').split(':')[-1].strip().split() for i in f.readlines()]
            with open(M_synopsis_file, "r",encoding='utf-8') as f:
                synopsis_list = [i.strip('\n') for i in f.readlines()]
                for i,synopsis in enumerate(synopsis_list):
                    self.add(synopsis.split(':')[0],title_list[i], synopsis.split(':')[1],type_list[i])
                    self.add(synopsis.split(':')[0], title_list[i], None, [title_list[i].strip().split(' ')[0]])

        elif self.type == 'book':
            if self.preload:
                if os.path.exists(pre_load_book_corpus_path):
                    with open(pre_load_book_corpus_path,"rb") as f:
                        self.dictionary,self.token2id,self.passage_list,self.id2token,self.invert_indice = pickle.load(f)
                else:
                    print(pre_load_book_corpus_path+" not found!")
                    sys.exit(-1)
                return
            with open(B_title_file,"r",encoding='utf-8') as f:
                title_list = [i.strip('\n').split(':')[-1] for i in f.readlines()]
            with open(B_synopsis_file, "r",encoding='utf-8') as f:
                synopsis_list = [i.strip('\n') for i in f.readlines()]
                for i, synopsis in enumerate(synopsis_list):
                    self.add(synopsis.split(':')[0], title_list[i], synopsis.split(':')[1])
                    self.add(synopsis.split(':')[0], title_list[i], None, [title_list[i].strip().split(' ')[0]])

        for token in self.token2id:
            self.dictionary[token][1] = len(self.invert_indice[self.token2id[token]])

    def _updateTokens(self, tokens):
        for token in tokens:
            if token in self.token2id:
                continue
            tid = len(self.token2id)
            self.token2id[token] = str(tid)
            self.id2token[str(tid)] = token
            self.dictionary[token] = [str(tid),0]

    def _updateInvertIndice(self, text_id, token_ids):
        for token_id in token_ids:
            if str(token_id) not in self.invert_indice:
                self.invert_indice[str(token_id)] = dict()
                self.invert_indice[str(token_id)][str(text_id)] = 1
            else:
                if str(text_id) not in self.invert_indice[str(token_id)]:
                    self.invert_indice[str(token_id)][str(text_id)] = 1
                else:
                    self.invert_indice[str(token_id)][str(text_id)] += 1


    @staticmethod
    def splitTokens(text):
        text = jieba.cut_for_search(text)
        text = [' ' if i.strip() in Punctutation else i.strip() for i in text]
        text = [i.strip() for i in text if i.strip()]
        return text

    @staticmethod
    def removeStopWords(tokens):
        all_tokens = []
        for token in tokens:
            if token in Stopwords:
                continue
            else:
                all_tokens.append(token)
        return all_tokens


if __name__ == '__main__':


    movie_corpus = Corpus("movie", preload=False)  # 第一次使用，预加载设为false，默认也为false
    movie_corpus.save()                            # 保存已经建立的语料库模型
    book_corpus = Corpus("book",preload=False)
    book_corpus.save()















