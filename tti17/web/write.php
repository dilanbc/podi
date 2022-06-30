
<?php
// this page send and get value to microcontroller
include ("connect.php");
//include ("read.php");



if(isset($_POST["stateupdatew"])) {
	$up= $_POST["stateupdatew"];
$sql = "UPDATE meter SET state='$up' WHERE ID='1'"; 
$quary=mysqli_query($con,$sql);
if($quary){

echo 'state added';
}
}

if(isset($_POST["unitw"])) {
	$unitw= $_POST["unitw"];
$sql = "UPDATE meter SET unit='$unitw' WHERE ID='1'"; 
$quary=mysqli_query($con,$sql);
if($quary){

echo 'unitAD'.$states;
}
}

if(isset($_POST["wattw"])) {
	$wattw= $_POST["wattw"];
$sql = "UPDATE meter SET watt='$wattw' WHERE ID='1'"; 
$quary=mysqli_query($con,$sql);
if($quary){

echo 'wattAD';

}
$sql = "SELECT*FROM meter  ORDER BY ID DESC LIMIT 1  ; ";

$result = mysqli_query($con,$sql);

$resultCheck=mysqli_num_rows($result);
if($resultCheck>0){
	while($row=mysqli_fetch_assoc($result)){
		$state=$row["state"];
		echo $state;
	}

}


}

?>