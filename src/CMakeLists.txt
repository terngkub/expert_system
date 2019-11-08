
find_package(Boost)
if (Boost_FOUND)
    include_directories((${Boost_INCLUDE_DIRS}))
	add_executable(expert_system
		ast.hpp
		fact.cpp
		fact.hpp
		grammar.hpp
		main.cpp
		parser.cpp
		parser.hpp
		rule.cpp
		rule.hpp
		system.cpp
		system.hpp
	)
    target_link_libraries(expert_system ${Boost_LIBRARIES})
endif()