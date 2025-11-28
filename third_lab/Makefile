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
	@echo ""
	@echo "Ожидаемые результаты:"
	@echo "- всего файлов в текущем каталоге 12: vscode, output, sours, tests, Makefile, condition.txt, Dockerfile, general.h, .with_dot.txt, without_dot.py, serv.sock, client.sock"
	@echo "- файлов с точки 2: .vscode, .with_dot.txt"
	@echo "- итог: 12 - 2 = 10 файлов без точки"
	@echo ""

	@cp tests/.with_dot.txt .
	@cp tests/without_dot.py .

	@$(OBJDIR)/$(SERVER_TARGET) &
	@$(OBJDIR)/$(CLIENT_TARGET)

	@rm -rf .with_dot.txt without_dot.py
	               
run-docker:
	docker build -t os-lab-3 .            
	docker run --rm -it os-lab-3

clean:
	@rm -rf $(OBJDIR) serv.sock client.sock .with_dot.txt without_dot.py