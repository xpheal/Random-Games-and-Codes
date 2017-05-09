all:
	g++ load.cpp -o load -O -Wall -std=c++11 -lsqlite3

clean:
	rm -rf nutrients.db nutrients.db-journal load

run:
	./load