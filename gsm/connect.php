<?php
$host='localhost';
$user='id16566998_test';
$pass='*b|1V%^ccz$G+&|O';
$db='id16566998_power';
	
$con=mysqli_connect($host,$user,$pass,$db);
if (!$con) {
	    	die('MySQL ERROR: ' . mysql_error());
			echo 'Not connected';
	}
else{
//echo ' connected';
}
return $con;
?>