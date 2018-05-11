<?php
require_once ('settings.php');
$conn = new mysqli(DB_HOST, DB_USER, DB_PW, DB_NAME);
if ($conn->connect_error) {
	die("Connection failed: " . $conn->connect_error);
}

$date = $_GET['date'];
$time = $_GET['time'];

