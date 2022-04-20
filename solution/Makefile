all: main.o INTERPRETATION.o EXECUTION.o PARSER.o SCANNER.o ADDITIONAL.o IDENT.o LEX.o
		g++ main.o LEX.o IDENT.o ADDITIONAL.o SCANNER.o PARSER.o EXECUTION.o INTERPRETATION.o

main.o: main.cpp
		g++ main.cpp -c 
		
INTERPRETATION.o: Interpretation.cpp Interpretation.hpp
		g++ Interpretation.cpp -c 
		
EXECUTION.o: Execution.cpp Execution.hpp
		g++ Execution.cpp -c 
		
PARSER.o: Parser.cpp Parser.hpp
		g++ Parser.cpp -c 

SCANNER.o: Scanner.cpp Scanner.hpp
		g++ Scanner.cpp -c 
		
ADDITIONAL.o: Additional.cpp Additional.hpp
		g++ Additional.cpp -c 
		
IDENT.o: Ident.cpp Ident.hpp
		g++ Ident.cpp -c 	 	

LEX.o:  Lex.cpp Lex.hpp
		g++ Lex.cpp -c 

clean:
		rm -f *.o 


