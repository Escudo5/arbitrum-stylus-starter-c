STACK_SIZE=1024
CC=clang
LD=wasm-ld
CFLAGS=-Iinclude/ -Istylus-sdk-c/include --target=wasm32-wasi -Os -Wall -g
LDFLAGS=-O2 --no-entry --stack-first -z stack-size=$(STACK_SIZE)

NAME = contract.wasm
C_FILES = $(wildcard contracts/*.c) $(wildcard stylus-sdk-c/src/*.c)
OBJECTS = $(patsubst %.c, %.o, $(C_FILES))
HEADERS = $(wildcard include/*.h) $(wildcard stylus-sdk-c/include/*.h)  # Agregamos los archivos .h como dependencias

# Paso 1: Crear los archivos objeto a partir de los archivos .c
$(OBJECTS): %.o: %.c $(HEADERS)  # Incluir los encabezados para la compilación
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Paso 2: Enlazar los objetos en un archivo wasm sin símbolos
contract_unstripped.wasm: $(OBJECTS)
	@$(LD) $(OBJECTS) $(LDFLAGS) -o $@

# Paso 3: Eliminar símbolos de depuración
$(NAME): contract_unstripped.wasm
	@wasm-strip -o $@ $<

# Paso 4: Verificar el archivo wasm con cargo-stylus (descomentado)
check_wasm: $(NAME)
	@cargo stylus check --wasm-file ./contract.wasm -e https://sepolia-rollup.arbitrum.io/rpc

# Paso 5: Desplegar el wasm usando cargo-stylus (descomentado)
deploy_wasm: $(NAME)
	@cargo stylus deploy --wasm-file ./contract.wasm -e https://sepolia-rollup.arbitrum.io/rpc --cargo-stylus-version 0.5.3 --private-key 59e14ed2af60d2bdc3ae630084ebe333b8f0690bdb18decb1dc9e0487e136a6e

# Limpieza de archivos generados
clean:
	@rm -f $(OBJECTS) contract_unstripped.wasm $(NAME)

# Regla de reinicio
re: clean all

# Jugar (ejecutar frontend)
play: all
	@npm run play

# Frontend (compilar frontend)
frontend: all
	@npm run frontend

# Regla de compilación predeterminada
.PHONY: all clean re play frontend check_wasm deploy_wasm
