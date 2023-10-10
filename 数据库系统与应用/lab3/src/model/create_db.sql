create database if not exists lab3;
use lab3;

# 创建论文表
create table if not exists Paper(
    SN integer primary key,
    title varchar(256) not null,
    public_source varchar(256) not null,
    publication_year integer,
    public_type integer check(public_type >= 1 and public_type <= 4),
    public_rank integer check(public_rank >= 1 and public_rank <= 6)
) character set utf8;

# 创建教师表
create table if not exists Teacher(
    TID varchar(5) primary key,
    Tname varchar(256) not null,
    gender integer not null check(gender >= 1 and gender <= 2),
    job integer not null check(job >= 0 and job <= 11),
    admin boolean default false,
    password varchar(255) not null,
    token varchar(255),
    avatar varchar(255)
) character set utf8;

# 创建课程表
create table if not exists Lesson(
    LID varchar(256) primary key,
    Lname varchar(256) not null,
    credit_hours integer,
    course_nature integer check(course_nature >= 1 and course_nature <= 2)
) character set utf8;

# 创建项目表
create table if not exists Project(
    PID varchar(256) primary key,
    Pname   varchar(256) not null,
    project_source varchar(256),
    project_type integer check(project_type >= 1 and project_type <= 5),
    total_budget float,
    start_year integer,
    end_year integer
) character set utf8;

# 创建发表论文表
create table if not exists PublishPaper(
    SN integer,
    TID varchar(5),
    paper_rank integer,
    is_corresponding_author boolean,
    primary key (SN, TID),
    foreign key (SN) references Paper(SN),
    foreign key (TID) references Teacher(TID)
) character set utf8;

# 创建承担项目表
create table if not exists UndertakeProject(
    PID varchar(256),
    TID varchar(5),
    project_rank integer,
    undertake_funding float,
    primary key (PID, TID),
    foreign key (TID) references Teacher(TID),
    foreign key (PID) references Project(PID)
) character set utf8;

# 创建主讲课程表
create table if not exists TeachCourse(
    LID varchar(256),
    TID varchar(5),
    teach_year integer,
    term integer check(term >= 1 and term <= 3),
    undertake_credit_hours integer,
    primary key (LID, TID),
    foreign key (TID) references Teacher(TID),
    foreign key (LID) references Lesson(LID)
) character set utf8;