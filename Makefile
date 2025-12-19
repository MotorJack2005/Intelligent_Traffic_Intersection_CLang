CFlag = -Wno-implicit-function-decleration


main : main_start

main_start : main.o
	@clear
	@gcc $(CFlag) main.o -o main_start && ./main_start



main.o : main.c
	@gcc $(CFlag) -c terminal.c

clean:
	@echo "REMOVING ALL EXECUTABLE"
	@rm 