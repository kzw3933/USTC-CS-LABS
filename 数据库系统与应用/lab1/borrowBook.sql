delimiter //
create procedure borrowBook(IN reader_id char(8), IN book_id char(8))
begin

    declare book_can_be_borrowed bool default false;
    declare today_have_borrow_this_book bool default false;
    declare book_have_reserved bool default false;
    declare reader_have_borrowed_too_many_books bool default false;
    declare reader_have_reserved_the_book bool default false;

    select (case status when 1 then false else true end) from Book
    where ID = book_id
        into book_can_be_borrowed;

    # 同一天不允许同一个读者重复借阅同一本书
    if exists (
        select * from Borrow 
        where Borrow.reader_ID = reader_id and day(Borrow.borrow_Date) = day(curdate()) and Borrow.book_ID = book_id 
    ) then
        set today_have_borrow_this_book = true;
    end if;
    
    # 如果该图书存在预约记录，而当前借阅者没有预约，则不允许借阅
    if exists (
        select * from Reserve 
        where Reserve.book_ID = book_id
    ) then
        set book_have_reserved = true;
    end if;

    # 一个读者最多只能借阅 3 本图书，意味着如果读者已经借阅了 3 本图书并且未归还则不允许再借书
    
    select count(*) >= 3 from Borrow
    where return_Date is null and Borrow.reader_ID = reader_id
        into reader_have_borrowed_too_many_books;
    
    # 如果借阅者已经预约了该图书，则允许借阅，但要求借阅完成后删除借阅者对该图书的预约记录
    if exists (
        select * from Reserve
        where reader_ID = reader_id and book_ID = book_id
    ) then
        set reader_have_reserved_the_book = true;
    end if;

    # 借阅成功后图书表中的 times 加 1 
    # 借阅成功后修改 status

    if not book_can_be_borrowed then
        select "The book can't be borrowed now" as message;
    elseif today_have_borrow_this_book then
        select "One reader can't borrow the same book in one day" as message;
    elseif reader_have_borrowed_too_many_books then
        select "One reader can't borrow more than 3 book which is not returned" as message;
    elseif book_have_reserved and not reader_have_reserved_the_book then
        select "The book has reserved by some other reader" as message;
    elseif reader_have_reserved_the_book then
        update Book
        set status = 1, borrow_Times = borrow_Times + 1
        where Book.ID = book_id;  
        
        insert into Borrow
        value(book_id, reader_id, curdate(), null);
                    
        delete from Reserve
        where Reserve.book_ID = book_id and Reserve.reader_ID = reader_id;  
    else
        insert into Borrow
        value(book_id, reader_id, curdate(), null);

        update Book
        set status = 1, borrow_Times = borrow_Times + 1
        where Book.ID = book_id;
    end if;
end //
delimiter ;

SET SQL_SAFE_UPDATES = false;
call borrowBook('r14', 'b18');
select * from borrow 
where reader_ID = 'r14'; 
drop procedure borrowBook;

