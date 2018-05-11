<?php
/**
 * inits the mySQL DB
 */

require_once ('settings.php');

$conn = new mysqli(DB_HOST, DB_USER, DB_PW, DB_NAME);

if ($conn->connect_error) {
	die("Connection failed: " . $conn->connect_error);
}

mysqli_query($conn, "TRUNCATE TABLE " . DB_TABLE);

$stmt = $conn->prepare("INSERT INTO " . DB_TABLE . " (date, time, taken) VALUES (?, ?, ?)");
$stmt->bind_param("ssi", $date, $time, $taken);

$taken = 0;

for ($date = MIN_DATE; $date <= MAX_DATE; $date++) {
	for ($time = MIN_TIME; $time <= MAX_TIME; $time++) {
		$stmt->execute();
	}
}

echo "EMPTY DB CREATED";

$stmt->close();
$conn->close();