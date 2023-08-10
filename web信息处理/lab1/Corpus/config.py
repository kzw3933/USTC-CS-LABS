from zhon.hanzi import punctuation as hanzi_punc
from string import punctuation as english_punc


## Spider配置

Referer = "https://movie.douban.com/"

Cookies = """ll="118183";bid=4MGld7_2UzQ;__utmz=30149280.1666588590.1.1.utmcsr=baidu|utmccn=(organic)|utmcmd=organic;dbcl2="224468347:1ViXNCPXCww";push_noty_num=0; push_doumail_num=0;__utmv=30149280.22446;__yadk_uid=HDWJsygOg3DPGd4qxxTfk4PUyYGHPqO8;__gads=ID=abb64a268a003ca1-22e8195d79d700bf:T=1666588625:RT=1666588625:S=ALNI_MY0CVtQhtulr7pEXCJpbaKzVtpehA;__utma=30149280.419976393.1666588590.1666588590.1666601460.2; ck=z6WK;_pk_ref.100001.8cb4=["","",1666792375,"https://www.baidu.com/link?url=wYqeAXqIUgTlEEIYyjzIlBbyBvqrkz_GwqaF_MBde-jzV2zkNesgxAPh0V11O6Wj&wd=&eqid=e51acc04000110d70000000363593bb5"];_pk_id.100001.8cb4=18f26e8620b03d1f.1666588589.2.1666792375.1666588702.; _pk_ses.100001.8cb4=*; ap_v=0,6.0;__gpi=UID=00000b6aa5385884:T=1666588625:RT=1666792377:S=ALNI_MZ37VMFHa9YF2W-rOFimg-ADy51bA"""

User_Agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 " \
             "Safari/537.36 "

Headers = {
        "Referer": Referer,
        "Cookie": Cookies,
        "user-agent": User_Agent
    }

Proxies = ["http://222.74.73.202:42055",
           "http://122.9.101.6:8888",
           "http://61.216.156.222:60808",
           "http://150.109.32.166:80",
           "http://183.247.199.215:30001",
           "http://120.194.55.139:6969",
           "http://120.194.55.139:6969"]

movie_ids_path = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\Movie_id.txt"
book_ids_path = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\Book_id.txt"

movie_url_base = "https://movie.douban.com/subject/"
book_url_base = "https://book.douban.com/subject/"

movie_htmls_dir = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\htmls\\movies\\"
book_htmls_dir = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\htmls\\books\\"

M_director_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\m_director.txt"
M_writer_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\m_writer.txt"
M_type_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\m_type.txt"
M_made_country_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\m_made_country.txt"
M_language_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\m_language.txt"
M_show_time_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\m_show_time.txt"
M_duration_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\m_duration.txt"
M_alias_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\m_alias.txt"
M_imdb_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\m_imdb.txt"
M_title_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\m_title.txt"
M_synopsis_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\m_synopsis.txt"
M_actors_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\m_actors.txt"

B_writer_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\b_writer.txt"
B_publisher_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\b_publisher.txt"
B_publishtime_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\b_publishtime.txt"
B_pagenums_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\b_pagenums.txt"
B_price_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\b_price.txt"
B_binding_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\b_binding.txt"
B_isbn_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\b_isbn.txt"
B_title_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\b_title.txt"
B_synopsis_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\b_synopsis.txt"
B_writerinfo_file = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\txts\\b_writerinfo.txt"




## Corpus

Punctutation = hanzi_punc + english_punc
stop_words_path = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\stop_words.txt"

Corpus_load_path = ""

Stopwords = []

with open(stop_words_path, "r", encoding='utf-8') as f:
    for line in f.readlines():
        Stopwords.append(line.split('\n')[0])

pre_load_movie_corpus_path = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\movie_corpus.pkl"
pre_load_book_corpus_path = "C:\\Users\\Administrator\\Desktop\\IR\\Corpus\\data\\book_corpus.pkl"


