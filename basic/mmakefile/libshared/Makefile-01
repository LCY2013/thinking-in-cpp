TARGET = main
TARGETOBJ = main.o
OBJS = reply.o
LIB = libreply.so
CXXFLAGS = -g -Wall -O2 -c -fPIC

.PHONY: clean
$(TARGET): $(LIB) main.o
	$(CXX) main.o -o $@ -L. -lreply -Wl, -rpath ./
#	$(CXX) main.o -o $(TARGET) -L. -lreply -Wl, -rpath ./
$(LIB): $(OBJS)
	$(CXX) -shared $(OBJS) -o $(LIB)
#	$(CXX) -shared $(OBJS) -o $@
reply.o: reply.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

#. : %.o
#	$(CXX) -shared -o $@ $^
#
#%.o: %.cpp
#	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJS) $(LIB) $(TARGETOBJ)

