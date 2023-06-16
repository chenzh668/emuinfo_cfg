all:
	arm-linux-gnueabihf-g++ main.c cJSON.cpp -o emuinfo.cgi
clean:
	rm -rf emuinfo.cgi