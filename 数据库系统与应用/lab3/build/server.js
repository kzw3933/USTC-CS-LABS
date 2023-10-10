/******/ (function(modules) { // webpackBootstrap
/******/ 	// The module cache
/******/ 	var installedModules = {};
/******/
/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {
/******/
/******/ 		// Check if module is in cache
/******/ 		if(installedModules[moduleId]) {
/******/ 			return installedModules[moduleId].exports;
/******/ 		}
/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = installedModules[moduleId] = {
/******/ 			i: moduleId,
/******/ 			l: false,
/******/ 			exports: {}
/******/ 		};
/******/
/******/ 		// Execute the module function
/******/ 		modules[moduleId].call(module.exports, module, module.exports, __webpack_require__);
/******/
/******/ 		// Flag the module as loaded
/******/ 		module.l = true;
/******/
/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}
/******/
/******/
/******/ 	// expose the modules object (__webpack_modules__)
/******/ 	__webpack_require__.m = modules;
/******/
/******/ 	// expose the module cache
/******/ 	__webpack_require__.c = installedModules;
/******/
/******/ 	// define getter function for harmony exports
/******/ 	__webpack_require__.d = function(exports, name, getter) {
/******/ 		if(!__webpack_require__.o(exports, name)) {
/******/ 			Object.defineProperty(exports, name, { enumerable: true, get: getter });
/******/ 		}
/******/ 	};
/******/
/******/ 	// define __esModule on exports
/******/ 	__webpack_require__.r = function(exports) {
/******/ 		if(typeof Symbol !== 'undefined' && Symbol.toStringTag) {
/******/ 			Object.defineProperty(exports, Symbol.toStringTag, { value: 'Module' });
/******/ 		}
/******/ 		Object.defineProperty(exports, '__esModule', { value: true });
/******/ 	};
/******/
/******/ 	// create a fake namespace object
/******/ 	// mode & 1: value is a module id, require it
/******/ 	// mode & 2: merge all properties of value into the ns
/******/ 	// mode & 4: return value when already ns object
/******/ 	// mode & 8|1: behave like require
/******/ 	__webpack_require__.t = function(value, mode) {
/******/ 		if(mode & 1) value = __webpack_require__(value);
/******/ 		if(mode & 8) return value;
/******/ 		if((mode & 4) && typeof value === 'object' && value && value.__esModule) return value;
/******/ 		var ns = Object.create(null);
/******/ 		__webpack_require__.r(ns);
/******/ 		Object.defineProperty(ns, 'default', { enumerable: true, value: value });
/******/ 		if(mode & 2 && typeof value != 'string') for(var key in value) __webpack_require__.d(ns, key, function(key) { return value[key]; }.bind(null, key));
/******/ 		return ns;
/******/ 	};
/******/
/******/ 	// getDefaultExport function for compatibility with non-harmony modules
/******/ 	__webpack_require__.n = function(module) {
/******/ 		var getter = module && module.__esModule ?
/******/ 			function getDefault() { return module['default']; } :
/******/ 			function getModuleExports() { return module; };
/******/ 		__webpack_require__.d(getter, 'a', getter);
/******/ 		return getter;
/******/ 	};
/******/
/******/ 	// Object.prototype.hasOwnProperty.call
/******/ 	__webpack_require__.o = function(object, property) { return Object.prototype.hasOwnProperty.call(object, property); };
/******/
/******/ 	// __webpack_public_path__
/******/ 	__webpack_require__.p = "";
/******/
/******/
/******/ 	// Load entry module and return exports
/******/ 	return __webpack_require__(__webpack_require__.s = "./src/server.js");
/******/ })
/************************************************************************/
/******/ ({

/***/ "./node_modules/crypto-browserify/index.js":
/*!*************************************************!*\
  !*** ./node_modules/crypto-browserify/index.js ***!
  \*************************************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

"use strict";


exports.randomBytes = exports.rng = exports.pseudoRandomBytes = exports.prng = __webpack_require__(/*! randombytes */ "randombytes")
exports.createHash = exports.Hash = __webpack_require__(/*! create-hash */ "create-hash")
exports.createHmac = exports.Hmac = __webpack_require__(/*! create-hmac */ "create-hmac")

var algos = __webpack_require__(/*! browserify-sign/algos */ "browserify-sign/algos")
var algoKeys = Object.keys(algos)
var hashes = ['sha1', 'sha224', 'sha256', 'sha384', 'sha512', 'md5', 'rmd160'].concat(algoKeys)
exports.getHashes = function () {
  return hashes
}

var p = __webpack_require__(/*! pbkdf2 */ "pbkdf2")
exports.pbkdf2 = p.pbkdf2
exports.pbkdf2Sync = p.pbkdf2Sync

var aes = __webpack_require__(/*! browserify-cipher */ "browserify-cipher")

exports.Cipher = aes.Cipher
exports.createCipher = aes.createCipher
exports.Cipheriv = aes.Cipheriv
exports.createCipheriv = aes.createCipheriv
exports.Decipher = aes.Decipher
exports.createDecipher = aes.createDecipher
exports.Decipheriv = aes.Decipheriv
exports.createDecipheriv = aes.createDecipheriv
exports.getCiphers = aes.getCiphers
exports.listCiphers = aes.listCiphers

var dh = __webpack_require__(/*! diffie-hellman */ "diffie-hellman")

exports.DiffieHellmanGroup = dh.DiffieHellmanGroup
exports.createDiffieHellmanGroup = dh.createDiffieHellmanGroup
exports.getDiffieHellman = dh.getDiffieHellman
exports.createDiffieHellman = dh.createDiffieHellman
exports.DiffieHellman = dh.DiffieHellman

var sign = __webpack_require__(/*! browserify-sign */ "browserify-sign")

exports.createSign = sign.createSign
exports.Sign = sign.Sign
exports.createVerify = sign.createVerify
exports.Verify = sign.Verify

exports.createECDH = __webpack_require__(/*! create-ecdh */ "create-ecdh")

var publicEncrypt = __webpack_require__(/*! public-encrypt */ "public-encrypt")

exports.publicEncrypt = publicEncrypt.publicEncrypt
exports.privateEncrypt = publicEncrypt.privateEncrypt
exports.publicDecrypt = publicEncrypt.publicDecrypt
exports.privateDecrypt = publicEncrypt.privateDecrypt

// the least I can do is make error messages for the rest of the node.js/crypto api.
// ;[
//   'createCredentials'
// ].forEach(function (name) {
//   exports[name] = function () {
//     throw new Error([
//       'sorry, ' + name + ' is not implemented yet',
//       'we accept pull requests',
//       'https://github.com/crypto-browserify/crypto-browserify'
//     ].join('\n'))
//   }
// })

var rf = __webpack_require__(/*! randomfill */ "randomfill")

exports.randomFill = rf.randomFill
exports.randomFillSync = rf.randomFillSync

exports.createCredentials = function () {
  throw new Error([
    'sorry, createCredentials is not implemented yet',
    'we accept pull requests',
    'https://github.com/crypto-browserify/crypto-browserify'
  ].join('\n'))
}

exports.constants = {
  'DH_CHECK_P_NOT_SAFE_PRIME': 2,
  'DH_CHECK_P_NOT_PRIME': 1,
  'DH_UNABLE_TO_CHECK_GENERATOR': 4,
  'DH_NOT_SUITABLE_GENERATOR': 8,
  'NPN_ENABLED': 1,
  'ALPN_ENABLED': 1,
  'RSA_PKCS1_PADDING': 1,
  'RSA_SSLV23_PADDING': 2,
  'RSA_NO_PADDING': 3,
  'RSA_PKCS1_OAEP_PADDING': 4,
  'RSA_X931_PADDING': 5,
  'RSA_PKCS1_PSS_PADDING': 6,
  'POINT_CONVERSION_COMPRESSED': 2,
  'POINT_CONVERSION_UNCOMPRESSED': 4,
  'POINT_CONVERSION_HYBRID': 6
}


/***/ }),

