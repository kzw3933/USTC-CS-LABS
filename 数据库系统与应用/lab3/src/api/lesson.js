import mysqlPool from '../model/db';

const lessonAPIs = async (app) => {
    app.get('/api/lesson/:tid', async (req, res) => {
        try {
            const { tid } = req.params;
            const [rows, fields] = await mysqlPool.query('select * from TeachCourse, Lesson where TeachCourse.LID = Lesson.LID and TeachCourse.TID = ?', [tid]);
            const data = rows.map((row) => {
                return {
                    lid: row.LID,
                    lname: row.Lname,
                    creditHours: row.credit_hours,
                    courseNature: row.course_nature,
                    teachYear: row.teach_year,
                    term: row.term,
                    undertakeCreditHours: row.undertake_credit_hours,
                }
            })
            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    app.get('/api/lesson/lesson/:lid', async (req, res) => {
        try {
            const { lid } = req.params;
            const [rows, fields] = await mysqlPool.query('select * from Lesson where LID = ?', [lid]);
            const data = {
                lid: rows[0].LID,
                lname: rows[0].Lname,
                creditHours: rows[0].credit_hours,
                courseNature: rows[0].course_nature,
                };

            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    app.get('/api/lesson/:tid/:lid', async (req, res) => {
        try {
            const { tid, lid } = req.params;
            const [rows, fields] = await mysqlPool.query('select * from TeachCourse, Lesson where TeachCourse.LID = Lesson.LID and TID = ? and TeachCourse.LID = ?', [tid, lid]);
            const data = {
                    lid: rows[0].LID,
                    lname: rows[0].Lname,
                    creditHours: rows[0].credit_hours,
                    courseNature: rows[0].course_nature,
                    teachYear: rows[0].teach_year,
                    term: rows[0].term,
                    undertakeCreditHours: rows[0].undertake_credit_hours,
                };

            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    app.post('/api/lesson/search', async (req, res) => {
        try {
            const {
                lid, lname, creditHours, courseNature, teachYear, term, undertakeCreditHours, tid
            } = req.body;
            let sql = "select * from TeachCourse, Lesson where TeachCourse.LID = Lesson.LID "
            let sql_array = [];
            if(lid) {
                sql += "and Lesson.LID = ? ";
                sql_array.push(lid);
            }
            if(lname) {
                sql += "and Lesson.Lname = ? ";
                sql_array.push(lname);
            }
            if(creditHours) {
                sql += "and Lesson.credit_hours = ? ";
                sql_array.push(creditHours);
            }
            if(courseNature) {
                sql += "and Lesson.course_nature = ? ";
                sql_array.push(courseNature);
            }
            if(teachYear) {
                sql += "and TeachCourse.teach_year = ? ";
                sql_array.push(teachYear);
            }
            if(term) {
                sql += "and TeachCourse.term = ? ";
                sql_array.push(lid);
            }
            if(undertakeCreditHours) {
                sql += "and TeachCourse.undertake_credit_hours = ? ";
                sql_array.push(undertakeCreditHours);
            }
            if(tid) {
                sql += "and TeachCourse.TID = ? ";
                sql_array.push(tid);
            }
            const [rows, fields] = await mysqlPool.query(sql, sql_array);
            const data = rows.map((row) => {
                return {
                    tid: row.TID,
                    lid: row.LID,
                    lname: row.Lname,
                    creditHours: row.credit_hours,
                    courseNature: row.course_nature,
                    teachYear: row.teach_year,
                    term: row.term,
                    undertakeCreditHours: row.undertake_credit_hours,
                }
            })
            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    

    app.post('/api/lesson', async (req, res) => {
        try {
            const {
                lid,
                lname,
                creditHours,
                courseNature,
                teachYear,  
                term,
                undertakeCreditHours,
                tid
            } = req.body;

            const [rows, fields] = await mysqlPool.query('select * from Lesson where LID = ?', [lid]);

            if(rows.length > 0) {
                throw new Error('数据库已存在该课程,不支持普通用户新增涉及多位教师的课程相关数据')
            }

            await mysqlPool.query('insert into Lesson(LID, Lname, credit_hours, course_nature) VALUES (?, ?, ?, ?)',
                                [lid, lname, creditHours, courseNature]);
            await mysqlPool.query('insert into TeachCourse(TID, LID, teach_year, term, undertake_credit_hours) VALUES (?, ?, ?, ?, ?)',
                                [tid, lid, teachYear, term, undertakeCreditHours]);

            return res.json({
                message: "新增课程成功"
            })

        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    app.post('/api/lesson/summary/:tid', async (req, res) => {
        try {

            const { tid } = req.params;
            const {startYear, endYear} = req.body;

            let sql = "select * from TeachCourse, Lesson where TeachCourse.LID = Lesson.LID and TeachCourse.TID = ? "
            let sql_array = [tid];
            
            if(startYear && endYear) {
                sql += `and teach_year >= ? and teach_year <= ? `;
                sql_array.push(startYear);
                sql_array.push(endYear);
            } else if(startYear) {
                sql += `and teach_year >= ? `;
                sql_array.push(startYear);
            } else if(endYear) {
                sql += `and teach_year <= ? `;
                sql_array.push(endYear);
            }

            const [rows, fields] = await mysqlPool.query(sql, sql_array);

            const data = rows.map((row) => {
                return {
                    lid: row.LID,
                    lname: row.Lname,
                    creditHours: row.credit_hours,
                    courseNature: row.course_nature,
                    teachYear: row.teach_year,
                    term: row.term,
                    undertakeCreditHours: row.undertake_credit_hours,
                }
            })
            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    app.post('/api/lesson/admin', async (req, res) => {
        try {
           const { postLessons } = req.body;


            const [rows, fields] = await mysqlPool.query('select * from Lesson where LID = ?', [postLessons[0].lid]);

            if(rows.length === 0) {
                await mysqlPool.query('insert into Lesson(LID, Lname, credit_hours, course_nature) VALUES (?, ?, ?, ?)',
                [postLessons[0].lid, postLessons[0].lname, postLessons[0].creditHours, postLessons[0].courseNature]);
            }

            await Promise.all(postLessons.map(async element => {
                await mysqlPool.query('insert into TeachCourse(TID, LID, teach_year, term, undertake_credit_hours) VALUES (?, ?, ?, ?, ?)',
                                [element.tid, element.lid, element.teachYear, element.term, element.undertakeCreditHours]);
            }));

            return res.json({
                message: "新增课程成功"
            })

        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    app.patch('/api/lesson', async (req, res) => {
        try {
            const {
                modifyLid,
                lname,
                creditHours,
                courseNature,
                teachYear,  
                term,
                undertakeCreditHours,
                tid
            } = req.body;

            const [rows, fields] = await mysqlPool.query('select * from TeachCourse where LID = ?', [modifyLid]);

            if(rows.length > 1) {
                throw new Error('不支持普通用户修改涉及多位教师的课程相关数据');
            }

            await mysqlPool.query('update Lesson set Lname = ?, credit_hours = ?, course_nature = ? where LID = ?',
                                [lname, creditHours, courseNature, modifyLid]);

            await mysqlPool.query('update TeachCourse set teach_year = ?, term = ?, undertake_credit_hours = ? where TID = ? and LID = ?',
                                [teachYear, term, undertakeCreditHours, tid, modifyLid]);
            return res.json({
                    message: "修改课程信息成功"
            });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    app.patch('/api/lesson/admin', async (req, res) => {
        try {
            const { modifyLessons, modifyTids } = req.body;

            const [rows, fields] = await mysqlPool.query('select * from TeachCourse where LID = ?', [modifyLessons[0].modifyLid]);

            let totalUndertakeCreditHours = 0;

            if(rows.length !== 0) {
                await Promise.all(rows.map(async row => {
                    if(! modifyTids.includes(row.TID)) {
                        totalUndertakeCreditHours += parseInt(row.undertake_credit_hours);
                    }
                  }));
            }

            await Promise.all(modifyLessons.map(async element => {
                totalUndertakeCreditHours += parseInt(element.undertakeCreditHours);
              }));  

            if(totalUndertakeCreditHours !== parseInt(modifyLessons[0].creditHours)) {
                throw new Error("所有教师承担课程学时总和不等于课程总学时");
            }

            await mysqlPool.query('update Lesson set Lname = ?, credit_hours = ?, course_nature = ? where LID = ?',
                                [modifyLessons[0].lname, modifyLessons[0].creditHours, modifyLessons[0].courseNature, modifyLessons[0].modifyLid]);
            
            await Promise.all(modifyLessons.map(async element => {
                await mysqlPool.query('update TeachCourse set teach_year = ?, term = ?, undertake_credit_hours = ? where TID = ? and LID = ?',
                                [element.teachYear, element.term, element.undertakeCreditHours, element.modifyTid, element.modifyLid]);
            })); 
            
            return res.json({
                    message: "修改课程信息成功"
            });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    app.delete('/api/lesson', async (req, res) => {
        try {
            const { deleteLid, tid } = req.body;

            const [rows, fields] = await mysqlPool.query('select * from TeachCourse where LID = ?', [deleteLid]);

            if(rows.length > 1) {
                throw new Error('不支持普通用户删除涉及多位教师的课程相关数据');
            }

            await mysqlPool.query('delete from TeachCourse where TID = ? and LID = ?',[tid, deleteLid]);
            await mysqlPool.query('delete from Lesson where LID = ?',[deleteLid]);

            return res.json({
                message: "删除课程成功"
            })
        } catch(err) {
            return res.status(400).json({
                message: err.message
            });
        }
    }) 

    app.delete('/api/lesson/admin', async (req, res) => {
        try {
            const { deleteLid } = req.body;

            await mysqlPool.query('delete from TeachCourse where LID = ?',[deleteLid]);
            await mysqlPool.query('delete from Lesson where LID = ?',[deleteLid]);

            return res.json({
                message: "删除课程成功"
            })
        } catch(err) {
            return res.status(400).json({
                message: err.message
            });
        }
    }) 
}

export default lessonAPIs;