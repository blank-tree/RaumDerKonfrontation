$(document).foundation();
$(function(){

	// Statemachine:
	// 0 - Welcome & Language Selection
	// 1 - Description
	// 2 - Choose Date
	// 3 - Choose Time
	// 4 - Paying
	// 5 - Checkout

	// Var
	var timeoutIdle = 600000; // 10min
	var timeoutRegular = 60000; // 1min
	var timeoutPaying = 120000; // 2min
	var timeoutCheckout = 10000; // 10sec
	var fadeTime = 500; // 0.5sec
	var price = 20; // CHF 15.-

	var currentTimeout;
	var currentState = 0;
	var moneyCollected = 0;
	var language = '';
	var appointmentDate = '';
	var appointmentDateHold = '';
	var appointmentTime = '';
	var appointmentTimeHold = '';


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
			resetTimeout(timeoutPaying);
			checkPayment();
		}
	});




	// Functionality
	
	// $('#appointment-date').click(function() {
	// 	appointmentDate = '09.05.2018';
	// 	client.publish('/appointmentDate', appointmentDate);
	// });

	// $('#appointment-time').click(function() {
	// 	appointmentTime = '18:00';
	// 	client.publish('/appointmentTime', appointmentTime);
	// });

	// $('#language').click(function() {
	// 	language = 'de';
	// 	client.publish('/language', language);
	// });

	// $('#resettvm').click(function() {
	// 	resetTVM();
	// });

	// Choose Language
	$('#screen-0 .button').click(function() {
		language = $(this).data('language');
		$('.content-' + language).show();
		currentState++;
		changeToState(currentState);
	});

	$('.back-button').click(function() {
		currentState--;
		changeToState(currentState);
	});

	$('.forward-button').click(function() {
		currentState++;
		changeToState(currentState);
	});

	function checkPayment() {
		if (moneyCollected >= price) {
			currentState++;
			resetTimeout(timeoutCheckout);
		}
	};

	function resetTVM() {
		client.publish('/resettvm', '1');
		$('body').fadeTo(fadeTime, 0, function() {
			location.reload();
		});
	};

	function resetTimeout(timeoutTime = timeoutRegular) {
		clearTimeout(currentTimeout);
		if (currentState != 0) {
			currentTimeout = setTimeout(resetTVM, timeoutTime);
		} else {
			currentTimeout = setTimeout(resetTVM, timeoutTime);
		}
	};

	function changeToState(nextState, timeoutTime = timeoutRegular) {
		$('.screen:not(#screen-' + nextState).fadeTo(fadeTime, 0, function() {
			$('.screen:not(#screen-' + nextState).hide();
			$('#screen-' + nextState).show().fadeTo(fadeTime, 1);
		});
		if (nextState == 0) {
			resetTVM();
		} else {
			resetTimeout(timeoutTime);
		}
		console.log('changed to state ' + nextState);
	};

	function pauseAllButtons() {
		// TODO: $('.button').
	};


	resetTimeout(timeoutIdle);
	$('#screen-0').show().fadeTo(fadeTime, 1);

});