delimiter //
create trigger lab_trigger1
after insert on Reserve for each row
begin
    update Book 
    set status = 2, reserve_Times = reserve_Times + 1
    where Book.ID = new.book_ID 
end
delimiter ;

delimiter //
create trigger lab_trigger2
after delete on Reserve for each row
begin
    update Book 
    set reserve_Times = reserve_Times - 1
    where Book.ID = old.book_ID
end
delimiter ;
