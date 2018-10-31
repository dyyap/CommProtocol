/*
 * JS -> C++, Frontend -> Backend
 * To run, do the following scripts:
 *
 * node client.js
 */

const net = require('net');
const readline = require('readline');

const HOST = '127.0.0.1';
const PORT = 9200;

const client = net.connect(PORT, HOST);

let rl;

client.on('connect', () => {
  sendMessage();
});

client.on('close', () => {
	if (rl) console.log();
	console.log('Disconnected');
  if (rl) rl.close();
});

client.on('error', () => {
	if (rl) console.log();
	console.log('No server detected');
  if (rl) rl.close();
});

function sendMessage() {
  rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
  });

  rl.question('Message to send (-1 to exit): ', msg => {
    rl.close();
    if (msg !== '-1') {
      client.write(msg);
      sendMessage();
    } else {
      client.end();
    }
  });
}
