CC = gcc

PREF_SRC = ./sours
OBJDIR = output

SERVER_TARGET = server
CLIENT_TARGET = client

all: server client test clean

server:
	@mkdir -p $(OBJDIR)
	@$(CC) -o $(OBJDIR)/$(SERVER_TARGET) $(PREF_SRC)/server.c

client:
	@mkdir -p $(OBJDIR)
	@$(CC) -o $(OBJDIR)/$(CLIENT_TARGET) $(PREF_SRC)/client.c

test:
	@$(OBJDIR)/$(SERVER_TARGET) &
	@$(OBJDIR)/$(CLIENT_TARGET)                   

run-docker:
	docker build -t os-lab-3 .            
	docker run --rm -it os-lab-3

clean:
	@rm -rf $(OBJDIR) socket_file