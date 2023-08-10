# 查询读者 Rose 借过的读书（包括已还和未还）的图书号、书名和借期

select Book.ID, Book.name, Borrow.borrow_Date from Borrow, Book, Reader
where Borrow.book_ID = Book.ID and Borrow.reader_ID = Reader.ID and Reader.name = 'Rose';

# 查询从没有借过图书也从没有预约过图书的读者号和读者姓名

select Reader.ID, Reader.name from Reader 
where not exists (
    select * from Borrow 
    where Borrow.reader_ID = ID
) and not exists (
    select * from Reserve 
    where Reserve.reader_ID = ID
);

# 查询被借阅次数最多的作者（注意一个作者可能写了多本书）
select author from (
	select Book.author as author, SUM(Book.borrow_Times) as borrow_times_sum from Book
    group by Book.author
) Author
where borrow_times_sum = (
	select MAX(borrow_times_sum) from (
		select Book.author, SUM(Book.borrow_Times) as borrow_times_sum from Book
		group by Book.author
	) Author
);


# 查询目前借阅未还的书名中包含“MySQL”的的图书号和书名
select Book.ID, Book.name from Book
where Book.name like '%MySQL%' and exists (
    select * from Borrow
    where book_ID = Book.ID and return_Date is null 
);


# 查询借阅图书数目超过 10 本的读者姓名
select Reader.name from Reader 
where (
    select COUNT(*) from Borrow
    where reader_ID = Reader.ID
) > 10;

# 查询没有借阅过任何一本 John 所著的图书的读者号和姓名
select Reader.ID, Reader.name from Reader
where not exists (
    select * from Borrow, Book
    where Borrow.book_ID = Book.ID and Borrow.reader_ID = Reader.ID and Book.author = 'John'
);


# 查询 2022 年借阅图书数目排名前 10 名的读者号、姓名以及借阅图书数
select Reader.ID, Reader.name, COUNT(*) as reader_borrow_times from Reader, Book, Borrow
where Reader.ID = Borrow.reader_ID and Book.ID = Borrow.book_ID and Borrow.borrow_Date >= '2022-01-01' and Borrow.borrow_Date < '2023-01-01'
group by Reader.ID, Reader.name
order by reader_borrow_times desc
limit 10;


# 创建一个读者借书信息的视图,该视图包含读者号、姓名、所借图书号、图书名和借期；并使用该视图查询最近一年所有读者的读者号以及所借阅的不同图书数
create view borrow_view(reader_id, reader_name, book_id, book_name, borrow_time) 
as select Reader.ID, Reader.name, Book.ID, Book.name, Borrow.borrow_Date from Book, Borrow, Reader
    where Book.ID = Borrow.book_ID and Reader.ID = Borrow.reader_id;

select reader_id, count(distinct book_id) from borrow_view
where borrow_time >= date_sub(curdate(), interval 1 year)
group by reader_id;

