$file = 'vsFIGHTER-Firmware.hex'
$entry = Get-CimInstance -Class Win32_SerialPort -Filter "Name LIKE '%Arduino Leonardo bootloader%'" | Select-Object DeviceID
if ($null -eq $entry) {
	Write-Output 'vsFIGHTER-Firmware not found. Press the reset button on the bottom twice then try again.'
	exit
}

$port = $entry.DeviceID
& ".\avrdude.exe" -v -C".\avrdude.conf" -patmega32u4 -cavr109 -P $port -b57600 -D "-Uflash:w:${file}:i"
