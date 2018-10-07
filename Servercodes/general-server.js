// I will insert comments to show you guys how to write JavaScript code
// to be consistent with everyone else

// First thing is to write any modules you will import/require
const net = require('net');

// Second thing is to write any defined values you will have
const HOST = '127.0.0.1';
const PORT = 6969;

// You will write any functions after
// The following is the way I want everyone to document functions
// Make sure to end your description with period "."

/**
 * Prints data given to it.
 * @param {Object} data Data given
 */
function printData(data) {
  console.log(data);
}

// Finally you write your normal code
// Make sure you have esline installed into your Atom editor
// so that all of the code you create
// is consistent with everyone else's

// A lot of the things you are seeing here will be found online
// For this file, you can learn more about net here:
// https://nodejs.org/api/net.html
// However, all we need should be in this file already
const server = net.createServer();
server.listen(PORT, HOST, console.log('Ready'));

server.on('connection', socket => {
  socket.on('data', data => {
    printData(data);
  });
});
