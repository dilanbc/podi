<?php
$host='localhost';
$user='id16887282_meter1234';
$pass='xNT2lET_$/0/t-OZ';
$db='id16887282_smart123';
	
$con=mysqli_connect($host,$user,$pass,$db);
if (!$con) {
	    	die('MySQL ERROR: ' . mysql_error());
			echo 'Not connected';
	}
else{

}
return $con;
?>