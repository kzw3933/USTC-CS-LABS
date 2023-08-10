# 创建数据库

create database  if not exists lab1;
use lab1;

# 创建Book表
create table if not exists Book(
    ID char(8) primary key,
    name varchar(10) not null,
    author varchar(10),
    price float,
    status int default 0,
    borrow_Times int default 0,
    reverse_Times int default 0
) character set utf8;

# 创建读者表
create table if not exists Reader(
    ID char(8) primary key,
    name varchar(10),
    age int,
    address varchar(20)
) character set utf8;

# 创建借阅表
create table if not exists Borrow(
    book_ID char(8),
    reader_ID char(8),
    borrow_Date date,
    return_Date date,
    primary key (book_ID, reader_ID, borrow_Date),
    foreign key (book_ID) references Book(ID),
    foreign key (reader_ID) references Reader(ID)
) character set utf8;

# 创建预约表
create table if not exists Reserve(
    book_ID char(8),
    reader_ID char(8),
    reserve_Date date default (curdate()),
    take_Date date,
    primary key (book_ID, reader_ID, reserve_Date),
    check (take_Date is null or take_Date > reserve_Date)
) character set utf8;