/***/ "./src/api/lesson.js":
/*!***************************!*\
  !*** ./src/api/lesson.js ***!
  \***************************/
/*! exports provided: default */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
__webpack_require__.r(__webpack_exports__);
/* harmony import */ var _model_db__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! ../model/db */ "./src/model/db.js");


const lessonAPIs = async (app) => {
    app.get('/api/lesson/:tid', async (req, res) => {
        try {
            const { tid } = req.params;
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from TeachCourse, Lesson where TeachCourse.LID = Lesson.LID and TeachCourse.TID = ?', [tid]);
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
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from Lesson where LID = ?', [lid]);
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
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from TeachCourse, Lesson where TeachCourse.LID = Lesson.LID and TID = ? and TeachCourse.LID = ?', [tid, lid]);
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
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query(sql, sql_array);
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from Lesson where LID = ?', [lid]);

            if(rows.length > 0) {
                throw new Error('数据库已存在该课程,不支持普通用户新增涉及多位教师的课程相关数据')
            }

            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('insert into Lesson(LID, Lname, credit_hours, course_nature) VALUES (?, ?, ?, ?)',
                                [lid, lname, creditHours, courseNature]);
            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('insert into TeachCourse(TID, LID, teach_year, term, undertake_credit_hours) VALUES (?, ?, ?, ?, ?)',
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query(sql, sql_array);

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


            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from Lesson where LID = ?', [postLessons[0].lid]);

            if(rows.length === 0) {
                await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('insert into Lesson(LID, Lname, credit_hours, course_nature) VALUES (?, ?, ?, ?)',
                [postLessons[0].lid, postLessons[0].lname, postLessons[0].creditHours, postLessons[0].courseNature]);
            }

            await Promise.all(postLessons.map(async element => {
                await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('insert into TeachCourse(TID, LID, teach_year, term, undertake_credit_hours) VALUES (?, ?, ?, ?, ?)',
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from TeachCourse where LID = ?', [modifyLid]);

            if(rows.length > 1) {
                throw new Error('不支持普通用户修改涉及多位教师的课程相关数据');
            }

            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('update Lesson set Lname = ?, credit_hours = ?, course_nature = ? where LID = ?',
                                [lname, creditHours, courseNature, modifyLid]);

            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('update TeachCourse set teach_year = ?, term = ?, undertake_credit_hours = ? where TID = ? and LID = ?',
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from TeachCourse where LID = ?', [modifyLessons[0].modifyLid]);

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

            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('update Lesson set Lname = ?, credit_hours = ?, course_nature = ? where LID = ?',
                                [modifyLessons[0].lname, modifyLessons[0].creditHours, modifyLessons[0].courseNature, modifyLessons[0].modifyLid]);
            
            await Promise.all(modifyLessons.map(async element => {
                await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('update TeachCourse set teach_year = ?, term = ?, undertake_credit_hours = ? where TID = ? and LID = ?',
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from TeachCourse where LID = ?', [deleteLid]);

            if(rows.length > 1) {
                throw new Error('不支持普通用户删除涉及多位教师的课程相关数据');
            }

            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('delete from TeachCourse where TID = ? and LID = ?',[tid, deleteLid]);
            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('delete from Lesson where LID = ?',[deleteLid]);

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

            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('delete from TeachCourse where LID = ?',[deleteLid]);
            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('delete from Lesson where LID = ?',[deleteLid]);

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

/* harmony default export */ __webpack_exports__["default"] = (lessonAPIs);

/***/ }),

/***/ "./src/api/paper.js":
/*!**************************!*\
  !*** ./src/api/paper.js ***!
  \**************************/
/*! exports provided: default */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
__webpack_require__.r(__webpack_exports__);
/* harmony import */ var _model_db__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! ../model/db */ "./src/model/db.js");


const paperAPIs = async (app) => {
    app.get('/api/paper/:tid', async (req, res) => {
        try {
            const { tid } = req.params;
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from PublishPaper, Paper where PublishPaper.SN = Paper.SN and TID = ?', [tid]);
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
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from Paper where SN = ?', [sn]);
            
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
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from PublishPaper, Paper  where PublishPaper.SN = Paper.SN and TID = ? and PublishPaper.SN = ?', [tid, sn]);

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
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query(sql, sql_array);
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from Paper where SN = ?', [sn]);

            if(rows.length > 0) {
                throw new Error('数据库已存在该论文,不支持普通用户新增涉及多位教师的论文相关数据')
            }

            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('insert into Paper(SN, title, public_source, publication_year, public_type, public_rank) VALUES (?, ?, ?, ?, ?, ?)',
                                [sn, title, publicSource, publicationYear, publicType, publicRank]);
            
            
            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('insert into PublishPaper(TID, SN, paper_rank, is_corresponding_author) VALUES (?, ?, ?, ?)',
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query(sql, sql_array);
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from Paper where SN = ?', [postPapers[0].sn]);
            const [tmp_rows, tmp_fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from PublishPaper where SN = ?', postPapers[0].sn);

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
                await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('insert into Paper (SN, title, public_source, publication_year, public_type, public_rank) VALUES (?, ?, ?, ?, ?, ?)',
                [postPapers[0].sn, postPapers[0].title, postPapers[0].publicSource, postPapers[0].publicationYear, postPapers[0].publicType, postPapers[0].publicRank]);
            }

            await Promise.all(postPapers.map(async element => {
                await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('insert into PublishPaper (TID, SN, paper_rank, is_corresponding_author) VALUES (?, ?, ?, ?)',
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from PublishPaper where SN = ?', [modifySN]);

            if(rows.length > 1) {
                throw new Error('不支持普通用户修改涉及多位教师的论文相关数据');
            }

            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('update Paper set title = ?, public_source = ?, publication_year = ?, public_type = ?, public_rank = ? where SN = ?',
                                [title, publicSource, publicationYear, publicType, publicRank, modifySN]);

            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('update PublishPaper set paper_rank = ?, is_corresponding_author = ? where TID = ? and SN = ?',
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

            const [tmp_rows, tmp_fields] =  await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from PublishPaper where SN = ?', modifyPapers[0].modifySN);

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

            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query(
                'update Paper set title = ?, public_source = ?, publication_year = ?, public_type = ?, public_rank = ? where SN = ?',
                [modifyPapers[0].title, modifyPapers[0].publicSource, modifyPapers[0].publicationYear, modifyPapers[0].publicType, modifyPapers[0].publicRank, modifyPapers[0].modifySN]
            );
              
            await Promise.all(modifyPapers.map(async element => {              
                await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query(
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from PublishPaper where SN = ?', [deleteSN]);

            if(rows.length > 1) {
                throw new Error('不支持普通用户删除涉及多位教师的论文相关数据');
            }

            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('delete from PublishPaper where SN = ? and TID = ?',[deleteSN, tid]);
            
            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('delete from Paper where SN = ?',[deleteSN]);

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
                await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('delete from PublishPaper where SN = ? and TID = ?', [element.deleteSN, element.deleteTid]);
              }));
              
            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('delete from Paper where SN = ?', [deletePapers[0].deleteSN]);

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

/* harmony default export */ __webpack_exports__["default"] = (paperAPIs);

/***/ }),

/***/ "./src/api/project.js":
/*!****************************!*\
  !*** ./src/api/project.js ***!
  \****************************/
/*! exports provided: default */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
__webpack_require__.r(__webpack_exports__);
/* harmony import */ var _model_db__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! ../model/db */ "./src/model/db.js");


const projectAPIs = async (app) => {
    app.get('/api/project/:tid', async (req, res) => {
        try {
            const { tid } = req.params;
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from UndertakeProject where TID = ?', [tid]);
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
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from Project where PID = ?', [pid]);
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
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from UndertakeProject, Project where UndertakeProject.PID = Project.PID and TID = ? and UndertakeProject.PID = ?', [tid, pid]);
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
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query(sql, sql_array);
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from Project where PID = ?', [pid]);

            if(rows.length > 0) {
                throw new Error('数据库已存在该项目,不支持普通用户新增涉及多位教师的项目相关数据')
            }


            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('insert into Project(PID, Pname, project_source, project_type, total_budget, start_year, end_year) VALUES (?, ?, ?, ?, ?, ?, ?)',
                                [pid, pname, projectSource, projectType, totalBudget, startYear, endYear]);
            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('insert into UndertakeProject(TID, PID, project_rank, undertake_funding) VALUES (?, ?, ?, ?)',
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query(sql, sql_array);
            
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from Project where PID = ?', [postProjects[0].pid]);
            const [tmp_rows, tmp_fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from UndertakeProject where PID = ?',  [postProjects[0].pid]);

            if(rows.length === 0) {
                await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('insert into Project(PID, Pname, project_source, project_type, total_budget, start_year, end_year) VALUES (?, ?, ?, ?, ?, ?, ?)',
                                [postProjects[0].pid, postProjects[0].pname, postProjects[0].projectSource, postProjects[0].projectType, postProjects[0].totalBudget, postProjects[0].startYear, postProjects[0].endYear]);
            }

            await Promise.all(postProjects.map(async element => {
                await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('insert into UndertakeProject(TID, PID, project_rank, undertake_funding) VALUES (?, ?, ?, ?)',
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from UndertakeProject where PID = ?', [modifyPid]);

            if(rows.length > 1) {
                throw new Error('不支持普通用户修改涉及多位教师的项目相关数据');
            }

            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('update Project set Pname = ?, project_source = ?, project_type = ?, total_budget= ?, start_year = ?, end_year = ? where PID = ?',
                                [pname, projectSource, projectType, totalBudget, startYear, endYear, modifyPid]);
            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('update UndertakeProject set project_rank = ?, undertake_funding = ?, undertake_credit_hours = ? where TID = ? and PID = ?',
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from UndertakeProject where PID = ?', [modifyProjects[0].modifyPid]);

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

            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('update Project set Pname = ?, project_source = ?, project_type = ?, total_budget= ?, start_year = ?, end_year = ? where PID = ?',
                                [modifyProjects[0].pname, modifyProjects[0].projectSource, modifyProjects[0].projectType, modifyProjects[0].totalBudget, modifyProjects[0].startYear, modifyProjects[0].endYear, modifyProjects[0].modifyPid]);
            
            await Promise.all(modifyProjects.map(async element => {              
                await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query(
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('select * from UndertakeProject where PID = ?', [deletePid]);

            if(rows.length > 1) {
                throw new Error('不支持普通用户删除涉及多位教师的项目相关数据');
            }

            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('delete from UndertakeProject where TID = ? and PID = ?',[tid, deletePid]);
            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('delete from Project PID = ?',[deletePid]);

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
            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('delete from UndertakeProject where PID = ?',[deletePid]);
            await _model_db__WEBPACK_IMPORTED_MODULE_0__["default"].query('delete from Project where PID = ?',[deletePid]);

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

/* harmony default export */ __webpack_exports__["default"] = (projectAPIs);

/***/ }),

/***/ "./src/api/teacher.js":
/*!****************************!*\
  !*** ./src/api/teacher.js ***!
  \****************************/
/*! exports provided: default */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
__webpack_require__.r(__webpack_exports__);
/* harmony import */ var crypto__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! crypto */ "./node_modules/crypto-browserify/index.js");
/* harmony import */ var crypto__WEBPACK_IMPORTED_MODULE_0___default = /*#__PURE__*/__webpack_require__.n(crypto__WEBPACK_IMPORTED_MODULE_0__);
/* harmony import */ var uuid__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! uuid */ "uuid");
/* harmony import */ var uuid__WEBPACK_IMPORTED_MODULE_1___default = /*#__PURE__*/__webpack_require__.n(uuid__WEBPACK_IMPORTED_MODULE_1__);
/* harmony import */ var multer__WEBPACK_IMPORTED_MODULE_2__ = __webpack_require__(/*! multer */ "multer");
/* harmony import */ var multer__WEBPACK_IMPORTED_MODULE_2___default = /*#__PURE__*/__webpack_require__.n(multer__WEBPACK_IMPORTED_MODULE_2__);
/* harmony import */ var _model_db__WEBPACK_IMPORTED_MODULE_3__ = __webpack_require__(/*! ../model/db */ "./src/model/db.js");






const upload = multer__WEBPACK_IMPORTED_MODULE_2___default()({
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
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_3__["default"].query('select * from Teacher where TID = ?', [tid]);
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

            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_3__["default"].query('select * from Teacher where TID = ?', [tid]);
            if (rows.length !== 0) {
                return res.status(400).json({
                    message: "该教师工号已存在",
                });
            }

            const passwordCryp = crypto__WEBPACK_IMPORTED_MODULE_0___default.a.createHash('sha1')
                .update(password)
                .digest('hex');

            await _model_db__WEBPACK_IMPORTED_MODULE_3__["default"].query('insert into Teacher(TID, Tname, gender, job, password) VALUES (?, ?, ?, ?, ?)',
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
            const passwordCryp = crypto__WEBPACK_IMPORTED_MODULE_0___default.a.createHash('sha1')
                .update(password)
                .digest('hex');
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_3__["default"].query('select * from Teacher where TID = ?', [tid]);
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
            const tokenstr = uuid__WEBPACK_IMPORTED_MODULE_1___default.a.v4();
            const token = crypto__WEBPACK_IMPORTED_MODULE_0___default.a.createHash('sha1')
                .update(tokenstr)
                .digest('hex');
            await _model_db__WEBPACK_IMPORTED_MODULE_3__["default"].query('update Teacher set token = ? where TID = ?', [token, tid]);
            const [newRows, newFields] = await _model_db__WEBPACK_IMPORTED_MODULE_3__["default"].query('select * from Teacher where TID = ?', [tid]);
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
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_3__["default"].query('select * from Teacher where TID = ?', [tid]);
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
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_3__["default"].query('select * from Teacher where TID = ?', [tid]);
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
            await _model_db__WEBPACK_IMPORTED_MODULE_3__["default"].query('update Teacher set token = ? where TID = ?', [null, tid]);
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
            const [rows, fields] = await _model_db__WEBPACK_IMPORTED_MODULE_3__["default"].query('select * from Teacher where TID = ?', [tid]);
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
                await _model_db__WEBPACK_IMPORTED_MODULE_3__["default"].query('update Teacher set avatar = ? where TID = ?', [avatar, tid]);
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

/* harmony default export */ __webpack_exports__["default"] = (teacherAPIs);

/***/ }),

/***/ "./src/model/db.js":
/*!*************************!*\
  !*** ./src/model/db.js ***!
  \*************************/
/*! exports provided: default */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
__webpack_require__.r(__webpack_exports__);
/* harmony import */ var mysql2_promise__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! mysql2/promise */ "mysql2/promise");
/* harmony import */ var mysql2_promise__WEBPACK_IMPORTED_MODULE_0___default = /*#__PURE__*/__webpack_require__.n(mysql2_promise__WEBPACK_IMPORTED_MODULE_0__);


const pool = mysql2_promise__WEBPACK_IMPORTED_MODULE_0___default.a.createPool({
    host: 'localhost',
    user: 'root',
    password: '20160137k',
    database: 'lab3',
    waitForConnections: true,
    connectionLimit: 10,
    queueLimit: 0
});

/* harmony default export */ __webpack_exports__["default"] = (pool);

/***/ }),

/***/ "./src/server.js":
/*!***********************!*\
  !*** ./src/server.js ***!
  \***********************/
/*! no exports provided */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
__webpack_require__.r(__webpack_exports__);
/* harmony import */ var express__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! express */ "express");
/* harmony import */ var express__WEBPACK_IMPORTED_MODULE_0___default = /*#__PURE__*/__webpack_require__.n(express__WEBPACK_IMPORTED_MODULE_0__);
/* harmony import */ var body_parser__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! body-parser */ "body-parser");
/* harmony import */ var body_parser__WEBPACK_IMPORTED_MODULE_1___default = /*#__PURE__*/__webpack_require__.n(body_parser__WEBPACK_IMPORTED_MODULE_1__);
/* harmony import */ var cors__WEBPACK_IMPORTED_MODULE_2__ = __webpack_require__(/*! cors */ "cors");
/* harmony import */ var cors__WEBPACK_IMPORTED_MODULE_2___default = /*#__PURE__*/__webpack_require__.n(cors__WEBPACK_IMPORTED_MODULE_2__);
/* harmony import */ var _api_paper__WEBPACK_IMPORTED_MODULE_3__ = __webpack_require__(/*! ./api/paper */ "./src/api/paper.js");
/* harmony import */ var _api_teacher__WEBPACK_IMPORTED_MODULE_4__ = __webpack_require__(/*! ./api/teacher */ "./src/api/teacher.js");
/* harmony import */ var _api_project__WEBPACK_IMPORTED_MODULE_5__ = __webpack_require__(/*! ./api/project */ "./src/api/project.js");
/* harmony import */ var _api_lesson__WEBPACK_IMPORTED_MODULE_6__ = __webpack_require__(/*! ./api/lesson */ "./src/api/lesson.js");









const app = express__WEBPACK_IMPORTED_MODULE_0___default()();
app.use(cors__WEBPACK_IMPORTED_MODULE_2___default()());
app.use(body_parser__WEBPACK_IMPORTED_MODULE_1___default.a.json());
app.use(express__WEBPACK_IMPORTED_MODULE_0___default.a.static('static'));
Object(_api_paper__WEBPACK_IMPORTED_MODULE_3__["default"])(app);
Object(_api_teacher__WEBPACK_IMPORTED_MODULE_4__["default"])(app);
Object(_api_project__WEBPACK_IMPORTED_MODULE_5__["default"])(app);
Object(_api_lesson__WEBPACK_IMPORTED_MODULE_6__["default"])(app);
app.listen(3000);

/***/ }),

/***/ "body-parser":
/*!******************************!*\
  !*** external "body-parser" ***!
  \******************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("body-parser");

/***/ }),

/***/ "browserify-cipher":
/*!************************************!*\
  !*** external "browserify-cipher" ***!
  \************************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("browserify-cipher");

/***/ }),

/***/ "browserify-sign":
/*!**********************************!*\
  !*** external "browserify-sign" ***!
  \**********************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("browserify-sign");

/***/ }),

/***/ "browserify-sign/algos":
/*!****************************************!*\
  !*** external "browserify-sign/algos" ***!
  \****************************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("browserify-sign/algos");

/***/ }),

/***/ "cors":
/*!***********************!*\
  !*** external "cors" ***!
  \***********************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("cors");

/***/ }),

/***/ "create-ecdh":
/*!******************************!*\
  !*** external "create-ecdh" ***!
  \******************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("create-ecdh");

/***/ }),

/***/ "create-hash":
/*!******************************!*\
  !*** external "create-hash" ***!
  \******************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("create-hash");

/***/ }),

/***/ "create-hmac":
/*!******************************!*\
  !*** external "create-hmac" ***!
  \******************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("create-hmac");

/***/ }),

/***/ "diffie-hellman":
/*!*********************************!*\
  !*** external "diffie-hellman" ***!
  \*********************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("diffie-hellman");

/***/ }),

/***/ "express":
/*!**************************!*\
  !*** external "express" ***!
  \**************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("express");

/***/ }),

/***/ "multer":
/*!*************************!*\
  !*** external "multer" ***!
  \*************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("multer");

/***/ }),

/***/ "mysql2/promise":
/*!*********************************!*\
  !*** external "mysql2/promise" ***!
  \*********************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("mysql2/promise");

/***/ }),

/***/ "pbkdf2":
/*!*************************!*\
  !*** external "pbkdf2" ***!
  \*************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("pbkdf2");

/***/ }),

/***/ "public-encrypt":
/*!*********************************!*\
  !*** external "public-encrypt" ***!
  \*********************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("public-encrypt");

/***/ }),

/***/ "randombytes":
/*!******************************!*\
  !*** external "randombytes" ***!
  \******************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("randombytes");

/***/ }),

/***/ "randomfill":
/*!*****************************!*\
  !*** external "randomfill" ***!
  \*****************************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("randomfill");

/***/ }),

/***/ "uuid":
/*!***********************!*\
  !*** external "uuid" ***!
  \***********************/
/*! no static exports found */
/***/ (function(module, exports) {

module.exports = require("uuid");

/***/ })

/******/ });
//# sourceMappingURL=server.js.map