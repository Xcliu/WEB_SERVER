#makefile written by human (not by cmake)

sourece_file=hello_world_server

hello_world_server:
	g++ -std=c++11 -o $(sourece_file) $(sourece_file).cpp

run:
	./$(sourece_file)>log.txt

thread_file=thread_create
thread:
	g++ -std=c++11 -o ${thread_file} ${thread_file}.cpp

clean:
	rm ./$(sourece_file) log.txt