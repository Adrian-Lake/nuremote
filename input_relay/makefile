SRCS = $(shell find . -name *.cpp)

all: $(IR_TARGETS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

rpc_client.so: rpc_common.o
	$(CXX) $(CXXFLAGS) -fPIC -shared rpc_common.o rpc_client.cpp -o $@

ir_host: rpc_common.o rpc_host.o
	$(CXX) $(CXXFLAGS) rpc_common.o rpc_host.o ir_host.cpp -o $@ -lX11 -lXtst

# convenience target
ir_clean: clean

clean:
	rm -f *.o
	rm -f $(IR_TARGETS)

