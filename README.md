# Problem 1:
The solution and project files are compatible with Visual Studio 2015.
Location highlights:

**Problem1/Problem1** contains the C++ source and header files.

**Problem1/testdata100k.txt** contains an example dataset generated by the Test.exe program

**Problem1/Release/Test.exe** is a precompiled version of the program. Example arguments:

To generate a dataset: 
`Test.exe (new dataset file name) gen (number of birth&death years to generate)`

To use a dataset: 
`Test.exe (dataset file name) use`

**Problem1/sampleoutput.txt** contains output of running Problem1/Release/Test.exe Problem1/testdata100k.txt use"
The program attempts to solve the problem in two ways:
####Search 1: A sequential search of each year from 1900-2000 to find the year wtih the most alive.
####Search 2: A binary search to find the year with the most alive.

# Problem 2:
Location highlights:

**schema.sql** contains a backup of the Player table used in the php script

**slot_machine_spin_results.php** takes GET variables as input. The script validates the input and (if valid) updates the Player table appropriately.
Example:
`slot_machine_spin_results.php?playerID=1&hash=24988d9aa627ea723a4769c83e481a76&coinsWon=100&coinsBet=30` could produce
`["1","One",2167,50,43.34]`
