collatz: main.c collatz.c cache.c
	gcc -o collatz main.c collatz.c cache.c 

clean:
	rm collatz output.csv
