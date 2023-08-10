import gzip
import csv
from src.stage1.config import *

class triplet_filter:
    """
    triplet_set: 要筛选的实体集合
    base_entitys_set: 三元组集合
    entitys_min: 实体最少出现次数
    entitys_max: 实体最多出现次数
    releation_min: 关系最少出现次数
    """
    def __init__(self, triplet_set, base_entitys_set, entitys_min, entitys_max=20000, releation_min=50):
        self.triplet_set = self._filter_by_standard(triplet_set)
        self.base_entitys_set = base_entitys_set
        self.entitys_min = entitys_min
        self.releation_min = releation_min
        self.entitys_max = entitys_max
        self.releations2counts = self._get_releations_counts()
        self.triplet_set, self.save_releations_set = self._filter_by_releations(self.triplet_set)
        self.entitys2counts = self._get_entitys_counts()


    ## 计算各个实体在三元组中出现的次数
    def _get_entitys_counts(self):
        entitys2count = dict()
        for line in self.triplet_set:
            triplet = line.strip('\n').split('\t')
            if(triplet[0] not in entitys2count):
                entitys2count[triplet[0]] = 1
            else:
                entitys2count[triplet[0]] += 1
            if (triplet[2] not in entitys2count):
                entitys2count[triplet[2]] = 1
            else:
                entitys2count[triplet[2]] += 1

        return entitys2count

    ## 计算每种关系在三元组中出现的次数
    def _get_releations_counts(self):
        releations2count = dict()
        for line in self.triplet_set:
            triplet = line.strip('\n').split('\t')
            if (triplet[1] not in releations2count):
                releations2count[triplet[1]] = 1
            else:
                releations2count[triplet[1]] += 1

        return releations2count

    ## 筛选以"<http://rdf.freebase.com/ns/"开头的实体
    def _filter_by_standard(self, triplet_set):
        new_triplet_set = []
        for line in triplet_set:
            triplet = line.strip('\n').split('\t')
            if not triplet[0].startswith("<http://rdf.freebase.com/ns/") or not triplet[2].startswith("<http://rdf.freebase.com/ns/"):
                continue
            else:
                new_triplet_set.append(line)

        return new_triplet_set


    ## 筛选出出现次数大于releation_min的关系以及其出现的三元组
    def _filter_by_releations(self, triplet_set):
        new_triplet_set = []
        save_releations_set = set()
        for line in triplet_set:
            triplet = line.strip('\n').split('\t')
            if self.releations2counts[triplet[1]] > self.releation_min:
                new_triplet_set.append(line)
                save_releations_set.add(triplet[1])

        return new_triplet_set, save_releations_set

    ## 筛选出下一跳的实体
    def get_extend_entitys(self):
        extend_entitys = set()
        for k,v in self.entitys2counts.items():
            if v >=  self.entitys_min and v <= self.entitys_max:
                extend_entitys.add(k)

        return extend_entitys


    ## 计算筛选出的三元组
    def run(self):
        extend_entitys = self.get_extend_entitys()
        new_triplet_set = []
        for line in self.triplet_set:
            triplet = line.strip('\n').split('\t')
            if (triplet[0] in self.base_entitys_set or triplet[0] in extend_entitys) and (triplet[2] in self.base_entitys_set or triplet[2] in extend_entitys):
                new_triplet_set.append(line)

        self.triplet_set = new_triplet_set

    ## 保存筛选出的三元组
    def save(self, file):
        self.run()
        with open(file, "a", encoding='utf-8') as f:
            for line in self.triplet_set:
                f.write(line)

