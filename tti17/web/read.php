<?php
include ("connect.php");

$sql = "SELECT*FROM meter  ORDER BY ID DESC LIMIT 1  ; ";

$result = mysqli_query($con,$sql);

$resultCheck=mysqli_num_rows($result);
if($resultCheck>0){
	while($row=mysqli_fetch_assoc($result)){
	     $unit=$row["unit"]; 
		$watt=$row["watt"];
		$state=$row["state"];
		if($state==1){$states="Off";}
		else{$states="On";}
	}

}
?>