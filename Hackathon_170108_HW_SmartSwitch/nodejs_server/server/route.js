const path = require("path");
const fs = require("fs");
const url = require('url');
const mysql = require('mysql');

//信号量
const {
    MysqlOnline,
    EnqueueMysql
    } = require(path.resolve(__dirname, "..", "utils/SIGN.js"));

//数据库配置项
const DBConfig = require(path.resolve(__dirname, "..", "utils/DBconfig.js"));

//加载路由
const _Router = require(path.resolve(__dirname, "Router.js"));


/*
 * 说明，为了支持并发，节省支援，采用连接池
 *
 * */
const pool = mysql.createPool(DBConfig);

/**
 *
 *说明:挂着池化DB
 *
 * */

const _Route = app=> {
    app.pool = pool;

    app.pool.on('enqueue', function () {
        process.send({
            cmd: EnqueueMysql,
            msg: `Waiting for available connection slot.`
        });
    });

    app.pool.on('connection', (connection) => {
        process.send({
            cmd: MysqlOnline,
            msg: `connected as id:  ${connection.threadId}.`
        });
    });

    /**
     * 说明：挂在请求方法 与 回调函数
     *
     *
     * */
    return _Router(app);
};

module.exports = _Route;