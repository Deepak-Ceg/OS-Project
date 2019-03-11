echo "Enter your choice:"
echo "1) Server"
echo "2) Client"

read a
case $a in
	1 )
	clear
	echo "Welcome!"
	gcc server.c -o lp
	echo "Enter port number:"
	read b
	echo "Server is ready to connect:p"
	./lp $b
	;;
	
	2)
	clear
	echo "Welcome!"
	 gcc client.c -o pos
	echo "Receiver is Ready to Send!!"
	echo "Enter the ip address,port number and filename with extension to send:"
	read m
	read n
	read q
	echo "Transferring Started!"
	./pos $m $n $q
	echo "Transferred Succesfully!"
	;;
esac



	
