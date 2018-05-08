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

  $('#button').click(function(){
    client.publish('/hello', 'world');
    sendReceipt('09.05.2018', '18:00');
  });


  function sendReceipt(appointmentDate, appointmentTime) {
  	client.publish('/appointmentDate', appointmentDate);
  	client.publish('/appointmentTime', appointmentTime);
  }

})