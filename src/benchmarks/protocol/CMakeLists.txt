add_executable(protocol_benchmark "protocol_benchmark.cpp")

target_link_libraries(protocol_benchmark PRIVATE shared)

target_link_libraries(protocol_benchmark PRIVATE benchmark::benchmark)
