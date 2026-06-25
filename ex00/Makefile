NAME = btc
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

MY_SOURCES = srcs/main.cpp
             
MY_OBJECTS = $(MY_SOURCES:srcs/%.cpp=obj/%.o)

all: $(NAME)

obj/%.o: srcs/%.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(MY_OBJECTS)
	$(CXX) $(CXXFLAGS) $(MY_OBJECTS) -o $(NAME)

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re