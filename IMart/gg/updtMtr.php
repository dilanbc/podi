<?php


//$accNo = $_GET["acc_no"];
//$crd = $_GET["crd"];
$datastring = $_POST["params"];

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "UPDATE customers SET curread='$datastring' WHERE accno='4'";

$sql1 = "SELECT * FROM customers WHERE accno='4'";
$result = $conn->query($sql1);

$st = "NO";


if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
		$st = $row["status"];
	}
}

if($st=="NO"){
	 echo $st;
	}
else{
	echo "";
	}


if ($conn->query($sql1) === TRUE) {
  //  echo $st;
} else {
    //echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>