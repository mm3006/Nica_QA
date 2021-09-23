#!bin/bash

echo "Do you want to create txt files with all data?"
echo "0. No"
echo "1. Yes" 

read a

echo "Do you want to check data for corrupted files?"
echo "0. No"
echo "1. Yes"

read b

echo "Do you want to create QA plots?"
echo "0. No"
echo "1. Yes"

read c

echo "Do you want to create output html files?"
echo "0. No"
echo "1. Yes"

read d

current_dir=$PWD

if [ $a == 1 ]
then
	echo "Creating txt files for all data"
	rm MiniDst.txt
	rm -rf datacc/*

	ls /eos/nica/mpd/sim/data/MiniDst/ >> MiniDst.txt

	while read file
	do
        	echo "Creating "$file".txt"

        	cd /eos/nica/mpd/sim/data/MiniDst/
        	cd $file &> /dev/null
        	cd $(ls) &> /dev/null
        	cd $(ls) &> /dev/null
        	cd $(ls) &> /dev/null
        	cd $(ls) &> /dev/null
        	cd $(ls) &> /dev/null
        	cd $(ls) &> /dev/null
        	cd $(ls) &> /dev/null

        	temp=$(find $PWD -maxdepth 1 -iregex '.+\.root' )

		cd $current_dir
        	echo "$temp" >> dataacc/$file".txt"
		echo "Created "$file".txt"

	done < MiniDst.txt
fi

cd $current_dir

if [ $b == 1 ]
then
	echo "Checking data for corrupted files" 
	rm -rf goodlist.txt
	rm -rf err_files.txt

	ls dataacc >> goodlist.txt

	cd $current_dir
	while read file 

	do

        	echo "Checking "$file
        	root -l -b -q qa_check.C'("'dataacc/$file'")'

	done < goodlist.txt

	cd $current_dir
fi

cd $current_dir

if [ $c == 1 ]
then
	echo "Creating QA plots"
	rm -rf datlist.txt
	ls datagood/ >> datlist.txt

	while read file

	do

		echo "Processing "$file

		root -l -b -q qa_test.C'("'datagood/$file'")'
	done < datlist.txt

fi

cd $current_dir

if [ $d == 1 ]
then
	echo "Deleting old html files. This may take a while."
	rm  rootlist.txt
	rm -rf html/*
	echo "Old html files deleted."
	ls data/ >> rootlist.txt
	filepath=$(echo $PWD)
	while read file

	do
        	echo "Creating "$file".html"

        	nica-report $filepath'/data/'$file $filepath'/html/'$file'.html'

	done <rootlist.txt


	rm -rf dane.tar
	tar -cvf dane.tar html/

fi
