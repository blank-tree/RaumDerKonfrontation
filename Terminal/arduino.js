const SerialPort = require('serialport');
const parsers = SerialPort.parsers;

module.exports = function(cb) {
	const parser = new parsers.Readline({
  		delimiter: '\n'
	});

	const port = new SerialPort('/dev/cu.usbmodem1421', {
	  baudRate: 9600  // 57600
	});

	port.pipe(parser);

	parser.on('data', cb);
}
