for file in "./"*{.txt,.dat} ; do { printf "==========\\nArquivo [%s]:\\n" "${file}" ; ./malha < "${file}" ; printf "\\n" ; } ; done
