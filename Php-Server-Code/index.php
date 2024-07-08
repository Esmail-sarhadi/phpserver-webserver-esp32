if ($_SERVER["REQUEST_METHOD"] == "POST") {
  $device = $_POST['device'];
  $temperature = $_POST['temperature'];
  $humidity = $_POST['humidity'];
  $co2 = $_POST['co2'];
  $heart_rate = $_POST['heart_rate'];
  date_default_timezone_set('Asia/Tehran');
  $datetime = date("Y-m-d H:i:s");
  $file = 'data.txt';
  $currentData = file_get_contents($file);
  $lines = explode("\n", trim($currentData));
  $messageNumber = count($lines) + 1;
  $currentData. = "$messageNumber, $datetime, Device: $device, Temperature: $temperature, Humidity: $humidity,
  CO2: $co2, Heart Rate: $heart_rate\n";
  file_put_contents($file, $currentData);
  $fileExcel = 'data.csv';
  $currentDataExcel = file_get_contents($fileExcel);
  $currentDataExcel. = "$messageNumber, $datetime, $device, $temperature, $humidity, $co2, $heart_rate\n";
  file_put_contents($fileExcel, $currentDataExcel);
  echo
  "Data received: Message Number: $messageNumber, Datetime: $datetime, Device: $device, Temperature:
  $temperature, Humidity: $humidity, CO2: $co2, Heart Rate: $heart_rate";
  exit;
}
