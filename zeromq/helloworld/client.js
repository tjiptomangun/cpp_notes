const zmq = require('zeromq');
const zsocket = new zmq.socket('pair');
zsocket.setsockopt(zmq.ZMQ_RECONNECT_IVL, 100);
zsocket.setsockopt(zmq.ZMQ_RCVTIMEO, 1);
zsocket.on('connect', () => {
	console.log('connected');
})
zsocket.on('message', (data)=> {
	console.log(data.toString());
})
zsocket.on('error', (data)=> {
	console.log(data.toString());
})


zsocket.monitor();
zsocket.connect("tcp://127.0.0.1:5555");
setInterval((n) => {
	n++;	
	const message = `Hello ${n}`
	try{ 
		zsocket.send(message);
		console.log('ok');
	} catch(err) {
		console.error(err);
	}
}, 1000, 0)
