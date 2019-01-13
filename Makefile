FLAGS=-pedantic -Wall -Wextra -Wcast-align -Wcast-qual \
      -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 \
      -Winit-self \
      -Wmissing-include-dirs -Wold-style-cast \
      -Woverloaded-virtual -Wredundant-decls -Wshadow \
      -Wsign-conversion -Wsign-promo \
      -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused \
      -lasan -fsanitize=address -fno-omit-frame-pointer \
      -std=c++14 -O2

TARGET=clock

all:
	@$(CXX) $(FLAGS) -I . main.cpp -o $(TARGET)

clean:
	@rm -f $(TARGET)

style:
	@find . -iname *.hpp -o -iname *.cpp | xargs clang-format-6.0 -verbose -i -style=file

clang-tidy:
	@bear make all --no-print-directory
	@clang-tidy -header-filter=.* main.cpp
	@rm compile_commands.json

cppcheck:
	@cppcheck -I. --enable=all --language=c++ main.cpp
