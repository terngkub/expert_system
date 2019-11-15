
find_package(Boost COMPONENTS program_options REQUIRED)
if (Boost_FOUND)
    include_directories((${Boost_INCLUDE_DIRS}))
	add_executable(expert_system
		ast.hpp
		expert_system.cpp
		expert_system.hpp
		fact.cpp
		fact.hpp
		grammar.hpp
		main.cpp
		options.cpp
		options.hpp
		parser.cpp
		parser.hpp
		rule.cpp
		rule.hpp
	)
    target_link_libraries(expert_system ${Boost_LIBRARIES})
endif()