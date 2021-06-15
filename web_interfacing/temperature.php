<?
	$servername = "teamrotoaash.mysql.db";
	$username = "teamrotoaash";
	$password = "Miraculu33";
	$dbname = "teamrotoaash";

	// Create connection
	$conn = new mysqli($servername, $username, $password, $dbname);
	// Check connection
	if ($conn->connect_error) {
	  die("Connection failed: " . "<br>" . $conn->connect_error);
	}

	$sql = "SELECT * FROM temperatures";
	$result = $conn->query($sql);

	if ($result->num_rows > 0) {
	  	// output data of each row
	  	while($row = $result->fetch_assoc())
			$last = $row;

	  	$temps = array_keys($_GET);
		if (empty($temps)) {
			echo "Bain n°1 : " . $last["sonde1"] . "°<br>Bain n°2 : " . $last["sonde2"]. "°<br>Bain n°3 : " .  $last["sonde3"] . "°<br>Bain n°4 : " . $last["sonde4"] . "°<br>";
		} else {
			//echo $temps[0] . " " . $temps[1] . " " . $temps[2] . " " . $temps[3] . " index: " . ($row["number"]) . "<br>";

			$sql = "INSERT INTO `temperatures` (`sonde1`, `sonde2`, `sonde3`, `sonde4`, `timeStamp`, `number`) VALUES ('" . $temps[0] . "', '" . $temps[1] . "', '" . $temps[2] . "', '" . $temps[3] . "', CURRENT_TIMESTAMP, '" . ($result->num_rows + 1) . "')";

		  	if ($conn->query($sql) === TRUE) {
				echo "New record created successfully";
			} else {
				echo "Error: " . $sql . $conn->error;
			}
		}

	} else {
	  echo "0 results";
	}
	$conn->close();


	/*
	}
	INSERT INTO `temperatures`(`sonde1`, `sonde2`, `sonde3`, `sonde4`, `timeStamp`, `number`) VALUES (temp1,temp2,temp3,temp4,DEFAULT,i)
	unset($temp);*/
?>