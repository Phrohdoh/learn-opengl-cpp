.PHONY = help build run
.DEFAULT_GOAL := help

help:
	@echo ""
	@echo "> make help"
	@echo "  Display this help"
	@echo ""
	@echo "> make build"
	@echo "  Build the project"
	@echo ""
	@echo "> make run"
	@echo "  Run the project"
	@echo ""

build: main.cpp
	@clang++ main.cpp -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -framework Carbon -lGLEW -o a.out

run: build
	@./a.out