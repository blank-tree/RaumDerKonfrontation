$(document).foundation();
$(function(){

	// Var
	var price = 20; // CHF 15.-
	var currentState = 0;
	var moneyCollected = 0;
	var language = '';
	var appointmentDate = '';
	var appointmentTime = '';


	// shitr.io Connection
	var client = mqtt.connect('mqtt://b23695cf:36a044b175c04e97@broker.shiftr.io', {
		clientId: 'RdK-iPad'
	});

	client.on('connect', function(){
		console.log('client has connected!');
		client.subscribe('/moneyCollected');
	});

	client.on('message', function(topic, message) {
		console.log('new message:', topic, message.toString());
		if (currentState == 0 && topic == '/moneyCollected') {
			moneyCollected = parseInt(message);
			console.log('money collected: ' + moneyCollected);
			checkPayment();
		}
	});


	// Functionality
	
	$('#appointment-date').click(function() {
		appointmentDate = '09.05.2018';
		client.publish('/appointmentDate', appointmentDate);
	});

	$('#appointment-time').click(function() {
		appointmentTime = '18:00';
		client.publish('/appointmentTime', appointmentTime);
	});

	$('#language').click(function() {
		language = 'de';
		client.publish('/language', language);
	});

	$('#resettvm').click(function() {
		resetTVM();
	});

	function checkPayment() {
		if (moneyCollected >= price) {
			currentState++;
			resetTVM();
		}
	}

	function resetTVM() {
		client.publish('/resettvm', '1');
		currentState = 0;
		language = '';
		appointmentDate = '';
		appointmentTime = '';
	};



});