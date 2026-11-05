add_executable(TestNestedTemporaryChannels TestNestedTemporaryChannels.cpp)

set_target_properties(TestNestedTemporaryChannels PROPERTIES AUTOMOC ON)

target_link_libraries(TestNestedTemporaryChannels PRIVATE Qt6::Test mumble_server_lib shared)

target_include_directories(TestNestedTemporaryChannels PRIVATE
	"${CMAKE_SOURCE_DIR}/src/murmur"
	"${CMAKE_SOURCE_DIR}/src"
)

add_test(NAME TestNestedTemporaryChannels COMMAND $<TARGET_FILE:TestNestedTemporaryChannels>)
