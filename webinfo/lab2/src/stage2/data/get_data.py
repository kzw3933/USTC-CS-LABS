import pickle
from src.stage2.data.config import *

class RDF:
    def __init__(self, all_triplets_set=None):
        self.all_triplets_set = all_triplets_set
        self.entitys2id = dict()
        self.releations2id = dict()

    def get_entitys2id_map(self, preload = None):
        next_indice = 0
        if preload:
            self.entitys2id = preload
            next_indice = len(preload)

        for line in self.all_triplets_set:
            triplet = line.strip('\n').split('\t')
            if triplet[0] not in self.entitys2id:
                self.entitys2id[triplet[0]] = str(next_indice)
                next_indice += 1
            if triplet[2] not in self.entitys2id:
                self.entitys2id[triplet[2]] = str(next_indice)
                next_indice += 1

    def get_releations2id_map(self, preload = None):
        next_indice = 0
        if preload:
            self.releations2id = preload
            next_indice = len(preload)

        for line in self.all_triplets_set:
            triplet = line.strip('\n').split('\t')
            if triplet[1] not in self.releations2id:
                self.releations2id[triplet[1]] = str(next_indice)
                next_indice += 1


    def refactor(self):
        new_triplets_set = []
        if self.releations2id and self.entitys2id:
            for line in self.all_triplets_set:
                triplet = line.strip('\n').split('\t')
                new_triplet = []
                new_triplet.append(self.entitys2id[triplet[0]])
                new_triplet.append(self.releations2id[triplet[1]])
                new_triplet.append(self.entitys2id[triplet[2]])
                new_triplets_set.append("\t".join(new_triplet)+"\n")

        self.all_triplets_set = new_triplets_set

    def save(self, pkl_path=None, map_path=None, rdf_path=None):
        if pkl_path:
            with open(rdf_pkl_path, "wb") as f:
                all_elements = [self.all_triplets_set, self.releations2id, self.entitys2id]
                pickle.dump(all_elements, f)
        if map_path and rdf_path:
            with open(rdf_path, "w", encoding='utf-8') as f:
                for line in self.all_triplets_set:
                    f.write(line)

            with open(map_path, "w", encoding='utf-8') as f:
                f.write(str(len(self.releations2id))+"\n")
                for k,v in self.releations2id.items():
                    f.write('\t'.join([k,v]) + "\n")
                f.write(str(len(self.entitys2id))+"\n")
                for k,v in self.entitys2id.items():
                    f.write('\t'.join([k,v]) + "\n")


    def load(self, rdf_pkl_path=None, map_path=None, rdf_path=None):
        if rdf_pkl_path:
            with open(rdf_pkl_path, "rb") as f:
                all_elements = pickle.load(f)
                self.all_triplets_set, self.releations2id, self.entitys2id = all_elements

        else:
            with open(rdf_path, "r", encoding='utf-8') as f:
                self.all_triplets_set = f.readlines()

            with open(map_path, "w", encoding='utf-8') as f:
                stage = 0
                for line in f.readlines():
                    if stage == 0:
                        releations_num = int(line.strip('\n'))
                        stage = 1

                    elif stage == 1:
                        map_list = line.strip('\n').split('\t')
                        self.releations2id[map_list[0]] = map_list[1]
                        releations_num -= 1
                        if releations_num == 0:
                            stage = 2

                    elif stage == 3:
                        stage = 4
                    elif stage == 4:
                        map_list = line.strip('\n').split('\t')
                        self.entitys2id[map_list[0]] = map_list[1]

if __name__ == '__main__':

    movie2fb = dict()
    movie2id = dict()
    fb2id = dict()
    all_triplets = []


    with open(movie_fb_path, "r") as f:
        for i in f.readlines():
            a, b = i.strip('\n').split('\t')
            b = "<http://rdf.freebase.com/ns/" + b + ">"
            movie2fb[a] = b

    with open(movie_id_map_path, "r") as f:
        for i in f.readlines():
            a, b = i.strip('\n').split('\t')
            movie2id[a] = b

    for k, v in movie2fb.items():
        fb2id[v] = movie2id[k]

    with open(extract_KG_file, "r", encoding='utf-8') as f:
        all_triplets = f.readlines()


    rdf = RDF(all_triplets)
    rdf.get_entitys2id_map(fb2id)
    rdf.get_releations2id_map()
    rdf.refactor()
    rdf.save(rdf_pkl_path, rdf_map_path, rdf_triplets_path)

