cmake_minimum_required(VERSION 3.19)
project(ConfigParser)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 98)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

set(HEADER_FILES
        config/Config.h
        config/Section.h
        config/detail/Parser.h
        config/detail/Node.h
        config/exceptions/ConfigException.h
        config/exceptions/ParseException.h
        config/exceptions/PathException.h
        config/exceptions/ValueException.h
        util/String.h
        util/NonCopyable.h
        util/Singleton.h
        log/Log.h
        log/Level.h
        log/Formatters.h
        log/detail/LoggerBase.h
        log/detail/Message.h
        log/handlers/AHandler.h
        log/handlers/ConsoleHandler.h
        log/handlers/ColorConsoleHandler.h
        log/handlers/FileHandler.h
        http/RequestParser.h)

set(SOURCE_FILES
        config/Config.cpp
        config/Section.cpp
        config/detail/Parser.cpp
        config/exceptions/ConfigException.cpp
        config/exceptions/ParseException.cpp
        config/exceptions/PathException.cpp
        config/exceptions/ValueException.cpp
        util/String.cpp
        log/Level.cpp
        log/Formatters.cpp
        log/detail/LoggerBase.cpp
        log/detail/Message.cpp
        log/handlers/ConsoleHandler.cpp
        log/handlers/ColorConsoleHandler.cpp
        log/handlers/FileHandler.cpp
        http/RequestParser.cpp)

add_executable(ConfigParser main.cpp ${HEADER_FILES} ${SOURCE_FILES})