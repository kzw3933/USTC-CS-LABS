from bs4 import BeautifulSoup
from IR.Corpus.config import *
import os
import re

class Filter:
    def __init__(self, names=None, re_group=None, *args):
        self.names = names
        self.re_group = re_group
        self.patterns = args

    def run(self, text):
        parser = BeautifulSoup(text, "html.parser")
        ret = []
        if self.patterns:
            for (key, value) in self.patterns:
                if not ret:
                    if key == "name":
                        ret.extend(parser.find_all(name=value))
                    else:
                        ret.extend(parser.find_all(attrs={key: value}))
                    if not ret:
                        return ""
                else:
                    temp = []
                    for i in ret:
                        if key == "name":
                            temp.extend(i.find_all(name=value))
                        else:
                            temp.extend(i.find_all(attrs={key: value}))

                    ret = temp
        else:
            ret = [parser]

        if not ret:
            return ""
        else:
            if self.names:
                temp = []
                for keyword in self.names:
                    for i in ret:
                        if re.search(keyword, i.text, re.DOTALL):
                            if i.text.replace(keyword, "").strip():
                                temp.append(i.text.replace(keyword, "").strip())
                    if temp:
                        break

                ret = temp
            elif self.re_group:
                re_pattern, group = self.re_group
                temp = []
                for i in ret:
                    if re.search(re_pattern, str(i), re.DOTALL):
                        temp.append(re.search(re_pattern, str(i), re.DOTALL).groups()[int(group)].strip())
                ret = temp
            else:
                ret = [i.text.replace('\n'," ").strip() for i in ret]
            return ret[0] if len(ret) == 1 else " ".join(ret)





