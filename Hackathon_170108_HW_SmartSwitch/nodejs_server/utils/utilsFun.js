/**
 * Utilities
 **/

var numReqs = 0;

// Keep track of http requests
const track4Reqs = ()=> {
    process.nextTick(()=> {
        console.log('numReqs =', numReqs);
    });
};


exports.track4Reqs = track4Reqs;

const say = (message) => {
    console.log("[SERVER] " + message);
};

exports.say = say;


/*
 *  说明：这个函数需要绑定子进程，所以不用箭头函数
 *
 * */
function messageHandler(msg) {
    if (msg.cmd) {
        switch (msg.cmd) {
            case `notifyRequest`:
                say(`The worker #${this.id} is working.It's pid is #${this.process.pid}`);
                break;
            case `online`:
            case `offline`:
            case `query`:
            case `mysqlonline`:
            case `EnqueueMysql`:
            case `errormysql`:
            case `tcpconnected`:
            case `tcpdisconnected`:
                say(msg.msg);
                break;
            default:
                say("未知错误");
                break;
        }
    }
}

exports.messageHandler = messageHandler;