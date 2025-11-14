CC = gcc

PREF_SRC = ./sources
OBJDIR = output

SERVER_TARGET = server
CLIENT_1_TARGET = client_1
CLIENT_2_TARGET = client_2

all: server client_1 client_2 test clean

server:
	@mkdir -p $(OBJDIR)
	@$(CC) -o $(OBJDIR)/$(SERVER_TARGET) $(PREF_SRC)/server.c

client_1:
	@mkdir -p $(OBJDIR)
	@$(CC) -o $(OBJDIR)/$(CLIENT_1_TARGET) $(PREF_SRC)/client_1.c

client_2:
	@mkdir -p $(OBJDIR)
	@$(CC) -o $(OBJDIR)/$(CLIENT_2_TARGET) $(PREF_SRC)/client_2.c

test:
	@cp tests/*.txt .
	@cp tests/*.c .

	@$(OBJDIR)/$(SERVER_TARGET) &
	@sleep 1
	@$(OBJDIR)/$(CLIENT_1_TARGET)
	@$(OBJDIR)/$(CLIENT_2_TARGET)
	@sleep 1                      

	@rm -rf txt_less_2_blocks.txt txt_more_2_blocks.txt c_more_2_blocks.c c_less_2_blocks.c

run_docker:
	docker build -t os-lab-2 .            
	docker run --rm -it os-lab-2

clean:
	@rm -rf $(OBJDIR)