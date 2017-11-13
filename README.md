## Autores

* Israel Medeiros Fontes
* israel_fontes@live.com
* Matricula: 2016037140
* Apoio do Prof. Selan Rodrigues dos Santos

# Introdução

O programa BARES (Basic ARithmetic Expression Evaluator based on Stacks) é capaz
de receber expressões aritméticas simples, formadas por:
• constantes numéricas inteiras (-32.768 a 32.767);
• operadores (+, −, /, ∗,ˆ, %), com precedência descrita em Tabela 1; e
• parênteses.
E apresentar seus respctivos resultados ou erros.

## Possíveis Erros tratados
1. Unexpected end of input

2. Ill formed integer

3. Missing term

4. Extraneous symbol after valid expression found at column

5. Integer constant out of range beginning

6. Missing closing 

7. Divivion by zero

8. Unhandled error found!

# Compilação
Para compilação do programa use:
> make

O comando make cria os diretórios necessários para compilação e compila o programa.

Geração de documentação:
> make doxy

# Executando 
Para executar o programa a partir de um arquivo com as contendo as expressões aritméticas
e salvar os resultdos em um outro arquivo:
> ./bin/bares < [arquivo de entreda] > [arquivo de saída]

[arquivo de entrada]: o caminho do arquivo de contendo as expressões.
[arquivo de sáida]: o caminho do arquivo que irá armazenar os resultados.
