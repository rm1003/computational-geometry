teste () (
    local file;
    local ansfile;
    for file in "./trab3/tests/inputs"/* ; do {
        printf "Testando arquivo \"%s\"...\\n" "${file}";
        ansfile="./trab3/tests/answers/${file#./trab3/tests/inputs/}"
        ansfile="${ansfile%.in}.out"
        ./trab3/bsp < "$file" | diff "${ansfile}" - ;
        local status=$?;
        if [ "${status}" -eq 0 ]; then
            printf "\\e[92m✓ Teste bem-sucedido (status %s)\\e[39m\\n" "${status}";
        else
            printf "\\e[91m× Erro no teste (status %s)\\e[39m\\n" "${status}";
        fi;
        printf "========\\n";
    }; done
)
teste
