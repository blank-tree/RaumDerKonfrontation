$(document).foundation();

var socket = io();

socket.on('value', function(msg){
	// TODO: State machine and stuff..

	console.log(msg);
	
});