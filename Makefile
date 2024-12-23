CC = g++

INCLUDE = -Iinclude -IMy_logging_system/include

CFLAGS =  -lm -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

all: list.out

list.out: build/main.o build/my_list.o build/my_log.o build/list_dump.o
	make My_logging_system/Makefile
	@$(CC) $(CFLAGS) $(INCLUDE) build/main.o build/my_list.o My_logging_system/build/my_log.o build/list_dump.o -o list.out

build/main.o: src/main.cpp
	@$(CC) $(CFLAGS) $(INCLUDE) -c src/main.cpp -o build/main.o

build/my_list.o: src/my_list.cpp include/my_list.h
	@$(CC) $(CFLAGS) $(INCLUDE) -c src/my_list.cpp -o build/my_list.o

build/list_dump.o: src/list_dump.cpp include/my_list.h
	@$(CC) $(CFLAGS) $(INCLUDE) -c src/list_dump.cpp -o build/list_dump.o



run:
	./list.out

clean:
	rm list.out

