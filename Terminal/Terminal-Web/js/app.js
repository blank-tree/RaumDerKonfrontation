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
	var price = 30; // CHF 15.-

	var currentTimeout;
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
		if (currentState == 4 && topic == '/moneyCollected') {
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
		client.publish('/language', language);
		currentState++;		
		changeToState();
	});

	// Choose Date
	$('#screen-2 .date-button').click(function() {
		$('#screen-2 .date-button').removeClass('primary').addClass('hollow');
		$(this).removeClass('hollow').addClass('primary');
		appointmentDate = $(this).data('date');
		client.publish('/appointmentDate', appointmentDate);
		$('#screen-2 .forward-button').removeClass('disabled');
	});

	// Choose Time
	$('#screen-3 .time-button').click(function() {
		$('#screen-3 .time-button').removeClass('primary').addClass('hollow');
		$(this).removeClass('hollow').addClass('primary');
		appointmentTime = $(this).data('time');
		client.publish('/appointmentTime', appointmentTime);
		$('#screen-3 .forward-button').removeClass('disabled');
	});

	$('.back-button').click(function() {
		currentState--;
		changeToState();
	});

	$('.forward-button').click(function() {
		currentState++;
		changeToState();
	});

	function checkPayment() {
		if (moneyCollected >= price) {
			currentState++;
			client.publish('/print', '1');
			changeToState(timeoutCheckout);
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

	function changeToState(timeoutTime = timeoutRegular) {
		for (var i = 0; i < 6; i++) {
			if ($('#screen-' + i).css('display') != 'none') {
				$('#screen-' + i).fadeOut(fadeTime);
				setTimeout(fadeToNewScreen, fadeTime);
			}
		}


		// $('.screen:not(#screen-' + nextState).fadeTo(fadeTime, 0, function() {
		// 	if ($(this).css('display') != 'none') {
		// 		$(this).hide();
		// 		console.log('doing stuff');
		// 	}
		// 	if (!foo) {
		// 		$('#screen-' + nextState).show(function() {
		// 			$(this).fadeTo(fadeTime, 1);
		// 		});
		// 		foo = true;
		// 	}
		// });
		if (currentState == 0) {
			resetTVM();
		} else {
			resetTimeout(timeoutTime);
		}
		console.log('changed to state ' + currentState);
	};

	function fadeToNewScreen() {
		$('#screen-' + currentState).show().fadeTo(fadeTime, 1);
	}

	function pauseAllButtons() {
		// TODO: $('.button').
	};


	resetTimeout(timeoutIdle);
	$('#screen-0').show().fadeTo(fadeTime, 1);

});