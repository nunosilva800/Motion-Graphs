all:
	 g++ -O3 -o bin/main Motion\ Graphs/*.cpp -I /usr/include/OIS -I /usr/include/OGRE/ -lOgreMain -lOIS
clean:
	rm bin/main
