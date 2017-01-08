const http = require('http');
const querystring = require("querystring");

// Create an HTTP server
var srv = http.createServer((req, res) => {
    res.writeHead(200, {'Content-Type': 'text/plain'});
    res.end('okay');
});
srv.on('upgrade', (req, socket, head) => {

    console.log(req.httpVersion);
    socket.write('HTTP/1.1 101 Web Socket Protocol Handshake\r\n' +
        'Upgrade: WebSocket\r\n' +
        'Connection: Upgrade\r\n' +
        '\r\n');

    socket.on("data", (d)=> {
        console.log(d.toString())
    });
    socket.on("end", ()=> {
        socket.write("发送服务器的消息");
      socket.end();
        if (!socket.destroyed) {
            console.log(" socket.end 会销毁socket,不信你可以执行end，看会不会进入这个方法");
            if (socket.destroy) {
               // socket.destroy();
                console.log(!socket.destroyed)
            }

        }
    });
});

// now that server is running
srv.listen(1337, '127.0.0.1', () => {

    // make a request
    var options = {
        port: 1337,
        hostname: '127.0.0.1',
        headers: {
            'Connection': 'Upgrade',
            'Upgrade': 'websocket'
        }
    };

    var req = http.request(options);
    req.on('upgrade', (res, socket, upgradeHead) => {
        socket.write('发送客户端的消息\r\n');
        socket.end();

        socket.on('data', (chunk) => {
            console.log(chunk.toString());
        });
        socket.on('end', () => {
            console.log("服务器消息接收完毕");
            console.log("socket已经被销毁",socket.destroyed);
            //socket.write('再次发送客户端的消息\r\n');
        });

    });
    req.end()
});


