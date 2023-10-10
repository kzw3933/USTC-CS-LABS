import mysql from 'mysql2/promise';

const pool = mysql.createPool({
    host: 'localhost',
    user: 'root',
    password: '20160137k',
    database: 'lab3',
    waitForConnections: true,
    connectionLimit: 10,
    queueLimit: 0
});

export default pool;