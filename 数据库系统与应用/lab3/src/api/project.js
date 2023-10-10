import mysqlPool from '../model/db';

const projectAPIs = async (app) => {
    app.get('/api/project/:tid', async (req, res) => {
        try {
            const { tid } = req.params;
            const [rows, fields] = await mysqlPool.query('select * from UndertakeProject where TID = ?', [tid]);
            const data = rows.map((row) => {
                return {
                    pid: row.PID,
                    pname: row.Pname,
                    projectSource: row.project_source,
                    projectType: row.project_type,
                    totalBudget: row.total_budget,
                    startYear: row.start_year,
                    endYear: row.end_year,
                    projectRank: row.project_rank,
                    undertakeFunding: row.undertake_funding
                }
            })
            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    app.get('/api/project/project/:pid', async (req, res) => {
        try {
            const { pid } = req.params;
            const [rows, fields] = await mysqlPool.query('select * from Project where PID = ?', [pid]);
            const data = {
                    pid: rows[0].PID,
                    pname: rows[0].Pname,
                    projectSource: rows[0].project_source,
                    projectType: rows[0].project_type,
                    totalBudget: rows[0].total_budget,
                    startYear: rows[0].start_year,
                    endYear: rows[0].end_year,
                };

            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    app.get('/api/project/:tid/:pid', async (req, res) => {
        try {
            const { tid, pid } = req.params;
            const [rows, fields] = await mysqlPool.query('select * from UndertakeProject, Project where UndertakeProject.PID = Project.PID and TID = ? and UndertakeProject.PID = ?', [tid, pid]);
            const data = {
                    pid: rows[0].PID,
                    pname: rows[0].Pname,
                    projectSource: rows[0].project_source,
                    projectType: rows[0].project_type,
                    totalBudget: rows[0].total_budget,
                    startYear: rows[0].start_year,
                    endYear: rows[0].end_year,
                    projectRank: rows[0].project_rank,
                    undertakeFunding: rows[0].undertake_funding
                };

            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    app.post('/api/project/search', async (req, res) => {
        try {
            const {
                pid, pname, projectSource, projectType, totalBudget, startYear, endYear, projectRank, undertakeFunding, tid
            } = req.body;
            let sql = "select * from UndertakeProject, Project where UndertakeProject.PID = Project.PID "
            let sql_array = [];
            if(pid) {
                sql += "and Project.PID = ? ";
                sql_array.push(pid);
            }
            if(pname) {
                sql += "and Project.Pname = ? ";
                sql_array.push(pname);
            }
            if(projectSource) {
                sql += "and Project.project_source = ? ";
                sql_array.push(projectSource);
            }
            if(projectType) {
                sql += "and Project.project_type = ? ";
                sql_array.push(projectType);
            }
            if(totalBudget) {
                sql += "and Project.total_budget = ? ";
                sql_array.push(totalBudget);
            }
            if(startYear) {
                sql += "and Project.start_year = ? ";
                sql_array.push(startYear);
            }
            if(endYear) {
                sql += "and Project.end_year = ? ";
                sql_array.push(endYear);
            }
            if(projectRank) {
                sql += "and UndertakeProject.project_rank = ? ";
                sql_array.push(projectRank);
            }
            if(undertakeFunding) {
                sql += "and UndertakeProject.undertake_funding = ? ";
                sql_array.push(undertakeFunding);
            }
            if(tid) {
                sql += "and UndertakeProject.TID = ? ";
                sql_array.push(tid);
            }
            const [rows, fields] = await mysqlPool.query(sql, sql_array);
            const data = rows.map((row) => {
                return {
                    tid: row.TID,
                    pid: row.PID,
                    pname: row.Pname,
                    projectSource: row.project_source,
                    projectType: row.project_type,
                    totalBudget: row.total_budget,
                    startYear: row.start_year,
                    endYear: row.end_year,
                    projectRank: row.project_rank,
                    undertakeFunding: row.undertake_funding
                }
            })
            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    

    app.post('/api/project', async (req, res) => {
        try {
            const {
                pid,
                pname,
                projectSource,
                projectType,
                totalBudget,  
                startYear,
                endYear,
                projectRank,
                undertakeFunding,
                tid
            } = req.body;

            const [rows, fields] = await mysqlPool.query('select * from Project where PID = ?', [pid]);

            if(rows.length > 0) {
                throw new Error('数据库已存在该项目,不支持普通用户新增涉及多位教师的项目相关数据')
            }


            await mysqlPool.query('insert into Project(PID, Pname, project_source, project_type, total_budget, start_year, end_year) VALUES (?, ?, ?, ?, ?, ?, ?)',
                                [pid, pname, projectSource, projectType, totalBudget, startYear, endYear]);
            await mysqlPool.query('insert into UndertakeProject(TID, PID, project_rank, undertake_funding) VALUES (?, ?, ?, ?)',
                                [tid, pid, projectRank, undertakeFunding]);

            return res.json({
                message: "新增项目成功"
            })

        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    app.post('/api/project/summary/:tid', async (req, res) => {
        try {
            const { tid } = req.params;
            const {startYear, endYear} = req.body;
            let sql = "select * from UndertakeProject, Project where UndertakeProject.PID = Project.PID and TID = ? "
            let sql_array = [tid];

            if(startYear && endYear) {
                sql += `and start_year >= ? and start_year <= ? `;
                sql_array.push(startYear);
                sql_array.push(endYear);
            } else if(startYear) {
                sql += `and start_year >= ? `;
                sql_array.push(startYear);
            } else if(endYear) {
                sql += `and start_year <= ? `;
                sql_array.push(endYear);
            }

            const [rows, fields] = await mysqlPool.query(sql, sql_array);
            
            const data = rows.map((row) => {
                return {
                    pid: row.PID,
                    pname: row.Pname,
                    projectSource: row.project_source,
                    projectType: row.project_type,
                    totalBudget: row.total_budget,
                    startYear: row.start_year,
                    endYear: row.end_year,
                    projectRank: row.project_rank,
                    undertakeFunding: row.undertake_funding
                }
            })
            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    app.post('/api/project/admin', async (req, res) => {
        try {
            const {postProjects} = req.body;

            const [rows, fields] = await mysqlPool.query('select * from Project where PID = ?', [postProjects[0].pid]);
            const [tmp_rows, tmp_fields] = await mysqlPool.query('select * from UndertakeProject where PID = ?',  [postProjects[0].pid]);

            if(rows.length === 0) {
                await mysqlPool.query('insert into Project(PID, Pname, project_source, project_type, total_budget, start_year, end_year) VALUES (?, ?, ?, ?, ?, ?, ?)',
                                [postProjects[0].pid, postProjects[0].pname, postProjects[0].projectSource, postProjects[0].projectType, postProjects[0].totalBudget, postProjects[0].startYear, postProjects[0].endYear]);
            }

            await Promise.all(postProjects.map(async element => {
                await mysqlPool.query('insert into UndertakeProject(TID, PID, project_rank, undertake_funding) VALUES (?, ?, ?, ?)',
                                [element.tid, element.pid, element.projectRank, element.undertakeFunding]);
            }));

            return res.json({
                message: "新增项目成功"
            })

        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    app.patch('/api/project', async (req, res) => {
        try {
            const {
                modifyPid,
                pname,
                projectSource,
                projectType,
                totalBudget,  
                startYear,
                endYear,
                projectRank,
                undertakeFunding,
                tid
            } = req.body;

            const [rows, fields] = await mysqlPool.query('select * from UndertakeProject where PID = ?', [modifyPid]);

            if(rows.length > 1) {
                throw new Error('不支持普通用户修改涉及多位教师的项目相关数据');
            }

            await mysqlPool.query('update Project set Pname = ?, project_source = ?, project_type = ?, total_budget= ?, start_year = ?, end_year = ? where PID = ?',
                                [pname, projectSource, projectType, totalBudget, startYear, endYear, modifyPid]);
            await mysqlPool.query('update UndertakeProject set project_rank = ?, undertake_funding = ?, undertake_credit_hours = ? where TID = ? and PID = ?',
                                [projectRank, undertakeFunding, tid, modifyPid]);
            return res.json({
                    message: "修改项目信息成功"
            });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    app.patch('/api/project/admin', async (req, res) => {
        try {
            const {
                modifyProjects,
                modifyTids
            } = req.body;

            const [rows, fields] = await mysqlPool.query('select * from UndertakeProject where PID = ?', [modifyProjects[0].modifyPid]);

            let totalUndertakeFunding = 0;

            if(rows.length !== 0) {
                await Promise.all(modifyProjects.map(async element => {
                    if (rows.some(row => row.project_rank === parseInt(element.projectRank) && !modifyTids.includes(row.TID))) {
                      throw new Error("该项目已存在与你排名一样的教师");
                    }
                }));

                await Promise.all(rows.map(async row => {
                    if(! modifyTids.includes(row.TID)) {
                        totalUndertakeFunding += parseInt(row.undertake_funding);
                    }
                }));
            }
            
            await Promise.all(modifyProjects.map(async element => {
                totalUndertakeFunding += parseInt(element.undertakeFunding);
              }));

            if(totalUndertakeFunding !== parseInt(modifyProjects[0].totalBudget)) {
                throw new Error("所有教师承担经费总和不等于项目总经费");
            }

            await mysqlPool.query('update Project set Pname = ?, project_source = ?, project_type = ?, total_budget= ?, start_year = ?, end_year = ? where PID = ?',
                                [modifyProjects[0].pname, modifyProjects[0].projectSource, modifyProjects[0].projectType, modifyProjects[0].totalBudget, modifyProjects[0].startYear, modifyProjects[0].endYear, modifyProjects[0].modifyPid]);
            
            await Promise.all(modifyProjects.map(async element => {              
                await mysqlPool.query(
                    'update UndertakeProject set project_rank = ?, undertake_funding = ? where TID = ? and PID = ?',
                  [element.projectRank, element.undertakeFunding, element.modifyTid, element.modifyPid]
                );
              }));
              
            return res.json({
                    message: "修改项目信息成功"
            });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    app.delete('/api/project', async (req, res) => {
        try {
            
            const { deletePid, tid } = req.body;

            const [rows, fields] = await mysqlPool.query('select * from UndertakeProject where PID = ?', [deletePid]);

            if(rows.length > 1) {
                throw new Error('不支持普通用户删除涉及多位教师的项目相关数据');
            }

            await mysqlPool.query('delete from UndertakeProject where TID = ? and PID = ?',[tid, deletePid]);
            await mysqlPool.query('delete from Project PID = ?',[deletePid]);

            return res.json({
                message: "删除项目成功"
            })
        } catch(err) {
            return res.status(400).json({
                message: err.message
            });
        }
    }) 

    app.delete('/api/project/admin', async (req, res) => {
        try {
            const { deletePid } = req.body;
            await mysqlPool.query('delete from UndertakeProject where PID = ?',[deletePid]);
            await mysqlPool.query('delete from Project where PID = ?',[deletePid]);

            return res.json({
                message: "删除项目成功"
            })
        } catch(err) {
            return res.status(400).json({
                message: err.message
            });
        }
    }) 
}

export default projectAPIs;