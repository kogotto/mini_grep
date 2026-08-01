# CMakeList.txt : CMake project for mini_grep, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("mini_grep")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Add source to this project's executable.
add_executable (mini_grep
	main.cpp
	CLogReader.cpp
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET mini_grep PROPERTY CXX_STANDARD 20)
endif()

add_custom_command(
        TARGET mini_grep POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/input
                ${CMAKE_CURRENT_BINARY_DIR}/input)

# TODO: Add tests and install targets if needed.
