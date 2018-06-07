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
<html class="no-js" lang="en">
<head>
	<meta charset="utf-8">
	<meta http-equiv="x-ua-compatible" content="ie=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0">
	<meta name="apple-mobile-web-app-capable" content="yes">
	<meta name="apple-mobile-web-app-status-bar-style" content="black">
	<title>Raum der Konfrontation // Terminal</title>
	<!-- <link rel="stylesheet" href="https://use.typekit.net/jxn5ehv.css"> -->
	<link rel="stylesheet" href="scss/app.min.css">

</head>
<body>
	<div id="blocking"></div>
	<div class="screen" id="screen-0">
		<div class="chose-lang grid">
			<div class="buttons">
				<a class="button" data-language="de">Deutsch</a>
				<a class="button" data-language="en">English</a>
			</div>
		</div>
	</div>

	<div class="screen" id="screen-1">
		<div class="grid">
			<div class="content-de container">
				<h1>Raum der Konfrontation</h1>
				<p>
					Der Prozess der Reflexion ermöglicht uns selbst, unser Handeln und unsere Umgebung besser zu verstehen
					und mitzufühlen. Die Möglichkeit und Zeit diese aktiv zu praktizieren fehlt jedoch oft im Alltag.
					<br><br>
					Nun wurde ein Raum geschaffen um dies zu ändern.<br>
					Eine Stunde alleine und anonym für dich zur Reflexion.
					<br><br>
					Vom 7. bis und mit 21. Juni 2018 besteht die Möglichkeit den «Raum der Konfrontation» zum Betrag von CHF 10.&mdash; zu nutzen.
				</p>
				<div>
					<a class="button back-button">Zurück</a>
					<a class="button forward-button">Fortfahren</a>
				</div>
			</div>
			<div class="content-en container">
				<h1>Room of Confrontation</h1>
				<p>
					The process of reflection enables us to better feel and understand ourselves, our actions and our
					environment. Yet the possibility and time to actively practice this is too often missing in our everyday
					lives.
					<br><br>
					Room has been created to change that.<br>
					One hour alone and anonymous for you to reflect yourself.
					<br><br>
					7. up to the 21. of June there is the possibility to book the «Room of Confrontation» for the amount of CHF 10.&mdash;.
				</p>
				<div>
					<a class="button back-button">Back</a>
					<a class="button forward-button">Continue</a>
				</div>
			</div>
		</div>
	</div>
	<div class="screen" id="screen-2">
		<div class="grid date">
			<div class="container">
				<div class="content-de">
					<h1>Wähle ein Datum:</h1>
				</div>
				<div class="content-en">
					<h1>Choose a date:</h1>
				</div>
				<div class="calendar">
					<h2 class="content-de">Juni</h2>
					<h2 class="content-en">June</h2>
					<?php for ($date = MIN_DATE; $date <= MAX_DATE; $date++):
						$timeframe = date('d') + 1 >= $date
							|| $date == DATE_VERNISSAGE
							|| $date == DATE_FINALSTEST
							|| $date == DATE_FINALS
							|| $date == DATE_FEST;
						?>
						<a class="button date-button <?= $timeframe ? 'disabled' : '' ?>"
							data-date="<?= $date ?>"><?= $date <= 9 ? '0' . $date : $date ?></a>
						<?php endfor; ?>
					</div>
					<div class="content-de">
						<div>
							<a class="button back-button">Zurück</a>
							<a class="button forward-button disabled">Fortfahren</a>
						</div>
					</div>
					<div class="content-en">
						<div>
							<a class="button back-button">Back</a>
							<a class="button forward-button disabled">Continue</a>
						</div>
					</div>
				</div>
			</div>
		</div>
		<div class="screen" id="screen-3">
			<div class="grid date">
				<div class="container">
					<div class="content-de">
						<h1>Wähle die Uhrzeit für deine Stunde:</h1>
					</div>
					<div class="content-en">
						<h1>Choose the time for your hour:</h1>
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
			</div>


			<div class="screen" id="screen-4">
				<div class="grid">
					<div class="container">
						<div class="content-de">
							<h1>Bezahle um Buchung abzuschliessen</h1>
							<h2>Dein Termin:</h2>
							<p>Datum: <span class="checkout-date"></span>. Juni 2018</p>
							<p>Zeit: <span class="checkout-time"></span>:00 - <span class="checkout-endtime"></span>:00</p>
							<p>Ort: (wird auf der Quittung ersichtlich)</p>
							<div class="payment-info">
								<div class="accepted-coins">
									<p>Akzeptierte Münzen:</p>
									<p>5.&mdash; / 2.&mdash; / 1.&mdash;</p>
								</div>
								<div class="to-pay">
									<p>Noch zu zahlen: CHF <span class="payment"></span></p>
									<p>Kein Rückgeld!</p>
								</div>
							</div>
							<div>
								<a class="button back-button">Zurück</a>
							</div>
						</div>
						<div class="content-en">
							<h1>Pay to complete the booking</h1>
							<h2>Your appointment:</h2>
							<p>Date: <span class="checkout-date"></span>. June 2018</p>
							<p>Time: <span class="checkout-time"></span>:00 - <span class="checkout-endtime"></span>:00</p>
							<p>Place: (will be apparent on the receipt)</p>
							<div class="payment-info">
								<div class="accepted-coins">
									<p>Accepted coins:</p>
									<p>5.&mdash; / 2.&mdash; / 1.&mdash;</p>
								</div>
								<div class="to-pay">
									<p>Remaining: CHF <span class="payment"></span></p>
									<p>No change!</p>
								</div>
							</div>
							<div>
								<a class="button back-button">Back</a>
							</div>
						</div>

						<!-- <div class="button" id="pay">PAY!</div> -->

					</div>
				</div>
			</div>

			<div class="screen" id="screen-5">
				<div class="grid">
					<div class="container">
						<div class="content-de">
							<h1>Entnehme deine Quittung</h1>
							<h2>Dein Termin:</h2>
							<p>Datum: <span class="checkout-date"></span>. Juni 2018</p>
							<p>Zeit: <span class="checkout-time"></span>:00 - <span class="checkout-endtime"></span>:00</p>
							<p>Ort: (ist auf der Quittung ersichtlich)</p> <br>
							<h2>Besten Dank für deine Buchung.</h2>
						</div>
						<div class="content-en">
							<h1>Take your receipt</h1>
							<h2>Your appointment:</h2>
							<p>Date: <span class="checkout-date"></span>. June 2018</p>
							<p>Time: <span class="checkout-time"></span>:00 - <span class="checkout-endtime"></span>:00</p>
							<p>Place: (apparent on the receipt)</p> <br>
							<h2>Thank you for your booking</h2>
						</div>
					</div>
				</div>
			</div>

		<script src="node_modules/jquery/dist/jquery.min.js"></script>
		<script src="node_modules/mqtt/dist/mqtt.min.js"></script>
		<script src="js/app.js"></script>
	</body>
	</html>
