FLAG = -Wall -Wextra -Werror
FILES = 	main.cpp \
			AllServers.cpp \
			Answer.cpp \
			Client.cpp \
			Help.cpp \
			Message.cpp \
			Select.cpp \
			Server.cpp \
			config/Config.cpp \
			config/Section.cpp \
			config/detail/Parser.cpp \
			config/exceptions/ConfigException.cpp \
			config/exceptions/ParseException.cpp \
			config/exceptions/PathException.cpp \
			config/exceptions/ValueException.cpp \
			http/RequestParser.cpp \
			log/Formatters.cpp \
			log/Level.cpp \
			log/detail/LoggerBase.cpp \
			log/detail/Message.cpp \
			log/handlers/ColorConsoleHandler.cpp \
			log/handlers/ConsoleHandler.cpp \
			log/handlers/FileHandler.cpp \
			util/File.cpp \
			util/String.cpp

NAME = webserv_dream_team
OBJ = $(FILES:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@clang++ $(FLAG) --std=c++98 $(OBJ) -o $(NAME)

%.o: %.cpp
	@clang++ -c $< -o $@ $(FLAG)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re