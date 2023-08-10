delimiter //
create procedure updateReaderID(IN old_id char(8), IN new_id char(8))
begin

    declare old_id_exist, new_id_exist bool;
    declare exit handler for sqlstate '43000' select "old_id not exist or new_id has existed in Reader table" as message;

    select (count(*) > 0) as exist from Reader 
    where ID = old_id 
        into old_id_exist;

    select (count(*) > 0) as exist from Reader 
    where ID = new_id 
        into new_id_exist;
    
    if old_id_exist and not new_id_exist then

        # 迁移Borrow表中相关数据迁移到临时表
        create table borrow_backup(
            book_ID char(8),
            reader_ID char(8),
            borrow_Date date,
            return_Date date,
            primary key (book_ID, borrow_Date)
        );

        insert into borrow_backup
            select * from Borrow 
            where Borrow.reader_ID = old_id;

        delete from Borrow where Borrow.reader_ID = old_id;   

        # 更新Reader表及临时表
        update Reader 
        set ID = new_id
        where ID = old_id;
        
        update borrow_backup
        set reader_ID = new_id;

        # 将临时表中的数据迁移回Borrow表
        insert into Borrow
            select * from borrow_backup;

        # 删除临时表
        drop table borrow_backup;

    else 
        -- select "old_id not exist or new_id has existed in Reader table" as message;
        signal sqlstate '43000';
    end if;
end //
delimiter ;

SET SQL_SAFE_UPDATES = false;
call updateReaderID('r1', 'r100');

select * from reader
where ID = 'r100';

select * from borrow
where reader_ID = 'r100';

drop procedure updateReaderID;





