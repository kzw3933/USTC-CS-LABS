delimiter //
create procedure returnBook(IN reader_id char(8), IN book_id char(8))
begin

    if not exists (
        select * from Borrow
        where Borrow.book_ID = book_id and Borrow.reader_ID = reader_id and Borrow.return_Date is null
    ) then
        select "you can not return the book" as message;
    else
        update Book
        set status = 0
        where ID = book_id;

        update Borrow
        set return_Date = curdate()
        where Borrow.book_ID = book_id and Borrow.reader_ID = reader_id and Borrow.return_Date is null;
    end if;
end //
delimiter ;

call returnBook('r14', 'b18');
drop procedure returnBook;

select * from Borrow
where reader_ID = 'r14';


