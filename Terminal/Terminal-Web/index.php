<?php
require_once('settings.php');
$timetable = array();
$conn = new mysqli(DB_HOST, DB_USER, DB_PW, DB_NAME);
if ($conn->connect_error) {
	die("Connection failed: " . $conn->connect_error);
}
$sql = "SELECT * FROM " . DB_TABLE;
$result = $conn->query($sql);
if ($result->num_rows > 0) {
	while ($row = $result->fetch_assoc()) {
		if (!array_key_exists($row['date'], $timetable)) {
			$timetable[$row['date']] = array();
		}
		$timetable[$row['date']][$row['time']] = $row['taken'];
	}
}
$conn->close();
?>
<!doctype html>
<html>
<head>
	<meta charset="utf-8">
	<meta http-equiv="x-ua-compatible" content="ie=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0">
	<meta name="apple-mobile-web-app-capable" content="yes">
	<meta name="apple-mobile-web-app-status-bar-style" content="black">
	<title>Raum der Konfrontation // Terminal</title>
	<link rel="stylesheet" href="https://use.typekit.net/jxn5ehv.css">
	<link rel="stylesheet" href="scss/app.min.css">

</head>
<body>
	<div id="blocking"></div>

	<div class="screen chose-lang grid" id="screen-0">
		<div class="buttons">
			<a class="button" data-language="de">Deutsch</a>
			<a class="button" data-language="en">English</a>
		</div>
	</div>

	<div class="screen grid" id="screen-1">
		<div class="content-de container">
			<h1>Raum der Konfrontation</h1>
			<p>
				Wir reflektieren; wir konfrontieren; wir fragen; jeden Tag. Wo und warum? Zu welchem Zweck? Mit welchen
				Positionen? Und mit wem, abgesehen von uns selbst?
				<br><br>
				Der Prozess der Reflexion ermöglicht uns selbst, unser Handeln und unsere Umgebung besser zu verstehen
				und mitzufühlen. Die Möglichkeit und Zeit diese aktiv zu praktizieren fehlt jedoch oft im Alltag.
				<br><br>
				Nun wurde ein Raum geschaffen um dies zu ändern.
			</p>
			<div class="">
				<a class="button back-button">Zurück</a>
				<a class="button forward-button">Fortfahren</a>
			</div>
		</div>
		<div class="content-en container">
			<h1>Room of Confrontation</h1>
			<p>
				We reflect; we confront; we ask; every day. Where and why? To what purpose? From which perspective? And
				with whom besides ourselves?
				<br><br>
				The process of reflection enables us to better feel and understand ourselves, our actions and our
				environment. Yet the possibility and time to actively practice this is too often missing in our everyday
				lives.
				<br><br>
				Room has been created to change that.
			</p>
			<div class="">
				<a class="button back-button">Back</a>
				<a class="button forward-button">Continue</a>
			</div>
		</div>
	</div>

	<div class="screen grid date" id="screen-2">
		<div class="container">
			<div class="content-de">
				<h1>Wähle Tag:</h1>
			</div>
			<div class="content-en">
				<h1>Choose a day:</h1>
			</div>
			<div class="calendar">
				<?php for ($date = MIN_DATE; $date <= MAX_DATE; $date++):
					$timeframe = date('d') + 1 > $date;
					?>
					<a class="button date-button <?= $timeframe ? 'disabled' : '' ?>"
						data-date="<?= $date ?>"><?= $date <= 9 ? '0' . $date : $date ?></a>
					<?php endfor; ?>
				</div>
				<div class="content-de">
					<div class="">
						<a class="button back-button">Zurück</a>
						<a class="button forward-button disabled">Fortfahren</a>
					</div>
				</div>
				<div class="content-en">
					<div class="">
						<a class="button back-button">Back</a>
						<a class="button forward-button disabled">Continue</a>
					</div>
				</div>
			</div>
		</div>

		<div class="screen grid date" id="screen-3">
			<div class="container">
				<div class="content-de">
					<h1>Wahle die Startuhrzeit für deine Stunde:</h1>
				</div>
				<div class="content-en">
					<h1>Choose the start time for your hour:</h1>
				</div>

				<?php for ($date = MIN_DATE; $date <= MAX_DATE; $date++): ?>
					<div class="timetables" id="date-<?= $date ?>">
						<div class="calendar time">
							<?php for ($time = MIN_TIME; $time <= MAX_TIME; $time++):
								$taken = $timetable[$date][$time] ?>
								<a class="button time-button <?= $taken ? 'disabled' : '' ?>"
									data-time="<?= $time ?>"><?= $time <= 9 ? '0' . $time : $time ?>:00</a>
								<?php endfor; ?>
							</div>
						</div>
					<?php endfor; ?>

					<div class="content-de">
						<a class="button back-button">Zurück</a>
						<a class="button forward-button disabled">Fortfahren</a>
					</div>
					<div class="content-en">
						<a class="button back-button">Back</a>
						<a class="button forward-button disabled">Continue</a>
					</div>
				</div>
			</div>


			<div class="screen grid" id="screen-4">
				<div class="container">
					<div class="content-de">
						<h1>Bezahlung</h1>
					</div>
					<div class="content-en">
						<h1>Payment</h1>
					</div>

					<div id="payment" class="columns">
						<h1>CHF <span></span></h1>
					</div>
					<div class="button" id="pay">PAY!</div>
					<div class="content-de">
						<a class="button back-button">Zurück</a>
					</div>
					<div class="content-en">
						<a class="button back-button">Back</a>
					</div>
				</div>
			</div>

			<div class="screen grid" id="screen-5">
				<div class="container">
					<div class="content-de">
						<h1>Entnehme deine Quittung</h1>
					</div>
					<div class="content-en">
						<h1>Take your receipt</h1>
					</div>
				</div>
			</div>

		<script src="node_modules/jquery/dist/jquery.min.js"></script>
		<script src="node_modules/mqtt/dist/mqtt.min.js"></script>
		<script src="js/app.js"></script>
	</body>
	</html>
