from IR.Corpus.corpus import Corpus
import IR.Search.syntax_book as syntax_book
import IR.Search.syntax_movie as syntax_movie

class IRBuilder:

    def __init__(self):
        self.movie_corpus = Corpus(ctype="movie", preload=True)
        self.book_corpus = Corpus(ctype="book", preload=True)

    # 对检索结果进行预处理、排序
    def resSort(self,raw):
        #返回结果最初为字典，现将其转化为列表
        raw = raw[0]
        ret = []
        for item in raw:
            ret.append(item)
        #按词频降序排列
        ret.sort(key=lambda x: raw[x],reverse = True)
        return ret


    def BoolExprFormat(self,query):
        query = query.replace('（', ' ( ').replace('）', ' ) ')
        query = query.replace('(', ' ( ').replace(')', ' ) ').strip()
        query = query.replace('and', ' AND ').replace('or', ' OR ').replace('not', ' NOT ')
        return query


if __name__ == '__main__':

    IR = IRBuilder()

    print("\033[34m"+ "*"*20 + "欢迎进入布尔检索系统"+"*"*20 + "\033[0m")

    while True:
        type = input("input your query type" +"\033[32m" + " [book or movie ]"+ "\033[0m"+ ": ")
        if type != "movie" and type != "book":
            continue

        query = input("input your query boolen expression" + "\033[32m" + " [ eg a and (b or c) ]"+ "\033[0m"+": ")
        result = None

        query = IR.BoolExprFormat(query)
        if type == 'movie':
            parser = syntax_movie.parser
        else:
            parser = syntax_book.parser
        try:
            result = parser.parse(query)
            result = IR.resSort(result)

            if type == 'movie':
                result = IR.movie_corpus.getPassageList(result)
            else:
                result = IR.book_corpus.getPassageList(result)
        except:
            print("\033[31m"+"Invalid inputs cause some errors happen when searching!"+"\033[0m")

        if result:
            print("The search results are in following: ")
            print("*" * 100)
            for item in result:
                passage_name,passage_text = item
                print("passage_name: "+"\033[32m"+passage_name+(50-len(passage_name))*" "+"\033[0m",end="")
                passage_text = str(passage_text.split('。')[0])+"..." if len(passage_text) >=10 else str(passage_text)+"..."
                print("passage_text: " + "\033[32m" + passage_text + "\033[0m")
            print("*"*100)


        else:
            print("\033[31m"+"Can't found the related content in system!"+"\033[0m")

        next = input("continue "+"\033[32m"+"[Y/n]? "+"\033[0m")
        if next == 'n' or next == 'N':
            break














