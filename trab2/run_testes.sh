#!/bin/bash

# Nome do executável
PROGRAM="./malha"

# Verifica se o programa existe
if [ ! -f "$PROGRAM" ]; then
    echo "Erro: O executável '$PROGRAM' não foi encontrado."
    exit 1
fi

# Encontra todos os arquivos .txt no diretório atual
TEST_FILES=$(find . -maxdepth 1 -type f -name "*.txt" | sort)

if [ -z "$TEST_FILES" ]; then
    echo "Nenhum arquivo de teste .txt encontrado no diretório atual."
    exit 0
fi

echo "Executando testes para os arquivos:"
echo "$TEST_FILES"
echo ""
echo ""
echo "----------------------------------------"

# Loop através de cada arquivo de teste
for test_file in $TEST_FILES; do
    echo "Testando: $test_file"
    
    # Executa o programa com o arquivo de teste
    $PROGRAM < "$test_file"
    
    # Verifica o status de saída do programa
    if [ $? -eq 0 ]; then
        echo "SUCESSO: $test_file"
    else
        echo "FALHA: $test_file (Código de saída: $?)"
    fi
    
    echo "----------------------------------------"
    echo ""
    echo ""
    echo "----------------------------------------"

done

echo "Todos os testes foram concluídos."