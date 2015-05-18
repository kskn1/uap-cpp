CC = g++-4.9
LDFLAGS = -shared -lyaml-cpp
CLIB_FLAGS = -std=c++1y -fPIC -Wall -Werror -g -O2 -I../yaml-cpp/include/
TARGET_LIB = libua_parser.so
TARGET_DIR = ./obj

LIB_SRC = ua_parser.cpp
LIB_OBJ = $(LIB_SRC:%.cpp=$(TARGET_DIR)/%.o)

$(TARGET_DIR)/%.o: %.cpp
	$(CC) -c $(CLIB_FLAGS) $^ -o $(TARGET_DIR)/$*.o $(LDFLAGS)
	@$(CC) -MM $(CLIB_FLAGS) $^ $(LDFLAGS) > $(TARGET_DIR)/$*.d
	
$(TARGET_LIB): $(LIB_OBJ)
	$(CC) $(CLIB_FLAGS) $(LDFLAGS) -o $(TARGET_DIR)/$@ $^

check_dir:
	mkdir -p $(TARGET_DIR)

all: check_dir $(TARGET_LIB) 

clean:
	rm -rf $(TARGET_DIR)/*.d $(TARGET_DIR)/*.o $(TARGET_DIR)/$(TARGET_LIB) 
