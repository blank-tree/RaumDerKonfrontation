<?php
require_once('settings.php');
$conn = new mysqli(DB_HOST, DB_USER, DB_PW, DB_NAME);
if ($conn->connect_error) {
	die("Connection failed: " . $conn->connect_error);
}

$date = $_GET['date'];
$time = $_GET['time'];
if ($time < 22 && $time > 7) {
	$nextHour = $time - 1;
}
$nextHour = $time < 22 ? $time + 1 : 0;
$success = false;


$sql = "UPDATE " . DB_TABLE . " SET taken = TRUE, timestamp = " . time() . " WHERE date=" . $date .
	" AND time=" . $time;
if ($conn->query($sql) == true) {
	if ($nextHour != 0) {
		$sql = "UPDATE " . DB_TABLE . " SET taken = TRUE, timestamp = " . time() . " WHERE date=" . $date .
			" AND time=" . $nextHour;
		$conn->query($sql);
	}
} else {
	echo "Error updating record: " . $conn->error;
}

$conn->close();

header("Location: http://" . $_SERVER['SERVER_NAME']);
exit();