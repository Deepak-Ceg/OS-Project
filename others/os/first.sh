#!/bin/bash

blu=$'\e[1;34m'
end=$'\e[0m'
I=0
FOUND=0

if [ "$1" == 'help' ]
then
  echo "Format: info <FILENAME> <INFOTYPE>"
  echo "INFO TYPE:-"
  echo "${blu}size${end} : Displays the size of the file"
  echo "${blu}date${end} : Displays date and time of creation"
  echo "${blu}type${end} : Displays Type of File"
  echo "${blu}perm${end} : Displays Permissions for the File"
  echo "${blu}own${end} : Displays Owner of the File"
  echo "${blu}i${end} : Displays no. of files inside the File" 
  exit 1 
fi

for f in *; do
  ((I++))
  if [ "$f" == "$1" ]
  then
    ((FOUND++))
    break;
  fi
done

  if [ "$FOUND" == "0" ]
  then
    echo "File Not Found !"
    exit 1
  fi

((I++))

if [ "$2" == '' ]
then
  echo "Enter Proper Arguments !"
  exit 1 
fi

echo "${blu}FileName:${end}"
ls -l | awk -v pos="$I" 'FNR == pos{print $9}'

if [ "$2" == 'size' ]
then
  echo "${blu}Size:${end}"
  ls -l | awk -v pos="$I" 'FNR == pos{print $5}' 
fi

if [ "$2" == 'date' ]
then
  echo "${blu}Date Of Creation:${end}"
  ls -l | awk -v pos="$I" 'FNR == pos{print $6 $7}'
  ls -l | awk -v pos="$I" 'FNR == pos{print $8}' 
fi

if [ "$2" == 'type' ]
then
  echo "${blu}File Type:${end}"
  ls -l | awk -v pos="$I" 'FNR == pos{print $1}' | cut -c1
  
  echo "- : File"
  echo "d : Directory"
  echo "l : Link"
  
fi

if [ "$2" == 'perm' ]
then
  echo "${blu}File Permissions:${end}"
  ls -l | awk -v pos="$I" 'FNR == pos{print $1}' | cut -c2-10  
fi

if [ "$2" == 'own' ]
then
  echo "${blu}File Owner:${end}"
  ls -l | awk -v pos="$I" 'FNR == pos{print $3}'  
fi

if [ "$2" == 'i' ]
then
  echo "${blu}Files Inside:${end}"
  ls -l | awk -v pos="$I" 'FNR == pos{print $2}'  
fi


