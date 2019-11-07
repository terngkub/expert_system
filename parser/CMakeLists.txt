find_package(Boost)
if (Boost_FOUND)
    include_directories((${Boost_INCLUDE_DIRS}))
    add_executable(parser
        ast.hpp
        grammar.hpp
        parser.cpp
        printer.hpp
    )
    target_link_libraries(parser ${Boost_LIBRARIES})
endif()