<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>Smart Meter</title>
<style type="text/css">
.headcss {
	background-color: #4AEF92;
}
.rawcss {
	background-color: #E4FFEF;
	color: #000000;
	text-align: center;
}
</style>
</head>

<body>
<table width="70%" height="461" border="0" align="center" cellpadding="10" cellspacing="20">
  <tbody >
    <tr>
      <th height="106" colspan="4" class="headcss" style="font-size: 24px;" scope="col">Smart Meter</th>
    </tr>
    <tr class="rawcss">
      <td width="26%" >J. Bandara</td>
      <td width="25%"><?php include ("read.php"); echo $unit; ?>kwh</td>
      <td width="25%"><?php include ("read.php"); echo $watt; ?>W</td>
      <form action="write.php" method="post">
      <td width="26%"><button name="stateupdatew" type="submit"  <?php include ("read.php"); if($state==1){echo'value="0"';}else{echo'value="1"';} echo ">".$states; ?></button></td>
      </form>
    </tr>
    <tr class="rawcss">
      <td>abc12</td>
      <td>567.63kwh</td>
      <td>10W</td>
      <td><input type="button" name="button" id="button" value="On"></td>
    </tr>
    <tr class="rawcss">
      <td>abc34</td>
      <td>23.369kwh</td>
      <td>15W</td>
      <td><input type="button" name="button2" id="button2" value="Off"></td>
    </tr>
    <tr class="rawcss">
      <td>abc56</td>
      <td>2.354kwh</td>
      <td>13W</td>
      <td><input type="button" name="button3" id="button3" value="On"></td>
    </tr>
    <tr class="rawcss">
      <td>abc78</td>
      <td>7.892kwh</td>
      <td>5W</td>
      <td><input type="button" name="button5" id="button5" value="On"></td>
    </tr>
  </tbody>
</table>
</body>
</html>