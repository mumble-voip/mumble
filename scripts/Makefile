BIN=protoc-gen-grpcwrapper
OUT=MurmurRPC.proto.Wrapper.cpp

../src/murmur/$(OUT): $(BIN)
	protoc --plugin=./$(BIN) -I ../src/murmur/ --grpcwrapper_out=../src/murmur/ ../src/murmur/MurmurRPC.proto

$(BIN): protoc-gen-grpcwrapper.cpp
	$(CXX) -std=c++11 -o $@ $< `pkg-config --libs --cflags protobuf` -lprotoc

clean:
	rm -rf $(BIN)

.PHONY: clean
