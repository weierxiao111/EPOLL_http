ROOT_PATH=$(shell pwd)
INCLUDE=$(ROOT_PATH)/include 
LIB=$(ROOT_PATH)/lib
LDFLAGS=-lmysqlclient
cc=g++

.PHONY:all
all:insert_cgi select_cgi question_cgi getdata_cgi clear_cgi result_cgi
insert_cgi:insert_cgi.o sql.o
	$(cc) -o $@ $^ -L$(LIB) $(LDFLAGS)
select_cgi:select_cgi.o sql.o
	$(cc) -o $@ $^ -L$(LIB) $(LDFLAGS)
question_cgi:question_cgi.o sql.o
	$(cc) -o $@ $^ -L$(LIB) $(LDFLAGS)
getdata_cgi:getdata_cgi.o sql.o
	$(cc) -o $@ $^ -L$(LIB) $(LDFLAGS)
clear_cgi:clear_cgi.o sql.o
	$(cc) -o $@ $^ -L$(LIB) $(LDFLAGS)
result_cgi:result_cgi.o sql.o
	$(cc) -o $@ $^ -L$(LIB) $(LDFLAGS)
%.o:%.cpp
	$(cc) -c $< -I$(INCLUDE)

.PHONY:clean
clean:
	rm -f *.o insert_cgi select_cgi question_cgi getdata_cgi clear_cgi result_cgi

