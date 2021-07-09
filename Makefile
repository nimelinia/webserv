FLAG = -Wall -Wextra -Werror --std=c++98

HEADER =    config/exceptions/PathException.hpp \
            config/exceptions/ConfigException.hpp \
            config/exceptions/ParseException.hpp \
            config/exceptions/ValueException.hpp \
            config/Section.hpp \
            config/detail/Node.hpp \
            config/detail/Parser.hpp \
            config/config_analizator.hpp \
            config/Config.hpp \
            http/parsers/RequestParser.hpp \
            http/parsers/UriParser.hpp \
            http/Client.hpp \
            http/StatusCodes.hpp \
            http/MimeTypes.hpp \
            http/Message.hpp \
            http/ResponseHandler.hpp \
            http/CgiHandler.hpp \
            http/Answer.hpp \
            http/CgiProcess.hpp \
            log/Log.hpp \
            log/Level.hpp \
            log/detail/Message.hpp \
            log/detail/LoggerBase.hpp \
            log/handlers/ConsoleHandler.hpp \
            log/handlers/AHandler.hpp \
            log/handlers/FileHandler.hpp \
            log/handlers/ColorConsoleHandler.hpp \
            log/Formatters.hpp \
            AllowedStd.hpp \
            server/AllServers.hpp \
            server/webserv.hpp \
            server/Select.hpp \
            server/Server.hpp \
            util/File.hpp \
            util/String.hpp \
            util/Singleton.hpp \
            util/NonCopyable.hpp

SOURCE =    config/exceptions/ConfigException.cpp \
            config/exceptions/ValueException.cpp \
            config/exceptions/PathException.cpp \
            config/exceptions/ParseException.cpp \
            config/Config.cpp \
            config/Section.cpp \
            config/detail/Parser.cpp \
            http/parsers/RequestParser.cpp \
            http/parsers/UriParser.cpp \
            http/CgiProcess.cpp \
            http/MimeTypes.cpp \
            http/StatusCodes.cpp \
            http/CgiHandler.cpp \
            http/Message.cpp \
            http/Answer.cpp \
            http/Client.cpp \
            http/ResponseHandler.cpp \
            log/Level.cpp \
            log/detail/Message.cpp \
            log/detail/LoggerBase.cpp \
            log/Formatters.cpp \
            log/handlers/ConsoleHandler.cpp \
            log/handlers/FileHandler.cpp \
            log/handlers/ColorConsoleHandler.cpp \
            main.cpp \
            server/AllServers.cpp \
            server/Select.cpp \
            server/Server.cpp \
            util/String.cpp \
            util/File.cpp


NAME = webserv
OBJDIR = _obj
OBJ = $(SOURCE:%.cpp=$(OBJDIR)/%.o)

all: $(NAME)

$(OBJDIR):
	mkdir $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJ)
	clang++ $(FLAG) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p '$(@D)'
	clang++ $(FLAG) -I. -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re