find_package(Boost)
if (Boost_FOUND)
    include_directories((${Boost_INCLUDE_DIRS}))
    add_executable(parser
        parser.cpp
    )
    target_link_libraries(parser ${Boost_LIBRARIES})
endif()