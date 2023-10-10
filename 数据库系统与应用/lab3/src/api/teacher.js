import Crypto from 'crypto';
import uuid from 'uuid';
import multer from 'multer';

import mysqlPool from '../model/db';

const upload = multer({
    dest: "./static/upload"
})

const teacherAPIs = (app) => {

    app.get('/api/teacher/:tid', async (req, res) => {
        try {
            const { tid } = req.params;
            //// if(!tid) {
            ////     return res.status(400).json({ 
            ////         message: "没有输入教师工号",
            ////     });
            //// }
            const [rows, fields] = await mysqlPool.query('select * from Teacher where TID = ?', [tid]);
            if (rows.length === 0) {
                return res.status(400).json({
                    message: "该教师不存在",
                });
            }
            const data = {
                tid: rows[0].TID,
                tname: rows[0].Tname,
                gender: rows[0].gender,
                job: rows[0].job,
                avatar: rows[0].avatar
            }
            return res.json({ data });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    app.post('/api/teacher', async (req, res) => {
        try {
            const {
                tid,
                tname,
                gender,
                job,
                password
            } = req.body;

            const [rows, fields] = await mysqlPool.query('select * from Teacher where TID = ?', [tid]);
            if (rows.length !== 0) {
                return res.status(400).json({
                    message: "该教师工号已存在",
                });
            }

            const passwordCryp = Crypto.createHash('sha1')
                .update(password)
                .digest('hex');

            await mysqlPool.query('insert into Teacher(TID, Tname, gender, job, password) VALUES (?, ?, ?, ?, ?)',
                [tid, tname, gender, job, passwordCryp]);

            return res.json({
                message: "新增教师成功"
            })

        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    app.post('/api/teacher/login', async (req, res) => {
        try {
            const {
                tid,
                password
            } = req.body;
            const passwordCryp = Crypto.createHash('sha1')
                .update(password)
                .digest('hex');
            const [rows, fields] = await mysqlPool.query('select * from Teacher where TID = ?', [tid]);
            if (rows.length === 0) {
                return res.status(400).json({
                    message: "该教师工号不存在",
                });
            }
            if (rows[0].password !== passwordCryp) {
                return res.status(400).json({
                    message: "密码错误,请重新输入",
                });
            }
            const tokenstr = uuid.v4();
            const token = Crypto.createHash('sha1')
                .update(tokenstr)
                .digest('hex');
            await mysqlPool.query('update Teacher set token = ? where TID = ?', [token, tid]);
            const [newRows, newFields] = await mysqlPool.query('select * from Teacher where TID = ?', [tid]);
            const data = {
                tid: newRows[0].TID,
                tname: newRows[0].Tname,
                gender: newRows[0].gender,
                token: newRows[0].token,
                job: newRows[0].job,
                avatar: newRows[0].avatar
            }
            return res.json({
                message: "成功登录",
                data: data,
                admin: newRows[0].admin
            });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    app.post('/api/teacher/auth', async (req, res) => {
        try {
            const {
                tid,
                token
            } = req.body;
            if (!tid) {
                return res.status(400).json({
                    message: "没有输入教师工号",
                });
            }
            if (!token) {
                return res.status(400).json({
                    message: "没有输入token",
                });
            }
            const [rows, fields] = await mysqlPool.query('select * from Teacher where TID = ?', [tid]);
            if (rows.length === 0) {
                return res.status(400).json({
                    message: "该教师工号不存在",
                });
            }
            if (token !== rows[0].token) {
                return res.status(400).json({
                    message: "token已过期,请重新登录",
                });
            }
            const data = {
                tid: rows[0].TID,
                tname: rows[0].Tname,
                gender: rows[0].gender,
                job: rows[0].job,
                avatar: rows[0].avatar
            }
            return res.json({
                message: "成功验证",
                data: data,
                admin: rows[0].admin
            });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    });

    app.post('/api/teacher/logout', async (req, res) => {
        try {
            const {
                tid,
                token
            } = req.body;

            //// if (!tid) {
            ////     return res.status(400).json({
            ////         message: "没有输入教师工号",
            ////     });
            //// }
            //// if (!token) {
            ////     return res.status(400).json({
            ////         message: "没有输入token",
            ////     });
            //// }
            const [rows, fields] = await mysqlPool.query('select * from Teacher where TID = ?', [tid]);
            if (rows.length === 0) {
                return res.status(400).json({
                    message: "该教师工号不存在",
                });
            }
            if (token !== rows[0].token) {
                return res.status(400).json({
                    message: "token已过期,请重新登录",
                });
            }
            await mysqlPool.query('update Teacher set token = ? where TID = ?', [null, tid]);
            return res.json({
                message: "退出成功"
            });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })

    app.patch('/api/teacher', upload.single('avatar'), async (req, res) => {
        try {
            const {
                tid,
                token                
            } = req.body;
            //// if (!tid) {
            ////     return res.status(400).json({
            ////         message: "没有输入教师工号",
            ////     });
            //// }
            //// if (!token) {
            ////     return res.status(400).json({
            ////         message: "没有输入token",
            ////     });
            //// }
            const [rows, fields] = await mysqlPool.query('select * from Teacher where TID = ?', [tid]);
            if (rows.length === 0) {
                return res.status(400).json({
                    message: "该教师工号不存在",
                });
            }
            if (token !== rows[0].token) {
                return res.status(400).json({
                    message: "token已过期,请重新登录",
                });
            }
            if (req.file) {
                const avatar = req.file.filename;
                await mysqlPool.query('update Teacher set avatar = ? where TID = ?', [avatar, tid]);
            } 
            return res.json({
                message: "修改用户资料成功"
            });
        } catch (err) {
            return res.status(400).json({
                message: err.message
            })
        }
    })
};

export default teacherAPIs;