if __name__ == '__main__':

    movie_id2fb = {}
    movie_entitys = set()
    extend_entitys = set()

    ## 获得id2fb的映射表
    with open(movie_id2fb_file, "r", encoding='utf-8') as f:
        for i in f.readlines():
            a, b = i.strip().split()
            movie_id2fb[a] = "<http://rdf.freebase.com/ns/" + b + ">"
            movie_entitys.add("<http://rdf.freebase.com/ns/" + b + ">")

    print("获得id2fb的映射表" + "=======" + "over!")

    ## 根据电影id匹配提取freebase中的实体提取所有三元组
    with gzip.open(freebase_file, "rb") as f:
        with open(extract_KG_by_movie_entitys_raw_file, "a", encoding='utf-8') as b:
            for line in f:
                line = line.strip()
                triplet = line.decode().strip('\n').split('\t')
                if triplet[0] in movie_entitys or triplet[2] in movie_entitys:
                    b.write("\t".join(line.decode().strip('\n').split('\t')[0:3]) + "\n")

    print("根据电影id匹配提取freebase中的实体提取所有三元组" + "=======" + "over!")

    ## 过滤保存子图并获取并过滤出所需的扩展实体
    with open(extract_KG_by_movie_entitys_raw_file, "r", encoding='utf-8') as f:
        filter = triplet_filter(f.readlines(), movie_entitys, 20)
        extend_entitys = filter.get_extend_entitys()
        filter.save(extract_KG_by_movie_entitys_file)

    print("过滤保存子图并获取并过滤出所需的扩展实体" + "=======" + "over!")

    ## 提取扩展实体对应的所有三元组
    with gzip.open(freebase_file, "rb") as f:
        with open(extract_KG_by_extend_entitys_raw_file, "a", encoding='utf-8') as b:
            for line in f:
                line = line.strip()
                triplet = line.decode().strip('\n').split('\t')
                if triplet[0] in movie_entitys or triplet[2] in movie_entitys:
                    continue
                if triplet[0] in extend_entitys or triplet[2] in extend_entitys:
                    b.write("\t".join(line.decode().strip('\n').split('\t')[0:3]) + "\n")

    print("提取扩展实体对应的所有三元组" + "=======" + "over!")

    ## 过滤并保存子图
    with open(extract_KG_by_movie_entitys_raw_file, "r", encoding='utf-8') as f:
        filter = triplet_filter(f.readlines(), extend_entitys, 15)
        filter.save(extract_KG_by_extend_entitys_file)

    print("过滤并保存子图" + "=======" + "over!")

    ## 实体扩充
    with open(movie_tag_file, "r", encoding='utf-8') as f:
        f_csv = csv.DictReader(f)
        with open(extend_KG_by_tag_file, "a", encoding='utf-8') as b:
            for row in f_csv:
                tags_set = set()
                for item in row['tag'].strip().split(','):
                    tags_set.add(item)
                for item in tags_set:
                    triplet = []
                    if row['id'] not in movie_id2fb:
                        continue
                    else:
                        triplet.append(movie_id2fb[row['id']])
                        triplet.append("<tag>")
                        triplet.append("<" + item + ">")
                        b.write("\t".join(triplet) + "\n")
    print("实体扩充" + "=======" + "over!")

    ## TODO: 实体对齐(合并相似语义的tag实体)
    ## TODO: 貌似不需要,一共就18个tag,而且各不相同: '动画', '青春', '喜剧', '科幻', '大陆', '香港', '人性', '犯罪', '动作', '美国',
    ## TODO: '纪录片', '文艺', '悬疑', '日本', '短片', '惊悚', '经典', '爱情'

    ## 合并KG并保存
    with open(extract_KG_file, "a", encoding='utf-8') as f:
        f1 = open(extract_KG_by_movie_entitys_file, "r", encoding='utf-8')
        f2 = open(extract_KG_by_extend_entitys_file, "r", encoding='utf-8')
        f3 = open(extend_KG_by_tag_file, "r", encoding='utf-8')
        for i in f1.readlines():
            f.write(i)
        for i in f2.readlines():
            f.write(i)
        for i in f3.readlines():
            f.write(i)
    print("合并KG并压缩保存" + "=======" + "over!")