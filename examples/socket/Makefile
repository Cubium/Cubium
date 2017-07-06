# Example Code Demonstrations
# 	UDP Socket Client and Server pair
# 		* Open two consoles
# 		* In one console enter 'make run_client'
# 		* In another console enter 'make run_server'
# 		* Both client and serer applications will start.
# 		* When you type things in the client and push enter, they should show up
# 		on the server window


run_client: client
	./client.bin

client:
	g++ -std=c++11 udp_client.cpp -o client.bin

run_server: server
	./server.bin

server:
	g++ -std=c++11 udp_server.cpp -o server.bin -lpthread

clean:
	rm *.bin
