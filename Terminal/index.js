var express = require('express');
var http = require('http').Server;
var socket = require('socket.io');
var readline = require('readline');

const arduino = require('./arduino');

let app = express();
let server = http(app);
let io = socket(server);

server.listen(3000, function(){
  console.log('listening on *:3000');
});

arduino((line)=>{
	console.log(line);
	io.emit('value', line == 1);
});

var rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
  terminal: false
});

rl.on('line', function(line){
    io.emit('value', line == "true");
})