if __name__ == '__main__':

    ## 创建电影信息的解析器

    ##  基本信息(导演,编剧,类型,制片国家/地区,语言,上映日期,片长,又名,IMDb)
    m_director = Filter(["导演:"], None, ("id", "info"), ("name", "span"))
    m_writer = Filter(["编剧:"], None, ("id", "info"), ("name", "span"))
    m_type = Filter(None, None, ("id", "info"), ("property", "v:genre"))
    m_made_country = Filter(None, (r'制片国家/地区:</span>(.*?)<br/?>',0), ("id", "info"))
    m_language = Filter(None, (r'语言:</span>(.*?)<br/?>', 0), ("id", "info"))
    m_show_time = Filter(None, None, ("id", "info"), ("property", "v:initialReleaseDate"))
    m_duration = Filter(None, None, ("id", "info"), ("property", "v:runtime"))
    m_alias = Filter(None, (r'又名:</span>(.*?)<br/?>', 0), ("id", "info"))
    m_imdb = Filter(None, (r'IMDb:</span>(.*?)<br/?>', 0), ("id", "info"))
    m_title = Filter(None,None,("name","title"))

    ###  内容介绍
    m_synopsis = Filter(None, None, ("id", "link-report"), ("property", "v:summary"))

    ###  演员信息
    m_actors = Filter(None, None, ("class", "celebrity"), ("class", "name"), ("name", "a"))

    ## 解析电影

    m_director_file = open(M_director_file, "w", encoding="utf-8")
    m_writer_file = open(M_writer_file, "w", encoding="utf-8")
    m_type_file = open(M_type_file, "w", encoding="utf-8")
    m_made_country_file = open(M_made_country_file, "w", encoding="utf-8")
    m_language_file = open(M_language_file, "w", encoding="utf-8")
    m_show_time_file = open(M_show_time_file, "w", encoding="utf-8")
    m_duration_file = open(M_duration_file, "w", encoding="utf-8")
    m_alias_file = open(M_alias_file, "w", encoding="utf-8")
    m_imdb_file = open(M_imdb_file, "w", encoding="utf-8")
    m_title_file = open(M_title_file, "w", encoding="utf-8")
    m_synopsis_file = open(M_synopsis_file, "w", encoding="utf-8")
    m_actors_file = open(M_actors_file, "w", encoding="utf-8")

    for html_file in os.listdir(movie_htmls_dir):
        with open(movie_htmls_dir + html_file, "r", encoding='utf-8') as f:
            fcontent = f.read()
            m_director_file.write(html_file + ": " + m_director.run(fcontent)+"\n")
            m_writer_file.write(html_file + ": " + m_writer.run(fcontent)+"\n")
            m_type_file.write(html_file + ": " + m_type.run(fcontent)+"\n")
            m_made_country_file.write(html_file + ": " + m_made_country.run(fcontent)+"\n")
            m_language_file.write(html_file + ": " + m_language.run(fcontent)+"\n")
            m_show_time_file.write(html_file + ": " + m_show_time.run(fcontent)+"\n")
            m_duration_file.write(html_file + ": " + m_duration.run(fcontent)+"\n")
            m_alias_file.write(html_file + ": " + m_alias.run(fcontent)+"\n")
            m_imdb_file.write(html_file + ": " + m_imdb.run(fcontent)+"\n")
            m_title_file.write(html_file + ": " + m_title.run(fcontent) + "\n")
            m_synopsis_file.write(html_file + ": " +
                                  m_synopsis.run(fcontent).replace("\n", " ").replace("\u3000", " ").replace("\u2022"," ").strip()+"\n")
            m_actors_file.write(html_file + ": " + m_actors.run(fcontent)+"\n")

            print(html_file+" is ok!")

    m_director_file.close()
    m_writer_file.close()
    m_type_file.close()
    m_made_country_file.close()
    m_language_file.close()
    m_show_time_file.close()
    m_duration_file.close()
    m_alias_file.close()
    m_imdb_file.close()
    m_title_file.close()
    m_synopsis_file.close()
    m_actors_file.close()



    ## 创建书籍信息的解析器

    ###  基本信息(作者,出版社,出版年,页数,定价,装帧,ISBN)
    b_writer = Filter(None, (r'作者((</span>:)|(:</span>))(.*?)<br/?>', 3), ("id", "info"))
    b_publisher = Filter(None, (r'出版社:</span>(.*?)<br/?>', 0), ("id", "info"))
    b_publishtime = Filter(None, (r'出版年:</span>(.*?)<br/?>', 0), ("id", "info"))
    b_pagenums = Filter(None, (r'页数:</span>(.*?)<br/?>', 0), ("id", "info"))
    b_price = Filter(None, (r'定价:</span>(.*?)<br/?>', 0), ("id", "info"))
    b_binding = Filter(None, (r'装帧:</span>(.*?)<br/?>', 0), ("id", "info"))
    b_isbn = Filter(None, (r'ISBN:</span>(.*?)<br/?>', 0), ("id", "info"))
    b_title = Filter(None,None,("name","title"))


    ###  内容介绍
    b_synopsis = Filter(None, None, ("id", "link-report"), ("class", "intro"),("name","p"))

    ###  作者介绍
    b_writerinfo = Filter(None, (r'作者简介(.*?)class="intro"(.*?)<p>(.*?)</p>', 2))

    ## 解析电影
    b_writer_file = open(B_writer_file, "w", encoding="utf-8")
    b_publisher_file = open(B_publisher_file, "w", encoding="utf-8")
    b_publishtime_file = open(B_publishtime_file, "w", encoding="utf-8")
    b_pagenums_file = open(B_pagenums_file, "w", encoding="utf-8")
    b_price_file = open(B_price_file, "w", encoding="utf-8")
    b_binding_file = open(B_binding_file, "w", encoding="utf-8")
    b_isbn_file = open(B_isbn_file, "w", encoding="utf-8")
    b_title_file = open(B_title_file, "w", encoding="utf-8")
    b_synopsis_file = open(B_synopsis_file, "w", encoding="utf-8")
    b_writerinfo_file = open(B_writerinfo_file, "w", encoding="utf-8")

    for html_file in os.listdir(book_htmls_dir):
        with open(book_htmls_dir + html_file, "r", encoding='utf-8') as f:
            fcontent = f.read()

            temp = []
            for i in re.findall(r'<a.*?>(.*?)</a>', str(b_writer.run(fcontent)), re.DOTALL):
                i = re.sub(r'\s+'," ",i)
                if i.strip('\n').strip():
                    temp.append(i.strip('\n').strip())

            ret = temp[0] if len(temp) == 1 else " ".join(temp)
            b_writer_file.write(html_file+": "+ret+"\n")


            if re.search(r'<a.*?>(.*?)</a>', b_publisher.run(fcontent), re.DOTALL):
                b_publisher_file.write(html_file+": "+re.search(r'<a.*?>(.*?)</a>', b_publisher.run(fcontent),
                                                                re.DOTALL).groups()[0]+"\n")
            elif re.search(r'(.*?)<br/?>', b_publisher.run(fcontent), re.DOTALL):
                b_publisher_file.write(html_file + ": " + re.search(r'(.*?)<br/?>', b_publisher.run(fcontent), re.DOTALL).groups()[0] + "\n")
            else:
                b_publisher_file.write(
                    html_file + ": " + b_publisher.run(fcontent) + "\n")

            b_publishtime_file.write(html_file + ": " + b_publishtime.run(fcontent) + "\n")
            b_pagenums_file.write(html_file + ": " + b_pagenums.run(fcontent) + "\n")
            b_price_file.write(html_file + ": " + b_price.run(fcontent) + "\n")
            b_binding_file.write(html_file + ": " + b_binding.run(fcontent) + "\n")
            b_isbn_file.write(html_file + ": " + b_isbn.run(fcontent) + "\n")
            b_title_file.write(html_file + ": " + b_title.run(fcontent) + "\n")
            b_synopsis_file.write(html_file + ": " + re.sub(r'\s+', " ", b_synopsis.run(fcontent)) + "\n")
            b_writerinfo_file.write(html_file + ": " + re.sub(r'\s+', " ", str(b_writerinfo.run(fcontent))) + "\n")
            print(html_file + " is ok!")

    b_writer_file.close()
    b_publisher_file.close()
    b_publishtime_file.close()
    b_pagenums_file.close()
    b_price_file.close()
    b_binding_file.close()
    b_isbn_file.close()
    b_title_file.close()
    b_synopsis_file.close()
    b_writerinfo_file.close()














