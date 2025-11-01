CC = gcc

OBJDIR = output

SERVER_TARGET = server
CLIENT_TARGET = client

all: client server test

client:
	@mkdir -p $(OBJDIR)
	@$(CC) -o $(OBJDIR)/$(CLIENT_TARGET) client.c

server:
	@mkdir -p $(OBJDIR)
	@$(CC) -o $(OBJDIR)/$(SERVER_TARGET) server.c

test:
	@echo "\n		ТЕСТ 1"
	@cp tests/case_no_define/* .
	@./$(OBJDIR)/$(CLIENT_TARGET)
	@./$(OBJDIR)/$(SERVER_TARGET)
	@rm -rf 123.txt meow.c 

	@echo "------------------------------------------------------"

	@echo "		ТЕСТ 2"
	@cp tests/case_with_define/* .
	@./$(OBJDIR)/$(CLIENT_TARGET)
	@./$(OBJDIR)/$(SERVER_TARGET)
	@rm -rf info.txt kate_the_best.h text.txt without_define.c

run_docker:
# 	docker build -t os-lab-mq .            
	docker run --rm -it os-lab-mq

clean:
	@rm -rf $(OBJDIR)