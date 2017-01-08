const path = require("path");
const fs = require("fs");
const qs = require('querystring');
const url = require('url');
const crypto = require('crypto');
const WebSocket = require('ws');
const net = require('net');

//信号量
const {
    Query,
    TCPconnected,
    TCPdisconnect
} = require(path.resolve(__dirname, "..", "utils/SIGN.js"));


//路径
const PATHS = {
    app: path.resolve(__dirname, "..", "app", "index.jsx"),
    build: path.resolve(__dirname, "..", "build", "index.html"),
    public: path.resolve(__dirname, "..", "build")
};


/**
 *
 * 说明：需要加载URL接口，就写在这里
 *
 * */
function Router(app) {
    app.POST("/dolegift", (req, res) => {
        let postData = "";
        req.on("data", data => {
            postData += data;
        });

        req.on("end", () => {
            let verification = false; //验证
            postData = qs.parse(postData);
            let reback = {verifi: verification};
            (gift = postData.giftSort) ? (reback.verifi = true) : "";
            res.writeHead(200, {'Content-Type': 'application/json'});
            res.end(JSON.stringify(reback));

        });

    });
    app.POST("/vipActivities", (req, res) => {
        let postData = "";
        req.on("data", data => {
            postData += data;
        });

        req.on("end", () => {
            let pool = app.pool;
            let verification = false; //验证
            let reback = {verifi: verification};


            postData = qs.parse(postData);

            pool.getConnection((err, connection) => {
                //查询
                connection.query('select * from `users`', function (err, rows, fields) {
                    if (err) throw err;

                    if (postData.ID == rows[0][`userID`] && postData.password == rows[0][`password`]) {
                        reback.verifi = true;
                    }
                    res.writeHead(200, {'Content-Type': 'application/json'});
                    res.end(JSON.stringify(reback));
                    process.send({
                        cmd: Query,
                        msg: `[child] 查询结果为=>${JSON.stringify(rows)}.`
                    });
                    connection.release();
                });
            });
        });
    });
    app.GET("/", (req, res) => {

        let statInfo = fs.statSync(PATHS.build);
        let regexpJS = /\.(js|jsx)$/;
        let regexpCSS = /\.(css)$/;
        let regexpImg = /\.(jpg|png)$/;
        let IncomingUrl = req.url;

        if (regexpJS.test(IncomingUrl)) {
            res.setHeader('Content-Type', 'application/x-javascript');
            res.setHeader('Server', 'huenchao');
            res.setHeader('X-Power-By', 'nodejs');
            res.writeHead(200, {});
            readStrem = fs.createReadStream(PATHS.public + IncomingUrl);
            readStrem.pipe(res);
            return;
        }
        if (regexpImg.test(IncomingUrl)) {
            let index = IncomingUrl.lastIndexOf(`.`);
            let ext = IncomingUrl.substr(index);
            res.setHeader('Content-Type', `image/${ext}`);
            res.setHeader('Server', 'huenchao');
            res.setHeader('X-Power-By', 'nodejs');
            res.writeHead(200, {});
            readStrem = fs.createReadStream(PATHS.public + IncomingUrl);
            readStrem.pipe(res);
            return;
        }
        if (regexpCSS.test(IncomingUrl)) {
            res.setHeader('Content-Type', 'text/css');
            res.setHeader('Server', 'huenchao');
            res.setHeader('X-Power-By', 'nodejs');
            res.writeHead(200, {});
            readStrem = fs.createReadStream(PATHS.public + IncomingUrl);
            readStrem.pipe(res);
            return;
        }

        if (statInfo) {
            res.setHeader('Content-Type', 'text/html');
            res.setHeader('Server', 'huenchao');
            res.setHeader('X-Power-By', 'nodejs');
            res.writeHead(200, {
                'Trailer': 'Content-MD5'
            });

            res.addTrailers({'Content-MD5': '7895bf4b8828b55ceaf47747b4bca667'});
            readStrem = fs.createReadStream(PATHS.build);
            readStrem.pipe(res);
        } else {
            res.writeHead(404, {'Content-Type': 'text/plain'});
            res.end("not found");
        }
    });


    /**
     *
     * 说明: 华为listos http接口
     *
     * */


    /*
     * *说明:控制开关
     *
     *
     * */
    app.POST("/todoSwitch", (req, res) => {

        let chunk = "",
            reback = "";

        req.on("data", data => {
            chunk += data;
        });

        req.on("end", () => {
            try {
                let postData = qs.parse(chunk),
                    pool = app.pool;


                pool.getConnection((err, connection) => {
                    //查询
                    connection.query(`UPDATE devices SET status = ${parseInt(postData.status)} WHERE deviceID = ${postData.deviceID}`, function (err, rows, fields) {
                        if (err) throw err;
                        const client = net.connect({port: 6666, host: "192.168.0.105"}, () => {
                            // 'connect' listener
                            process.send({
                                cmd: TCPconnected,
                                msg: 'connected to server!'
                            });

                            client.write(`${chunk}\r\n`);
                        });

                        /**
                         * 说明： 0 成功 1失败
                         *
                         * */

                        process.send({
                            cmd: Query,
                            msg: `[客户端] 传过来的数据=>${JSON.stringify(postData)}.`
                        });


                        let chunks = [];
                        let size = 0;

                        client.on('data', (chunk) => {

                            chunks.push(chunk);
                            size += chunk.length;


                        });

                        client.on('end', () => {

                            let data = null;
                            switch (chunks.length) {
                                case 0:
                                    data = Buffer.alloc(0);
                                    break;
                                case 1:
                                    data = chunks[0];
                                    break;
                                default:
                                    data = Buffer.alloc(size);
                                    for (let i = 0, pos = 0, l = chunks.length; i < l; i++) {
                                        let chunk = chunks[i];
                                        chunk.copy(data, pos);
                                        pos += chunk.length;
                                    }
                                    break;
                            }

                            process.send({
                                cmd: Query,
                                msg: `【服务器返回的数据】 = > ${data.toString()}`
                            });

                            res.end(JSON.stringify(
                                JSON.parse(data.toString())
                            ));
                            connection.release();

                            process.send({
                                cmd: TCPdisconnect,
                                msg: `disconnected from server.`
                            });

                        });

                    });
                });
            } catch (e) {
                reback = {status: 0, msg: "失败"};
                res.end(JSON.stringify(reback));
            }
        });
    });

    /**
     * 说明：获取设备信息
     *
     *
     * */
    app.POST("/getDevices", (req, res) => {

        let chunk = "",
            reback = "";

        req.on("data", data => {
            chunk += data;
        });


        req.on("end", () => {
            try {
                let postData = qs.parse(chunk),
                    pool = app.pool;

                pool.getConnection((err, connection) => {

                    //查询
                    connection.query(`select deviceID from UserDevicebind where userID =${postData.userID} `, (err, rows, fields) => {

                        if (err) throw err;

                        let length = rows.length;


                        let querySqlMiddle = ``;

                        for (let i = length; i;) {

                            if (i > 1) {
                                querySqlMiddle += `deviceID = ${rows[--i]['deviceID']}  or `;
                            } else {
                                querySqlMiddle += `deviceID = ${rows[--i]['deviceID']} `;
                            }
                        }

                        let sql = `select a.* from devices a where timestamp = (select max(timestamp)
                      from devices where deviceID = a.deviceID and (${querySqlMiddle}))order by a.deviceID`;


                        process.send({
                            cmd: Query,
                            msg: `[querySQL] 查询结果为=>${sql}.`
                        });

                        connection.query(`${sql} `, (err, rows, fields) => {

                            if (err) throw err;

                            res.end(JSON.stringify(rows));

                            process.send({
                                cmd: Query,
                                msg: `[child] 查询结果为=>${JSON.stringify(rows)}.`
                            });

                            connection.release();

                        });

                    });


                });
            } catch (e) {
                reback = {status: 0, msg: "Format problem"};
                res.end(JSON.stringify(reback));
            }
        });
    });

    /*
     * *
     * 说明：保存设备消息
     *
     *
     *
     * */

    app.POST("/saveDevices", (req, res) => {
        let chunk = "",
            reback = null;

        req.on("data", data => {
            chunk += data;
        });


        req.on("end", () => {
            try {
                let postData = qs.parse(chunk),
                    pool = app.pool;

                process.send({
                    cmd: Query,
                    msg: `[child] 查询结果为=>${JSON.stringify(postData)}.`
                });

                let timestamp = (new Date().getTime()).toString().substring(6);

                let sql = `insert into devices values  (${postData.deviceID},${postData.pm},${postData.humidity},${postData.temperature},
${parseInt(postData.status)},${timestamp},${"\"开关_" + postData.deviceID + "\""})`;

                pool.getConnection((err, connection) => {

                    process.send({
                        cmd: Query,
                        msg: `[sql] 全拼结果为=>${sql}.`
                    });

                    //查询
                    connection.query(sql, (err, rows, fields) => {

                        if (err) throw err;
                        reback = {status: 1, err: ""};
                        res.end(JSON.stringify(reback));


                        process.send({
                            cmd: Query,
                            msg: `[child] 查询结果为=>${JSON.stringify(rows)}.`
                        });
                        connection.release();
                    });
                });
            } catch (e) {
                reback = {status: 0, msg: "数据格式不对"};
                res.end(JSON.stringify(reback));
            }
        });


    });


    /*
     * *
     * 说明:客户端发送deviceID，
     *     返回一条最新的数据。
     *
     *
     *
     * */
    app.POST("/getNewOneDevices", (req, res) => {

        let chunk = "",
            reback = "";

        req.on("data", data => {
            chunk += data;
        });

        req.on("end", () => {
            try {
                let postData = qs.parse(chunk),
                    pool = app.pool;
                let sql = `select * from devices where deviceID =${postData.deviceID} order by timestamp desc limit 1`;

                pool.getConnection((err, connection) => {


                    //查询
                    connection.query(sql, (err, rows, fields) => {

                        if (err) throw err;


                        process.send({
                            cmd: Query,
                            msg: `[querySQL] sql全拼为=>${sql}.`
                        });


                        res.end(JSON.stringify(rows[0]));

                        process.send({
                            cmd: Query,
                            msg: `[child] 查询结果为=>${JSON.stringify(rows[0])}.`
                        });

                        connection.release();

                    });

                });
            } catch (e) {
                reback = {status: 0, msg: "Format problem"};
                res.end(JSON.stringify(reback));
            }
        });
    });


    /*
     * *
     * 说明:客户端发送deviceID和nickName，
     *     修改最新的那一条的nickName
     *
     *
     *
     *
     * */
    app.POST("/updateNickName", (req, res) => {

        let chunk = "",
            reback = "";

        req.on("data", data => {
            chunk += data;
        });


        req.on("end", () => {
            try {
                let postData = qs.parse(chunk),
                    pool = app.pool;
                let sql = `update devices set nickName = \"${postData.nickName}\" where deviceID =${postData.deviceID } `;

                pool.getConnection((err, connection) => {


                    process.send({
                        cmd: Query,
                        msg: `[querySQL] sql全拼为=>${sql}.`
                    });

                    //查询
                    connection.query(sql, (err, rows, fields) => {

                        if (err) throw err;


                        res.end(JSON.stringify(rows));

                        process.send({
                            cmd: Query,
                            msg: `[child] 查询结果为=>${JSON.stringify(rows)}.`
                        });

                        connection.release();

                    });


                });
            } catch (e) {
                reback = {status: 0, err: "Format problem"};
                res.end(JSON.stringify(reback));
            }
        });
    });


    /*
     * *
     * 说明:客户端发送deviceID和tel和password和nickName
     *     服务器生成userID
     *
     *
     *
     *
     * */

    app.POST("/saveUserInfo", (req, res) => {


        let chunk = "",
            reback = null;

        req.on("data", data => {
            chunk += data;
        });


        req.on("end", () => {
            try {
                let postData = qs.parse(chunk),
                    pool = app.pool;

                process.send({
                    cmd: Query,
                    msg: `[child] 查询结果为=>${JSON.stringify(postData)}.`
                });

                let sql = `insert into UserDevicebind values  (${null},${postData.tel},${null},${postData.password})`;

                pool.getConnection((err, connection) => {

                    process.send({
                        cmd: Query,
                        msg: `[sql] 全拼结果为=>${sql}.`
                    });

                    //查询
                    connection.query(sql, (err, rows, fields) => {

                        if (err) throw err;

                        reback = {userID: postData.tel, status: 1, msg: "success"};
                        res.end(JSON.stringify(reback));


                        process.send({
                            cmd: Query,
                            msg: `[child] 查询结果为=>${JSON.stringify(rows)}.`
                        });
                        connection.release();
                    });
                });
            } catch (e) {
                reback = {userID: postData.tel, status: 0, msg: "fail"};
                res.end(JSON.stringify(reback));
            }
        });


    });


    /*
     * *
     *
     * 说明：登录功能
     *
     *  服务器校验password
     *
     *
     *
     * */


    app.POST("/login", (req, res) => {


        let chunk = "",
            reback = null;

        req.on("data", data => {
            chunk += data;
        });


        req.on("end", () => {
            try {
                let postData = qs.parse(chunk),
                    pool = app.pool;

                process.send({
                    cmd: Query,
                    msg: `[child] 查询结果为=>${JSON.stringify(postData)}.`
                });

                let sql = `select password from UserDevicebind where userID = ${postData.userID}`;

                pool.getConnection((err, connection) => {

                    process.send({
                        cmd: Query,
                        msg: `[sql] 全拼结果为=>${sql}.`
                    });

                    //查询
                    connection.query(sql, (err, rows, fields) => {

                        if (err) throw err;

                        process.send({
                            cmd: Query,
                            msg: `[child] 查询结果为=>${JSON.stringify(rows)}.`
                        });

                        if (rows[0] && rows[0].password == postData.password) {

                            reback = {userID: postData.userID, password: postData.password, status: 1, msg: "success"};
                            res.end(JSON.stringify(reback));

                        } else {

                            reback = {userID: postData.userID, password: postData.password, status: 0, msg: "fail"};
                            res.end(JSON.stringify(reback));

                        }

                        connection.release();
                    });
                });
            } catch (e) {
                reback = {userID: postData.userID, password: postData.password, status: 0, msg: "fail"};
                res.end(JSON.stringify(reback));
            }
        });


    });


    /*
     * *
     *
     * 说明：绑定设备ID和用户ID
     *
     *  update UserDevicebind
     *
     *  insert 默认为零的数据
     *
     *
     *
     * */

    app.POST("/bindUserAndDev", (req, res) => {


        let chunk = "",
            reback = null;

        req.on("data", data => {
            chunk += data;
        });

        req.on("end", () => {
            try {
                let postData = qs.parse(chunk),
                    pool = app.pool;

                process.send({
                    cmd: Query,
                    msg: `[客户端传送数据] 结果为=>${JSON.stringify(postData)}.`
                });

                // 更新sql
                let sqlUpdate = `update UserDevicebind set  deviceID = \"${postData.deviceID}\" where userID = ${postData.userID}`;

                pool.getConnection((err, connection) => {

                    process.send({
                        cmd: Query,
                        msg: `[sqlUpdate] 全拼结果为=>${sqlUpdate}.`
                    });

                    //查询
                    connection.query(sqlUpdate, (err, rows, fields) => {

                        if (err) throw err;

                        process.send({
                            cmd: Query,
                            msg: `[child] 查询结果为=>${JSON.stringify(rows)}.`
                        });

                        let timestamp = (new Date().getTime()).toString().substring(6);

                        //插入sql
                        let sqlInsert = `insert into devices values (\"${postData.deviceID}\",0,0,0,0,${timestamp},\"开关_${postData.deviceID}\")`;


                        process.send({
                            cmd: Query,
                            msg: `[sql插入语句] 结果为=>${sqlInsert}.`
                        });


                        connection.query(sqlInsert, (err, rows, fields) => {

                            if (err) throw err;



                            process.send({
                                cmd: Query,
                                msg: `[sql插入] 结果为=>${JSON.stringify(rows)}.`
                            });


                            reback = { status: 1, msg: "success"};
                            res.end(JSON.stringify(reback));

                            connection.release();
                        });
                    });
                });
            } catch (e) {
                reback = { status: 0, msg: "fail"};
                res.end(JSON.stringify(reback));
            }
        });


    });


    return app;
}

module.exports = Router;