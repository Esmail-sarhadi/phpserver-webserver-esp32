<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $device = $_POST['device'];
    $temperature = $_POST['temperature'];
    $humidity = $_POST['humidity'];
    $co2 = $_POST['co2'];
    $heart_rate = $_POST['heart_rate'];
    
    // Set the timezone
    date_default_timezone_set('Asia/Tehran');
    
    // Get the current datetime
    $datetime = date("Y-m-d H:i:s");
    
    // File to store data in text format
    $file = 'data.txt';
    
    // Get the current content of the text file
    $currentData = file_get_contents($file);
    
    // Calculate the message number
    $lines = explode("\n", trim($currentData));
    $messageNumber = count($lines) + 1;
    
    // Append new data to the text file
    $currentData .= "$messageNumber, $datetime, Device: $device, Temperature: $temperature, Humidity: $humidity, CO2: $co2, Heart Rate: $heart_rate\n";
    file_put_contents($file, $currentData);
    
    // File to store data in CSV format
    $fileExcel = 'data.csv';
    
    // Get the current content of the CSV file
    $currentDataExcel = file_get_contents($fileExcel);
    
    // Append new data to the CSV file
    $currentDataExcel .= "$messageNumber, $datetime, $device, $temperature, $humidity, $co2, $heart_rate\n";
    file_put_contents($fileExcel, $currentDataExcel);
    
    // Return a confirmation message
    echo "Data received: Message Number: $messageNumber, Datetime: $datetime, Device: $device, Temperature: $temperature, Humidity: $humidity, CO2: $co2, Heart Rate: $heart_rate";
    exit;
}
?>
