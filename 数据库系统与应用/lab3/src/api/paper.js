import mysqlPool from '../model/db';

const paperAPIs = async (app) => {
    app.get('/api/paper/:tid', async (req, res) => {
        try {
            const { tid } = req.params;
            const [rows, fields] = await mysqlPool.query('select * from PublishPaper, Paper where PublishPaper.SN = Paper.SN and TID = ?', [tid]);
            const data = rows.map((row) => {
                return {
                    sn: row.SN,
                    title: row.title,
                    publicSource: row.public_source,
                    publicationYear: row.publication_year,
                    publicType: row.public_type,
                    publicRank: row.public_rank,
                    paperRank: row.paper_rank,
                    isCorrespondingAuthor: row.is_corresponding_author
                }
            })
            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    app.get('/api/paper/paper/:sn', async (req, res) => {
        try {
            const { sn } = req.params;
            const [rows, fields] = await mysqlPool.query('select * from Paper where SN = ?', [sn]);
            
            const data = {
                sn: rows[0].SN,
                title: rows[0].title,
                publicSource: rows[0].public_source,
                publicationYear: rows[0].publication_year,
                publicType: rows[0].public_type,
                publicRank: rows[0].public_rank,
            };
            
            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    app.get('/api/paper/:tid/:sn', async (req, res) => {
        try {
            const { tid, sn } = req.params;
            const [rows, fields] = await mysqlPool.query('select * from PublishPaper, Paper  where PublishPaper.SN = Paper.SN and TID = ? and PublishPaper.SN = ?', [tid, sn]);

            const data = {
                    sn: rows[0].SN,
                    title: rows[0].title,
                    publicSource: rows[0].public_source,
                    publicationYear: rows[0].publication_year,
                    publicType: rows[0].public_type,
                    publicRank: rows[0].public_rank,
                    paperRank: rows[0].paper_rank,
                    isCorrespondingAuthor: rows[0].is_corresponding_author
                };
            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })


    app.post('/api/paper/search', async (req, res) => {
        try {
            const {
                sn, title, publicSource, publicationYear, publicType, publicRank, paperRank, isCorrespondingAuthor, tid
            } = req.body;
            let sql = "select * from PublishPaper, Paper where PublishPaper.SN = Paper.SN "
            let sql_array = [];
            if(sn) {
                sql += "and Paper.SN = ? ";
                sql_array.push(sn);
            }
            if(title) {
                sql += "and Paper.title = ? ";
                sql_array.push(title);
            }
            if(publicSource) {
                sql += "and Paper.public_source = ? ";
                sql_array.push(publicSource);
            }
            if(publicationYear) {
                sql += "and Paper.publication_year = ? ";
                sql_array.push(publicationYear);
            }
            if(publicType) {
                sql += "and Paper.public_type = ? ";
                sql_array.push(publicType);
            }
            if(publicRank) {
                sql += "and Paper.public_rank = ? ";
                sql_array.push(publicRank);
            }
            if(paperRank) {
                sql += "and PublishPaper.paper_rank = ? ";
                sql_array.push(paperRank);
            }
            if(isCorrespondingAuthor) {
                sql += "and PublishPaper.is_corresponding_author = ? ";
                sql_array.push(isCorrespondingAuthor);
            }
            if(tid) {
                sql += "and PublishPaper.TID = ? ";
                sql_array.push(tid);
            }
            const [rows, fields] = await mysqlPool.query(sql, sql_array);
            const data = rows.map((row) => {
                return {
                    tid: row.TID,
                    sn: row.SN,
                    title: row.title,
                    publicSource: row.public_source,
                    publicationYear: row.publication_year,
                    publicType: row.public_type,
                    publicRank: row.public_rank,
                    paperRank: row.paper_rank,
                    isCorrespondingAuthor: row.is_corresponding_author
                }
            })
            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    
    app.post('/api/paper', async (req, res) => {
        try {
            const {
                sn,
                title,
                publicSource,
                publicationYear,
                publicType,  
                publicRank,
                paperRank,
                isCorrespondingAuthor,
                tid
            } = req.body;

            const [rows, fields] = await mysqlPool.query('select * from Paper where SN = ?', [sn]);

            if(rows.length > 0) {
                throw new Error('数据库已存在该论文,不支持普通用户新增涉及多位教师的论文相关数据')
            }

            await mysqlPool.query('insert into Paper(SN, title, public_source, publication_year, public_type, public_rank) VALUES (?, ?, ?, ?, ?, ?)',
                                [sn, title, publicSource, publicationYear, publicType, publicRank]);
            
            
            await mysqlPool.query('insert into PublishPaper(TID, SN, paper_rank, is_corresponding_author) VALUES (?, ?, ?, ?)',
                                [tid, sn, paperRank, isCorrespondingAuthor]);

            return res.json({
                    message: "新增论文成功"
            })

        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    

    app.post('/api/paper/summary/:tid', async (req, res) => {
        try {
            const { tid } = req.params;
            const {startYear, endYear} = req.body;
            let sql = "select * from PublishPaper, Paper where PublishPaper.SN = Paper.SN and PublishPaper.TID = ? "
            let sql_array = [tid];
            if(startYear && endYear) {
                sql += `and publication_year >= ? and publication_year <= ? `;
                sql_array.push(startYear);
                sql_array.push(endYear);
            } else if(startYear) {
                sql += `and publication_year >= ? `;
                sql_array.push(startYear);
            } else if(endYear) {
                sql += `and publication_year <= ? `;
                sql_array.push(endYear);
            }

            const [rows, fields] = await mysqlPool.query(sql, sql_array);
            const data = rows.map((row) => {
                return {
                    sn: row.SN,
                    title: row.title,
                    publicSource: row.public_source,
                    publicationYear: row.publication_year,
                    publicType: row.public_type,
                    publicRank: row.public_rank,
                    paperRank: row.paper_rank,
                    isCorrespondingAuthor: row.is_corresponding_author
                }
            })
            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    app.post('/api/paper/admin', async (req, res) => {
        try {
            const { postPapers, postTids } = req.body;

            const [rows, fields] = await mysqlPool.query('select * from Paper where SN = ?', [postPapers[0].sn]);
            const [tmp_rows, tmp_fields] = await mysqlPool.query('select * from PublishPaper where SN = ?', postPapers[0].sn);

            if(tmp_rows.length !== 0) {
                await Promise.all(postPapers.map(async element => {
                    if (parseInt(element.isCorrespondingAuthor) && tmp_rows.some(row => row.is_corresponding_author && ! postTids.includes(row.TID))) {
                        throw new Error("该论文已存在其他通讯作者");
                    }
                
                    if (tmp_rows.some(row => row.paper_rank === parseInt(element.paperRank) && ! postTids.includes(row.TID))) {
                        throw new Error("该论文已存在与你排名一样的作者");
                    }
                }));
            }

            if(rows.length === 0) {
                await mysqlPool.query('insert into Paper (SN, title, public_source, publication_year, public_type, public_rank) VALUES (?, ?, ?, ?, ?, ?)',
                [postPapers[0].sn, postPapers[0].title, postPapers[0].publicSource, postPapers[0].publicationYear, postPapers[0].publicType, postPapers[0].publicRank]);
            }

            await Promise.all(postPapers.map(async element => {
                await mysqlPool.query('insert into PublishPaper (TID, SN, paper_rank, is_corresponding_author) VALUES (?, ?, ?, ?)',
                  [element.tid, element.sn, element.paperRank, element.isCorrespondingAuthor]);
            }));
          
            return res.json({
              message: "新增论文信息成功"
            });
          } catch (err) {
            return res.status(400).json({
              message: err.message
            });
          }
    });

    app.patch('/api/paper', async (req, res) => {
        try {
            const {
                modifySN,
                title,
                publicSource,
                publicationYear,
                publicType,  
                publicRank,
                paperRank,
                isCorrespondingAuthor,
                tid
            } = req.body;

            const [rows, fields] = await mysqlPool.query('select * from PublishPaper where SN = ?', [modifySN]);

            if(rows.length > 1) {
                throw new Error('不支持普通用户修改涉及多位教师的论文相关数据');
            }

            await mysqlPool.query('update Paper set title = ?, public_source = ?, publication_year = ?, public_type = ?, public_rank = ? where SN = ?',
                                [title, publicSource, publicationYear, publicType, publicRank, modifySN]);

            await mysqlPool.query('update PublishPaper set paper_rank = ?, is_corresponding_author = ? where TID = ? and SN = ?',
                                [paperRank, isCorrespondingAuthor, tid, modifySN]);
            return res.json({
                    message: "修改论文信息成功"
            });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    app.patch('/api/paper/admin', async (req, res) => {
        try {
            const {
                modifyPapers,
                modifyTids
            } = req.body;

            const [tmp_rows, tmp_fields] =  await mysqlPool.query('select * from PublishPaper where SN = ?', modifyPapers[0].modifySN);

            await Promise.all(modifyPapers.map(async element => {
                if (parseInt(element.isCorrespondingAuthor)) {
                  if (tmp_rows.some(row => row.is_corresponding_author && !modifyTids.includes(row.TID))) {
                    throw new Error("该论文已存在其他通讯作者");
                  }
                }
              
                if (tmp_rows.some(row => row.paper_rank === parseInt(element.paperRank) && !modifyTids.includes(row.TID))) {
                  throw new Error("该论文已存在与你排名一样的作者");
                }
              }));

            await mysqlPool.query(
                'update Paper set title = ?, public_source = ?, publication_year = ?, public_type = ?, public_rank = ? where SN = ?',
                [modifyPapers[0].title, modifyPapers[0].publicSource, modifyPapers[0].publicationYear, modifyPapers[0].publicType, modifyPapers[0].publicRank, modifyPapers[0].modifySN]
            );
              
            await Promise.all(modifyPapers.map(async element => {              
                await mysqlPool.query(
                  'update PublishPaper set paper_rank = ?, is_corresponding_author = ? where TID = ? and SN = ?',
                  [element.paperRank, element.isCorrespondingAuthor, element.modifyTid, element.modifySN]
                );
              }));

            return res.json({
                    message: "修改论文信息成功"
            });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    app.delete('/api/paper', async (req, res) => {
        try {
            const { deleteSN, tid } = req.body;

            const [rows, fields] = await mysqlPool.query('select * from PublishPaper where SN = ?', [deleteSN]);

            if(rows.length > 1) {
                throw new Error('不支持普通用户删除涉及多位教师的论文相关数据');
            }

            await mysqlPool.query('delete from PublishPaper where SN = ? and TID = ?',[deleteSN, tid]);
            
            await mysqlPool.query('delete from Paper where SN = ?',[deleteSN]);

            return res.json({
                message: "删除论文成功"
            })
        } catch(err) {
            return res.status(400).json({
                message: err.message
            });
        }
    }) 

    app.delete('/api/paper/admin', async (req, res) => {
        try {
            const { deletePapers } = req.body;

            await Promise.all(deletePapers.map(async element => {
                await mysqlPool.query('delete from PublishPaper where SN = ? and TID = ?', [element.deleteSN, element.deleteTid]);
              }));
              
            await mysqlPool.query('delete from Paper where SN = ?', [deletePapers[0].deleteSN]);

            return res.json({
                message: "删除论文成功"
            })
        } catch(err) {
            return res.status(400).json({
                message: err.message
            });
        }
    }) 
}

export default paperAPIs;