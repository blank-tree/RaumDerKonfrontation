$(document).foundation();
$(function(){
  var client = mqtt.connect('mqtt://b23695cf:36a044b175c04e97@broker.shiftr.io', {
    clientId: 'javascript'
  });

  client.on('connect', function(){
    console.log('client has connected!');
  });

  client.on('message', function(topic, message) {
    console.log('new message:', topic, message.toString());
  });

  $('#appointment-date').click(function() {
  	client.publish('/appointmentDate', '09.05.2018');
  });

  $('#appointment-time').click(function() {
  	client.publish('/appointmentTime', '18:00');
  });

  $('#language').click(function() {
  	client.publish('/language', 'de');
  });

  $('#print').click(function() {
  	client.publish('/print', '1');
  });



});