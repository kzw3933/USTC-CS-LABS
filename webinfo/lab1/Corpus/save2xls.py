from IR.Corpus.config import *
import xlwt

def movie2csv():
    workbook = xlwt.Workbook(encoding="utf-8",style_compression=0)
    sheet = workbook.add_sheet('豆瓣电影', cell_overwrite_ok=True)
    movie_cols = ("电影名", "ID", "导演", "编剧", "电影类型", "制作国家/地区", "语言",
                  "上映时间", "时长", "别名", "IMDB", "内容简介", "主演")

    for i in range(len(movie_cols)):
        sheet.write(0,i,movie_cols[i])

    m_director_file = open(M_director_file, "r", encoding="utf-8")
    m_writer_file = open(M_writer_file, "r", encoding="utf-8")
    m_type_file = open(M_type_file, "r", encoding="utf-8")
    m_made_country_file = open(M_made_country_file, "r", encoding="utf-8")
    m_language_file = open(M_language_file, "r", encoding="utf-8")
    m_show_time_file = open(M_show_time_file, "r", encoding="utf-8")
    m_duration_file = open(M_duration_file, "r", encoding="utf-8")
    m_alias_file = open(M_alias_file, "r", encoding="utf-8")
    m_imdb_file = open(M_imdb_file, "r", encoding="utf-8")
    m_title_file = open(M_title_file, "r", encoding="utf-8")
    m_synopsis_file = open(M_synopsis_file, "r", encoding="utf-8")
    m_actors_file = open(M_actors_file, "r", encoding="utf-8")

    m_director_list = [i.strip('\n').split(':')[-1] for i in m_director_file.readlines()]
    m_writer_list = [i.strip('\n').split(':')[-1] for i in m_writer_file.readlines()]
    m_type_list = [i.strip('\n').split(':')[-1] for i in m_type_file.readlines()]
    m_made_country_list = [i.strip('\n').split(':')[-1] for i in m_made_country_file.readlines()]
    m_language_list = [i.strip('\n').split(':')[-1] for i in m_language_file.readlines()]
    m_show_time_list = [i.strip('\n').split(':')[-1] for i in m_show_time_file.readlines()]
    m_duration_list = [i.strip('\n').split(':')[-1] for i in m_duration_file.readlines()]
    m_alias_list = [i.strip('\n').split(':')[-1] for i in m_alias_file.readlines()]
    m_imdb_list = [i.strip('\n').split(':')[-1] for i in m_imdb_file.readlines()]
    m_title_list = [i.strip('\n') for i in m_title_file.readlines()]
    m_id_list = [i.split(':')[0].strip() for i in m_title_list]
    m_title_list = [i.split(':')[-1].strip() for i in m_title_list]
    m_synopsis_list = [i.strip('\n').split(':')[-1] for i in m_synopsis_file.readlines()]
    m_actors_list = [i.strip('\n').split(':')[-1] for i in m_actors_file.readlines()]

    for i in range(len(m_title_list)):
        sheet.write(i + 1, 0, m_title_list[i])
        sheet.write(i + 1, 1, m_id_list[i])
        sheet.write(i + 1, 2, m_director_list[i])
        sheet.write(i + 1, 3, m_writer_list[i])
        sheet.write(i + 1, 4,m_type_list[i] )
        sheet.write(i + 1, 5, m_made_country_list[i])
        sheet.write(i + 1, 6, m_language_list[i])
        sheet.write(i + 1, 7, m_show_time_list[i])
        sheet.write(i + 1, 8, m_duration_list[i])
        sheet.write(i + 1, 9, m_alias_list[i])
        sheet.write(i + 1, 10, m_imdb_list[i])
        sheet.write(i + 1, 11, m_synopsis_list[i])
        sheet.write(i + 1, 12, m_actors_list[i])

    workbook.save('豆瓣电影.xls')

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

def book2csv():
    workbook = xlwt.Workbook(encoding="utf-8", style_compression=0)
    sheet = workbook.add_sheet('豆瓣书籍', cell_overwrite_ok=True)
    book_cols = ("书籍名", "ID", "作者", "出版社", "出版时间", "页数", "定价", "装帧", "ISBN", "内容简介", "作者介绍")

    for i in range(len(book_cols)):
        sheet.write(0, i, book_cols[i])

    b_writer_file = open(B_writer_file, "r", encoding="utf-8")
    b_publisher_file = open(B_publisher_file, "r", encoding="utf-8")
    b_publishtime_file = open(B_publishtime_file, "r", encoding="utf-8")
    b_pagenums_file = open(B_pagenums_file, "r", encoding="utf-8")
    b_price_file = open(B_price_file, "r", encoding="utf-8")
    b_binding_file = open(B_binding_file, "r", encoding="utf-8")
    b_isbn_file = open(B_isbn_file, "r", encoding="utf-8")
    b_title_file = open(B_title_file, "r", encoding="utf-8")
    b_synopsis_file = open(B_synopsis_file, "r", encoding="utf-8")
    b_writerinfo_file = open(B_writerinfo_file, "r", encoding="utf-8")

    b_writer_list = [i.strip('\n').split(':')[-1] for i in b_writer_file.readlines()]
    b_publisher_list = [i.strip('\n').split(':')[-1] for i in b_publisher_file.readlines()]
    b_publishtime_list = [i.strip('\n').split(':')[-1] for i in b_publishtime_file.readlines()]
    b_pagenums_list = [i.strip('\n').split(':')[-1] for i in b_pagenums_file.readlines()]
    b_price_list = [i.strip('\n').split(':')[-1] for i in b_price_file.readlines()]
    b_binding_list = [i.strip('\n').split(':')[-1] for i in b_binding_file.readlines()]
    b_isbn_list = [i.strip('\n').split(':')[-1] for i in b_isbn_file.readlines()]
    b_title_list = [i.strip('\n') for i in b_title_file.readlines()]
    b_id_list = [i.split(":")[0] for i in b_title_list]
    b_title_list = [i.split(":")[-1] for i in b_title_list]
    b_synopsis_list = [i.strip('\n').split(':')[-1] for i in b_synopsis_file.readlines()]
    b_writerinfo_list = [i.strip('\n').split(':')[-1] for i in b_writerinfo_file.readlines()]


    for i in range(len(b_title_list)):
        sheet.write(i + 1, 0, b_title_list[i])
        sheet.write(i + 1, 1, b_id_list[i])
        sheet.write(i + 1, 2, b_writer_list[i])

        sheet.write(i + 1, 3, b_publisher_list[i])
        sheet.write(i + 1, 4, b_publishtime_list[i])
        sheet.write(i + 1, 5, b_pagenums_list[i])
        sheet.write(i + 1, 6, b_price_list[i])
        sheet.write(i + 1, 7, b_binding_list[i])
        sheet.write(i + 1, 8, b_isbn_list[i])
        sheet.write(i + 1, 9, b_synopsis_list[i])
        sheet.write(i + 1, 10, b_writerinfo_list[i])

    workbook.save('豆瓣书籍.xls')

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


if __name__  ==  '__main__':

    movie2csv()
    book2csv()

