<?php

header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
try
{
	function validatePlayer($a_playerQueryRes, $a_hash, $a_coinsWon, $a_coinsBet, &$a_outPlayerRow)
	{
		if(mysqli_num_rows($a_playerQueryRes) != 1)
			return false;
		$a_outPlayerRow = $a_playerQueryRes->fetch_assoc();
		if($a_coinsBet <= 0 || $a_coinsWon < 0)
			return false;
		if($a_coinsBet > intval($a_outPlayerRow["Credits"]))
			return false;
		if(empty($a_hash))
			return false;
		$weakHash = md5($a_outPlayerRow["PlayerID"].$a_outPlayerRow["SaltValue"]);		
		return strcmp($weakHash, $a_hash) == 0;
	}
	
	$link = mysqli_connect("localhost", "testuser", "my_password", "Problem2");
	
	if (!$link || $link->connect_error) 
	{
		error_log("Error: Invalid DB Connection", 0);
		echo json_encode(array());
		exit;
	}
	$playerID = mysqli_real_escape_string($link, $_GET['playerID']);
	$hash = mysqli_real_escape_string($link, $_GET['hash']);
	$coinsWon = intval(mysqli_real_escape_string($link, $_GET['coinsWon']));
	$coinsBet = intval(mysqli_real_escape_string($link, $_GET['coinsBet']));
	$query = 'SELECT * from Player where PlayerID='.$playerID.';';
	$playerQueryRes = mysqli_query($link, $query);
	$playerRow = null;
	if(!validatePlayer($playerQueryRes, $hash, $coinsWon, $coinsBet, $playerRow))
	{
		error_log("Error: Unable to Validate", 0);
		echo json_encode(array());
	}    
	else
	{
		$newCoinTotal = intval($playerRow["Credits"]) - $coinsBet + $coinsWon;
		$newSpinTotal = intval($playerRow["LifetimeSpins"]) + 1;		
		
		$updatePlayerData = 'UPDATE Player SET Player.Credits='.$newCoinTotal.', Player.LifetimeSpins='.$newSpinTotal.' where PlayerID='.$playerID.';';
		$playerUpdateRes = mysqli_query($link, $updatePlayerData);
		if($playerUpdateRes == null)
		{
			error_log("Error: Unable to Update Player", 0);
			echo json_encode(array());	
		}
		else
		{		
			$avgReturn = $newCoinTotal / $newSpinTotal;
			echo json_encode(array($playerID, $playerRow['Name'], $newCoinTotal, $newSpinTotal, $avgReturn));
		}
		mysqli_free_result($playerUpdateRes);
	}
	mysqli_free_result($playerQueryRes);
	mysqli_close($link);
}
catch(Exception $e)
{
	error_log("Exception ".$e->getMessage(), 0);
	echo '()';
}
?>