# Triagem no Pronto Atendimento

Sistema de triagem para pronto atendimento, escrito em C. Classifica os pacientes em cinco níveis
de risco, chama sempre o mais urgente e, no empate, quem chegou primeiro.

Problema 1 de Projeto Aplicado III — Ciência da Computação, UnDF.
Arthur Mendes Alcoforado

## Como rodar

Os mesmos quatro comandos valem no Windows (PowerShell), no macOS e no Linux:

```
git clone https://github.com/alcoff14/pa3-problema1-triagem.git
cd pa3-problema1-triagem
gcc main.c cadastro.c fila.c ordenacao.c -o triagem
./triagem
```

É preciso ter o `git` e um compilador de C. Se faltar:

- no macOS, `xcode-select --install`;
- no Linux, `sudo apt install build-essential git`;
- no Windows, o MinGW-W64 e o Git for Windows.

## Como usar

O menu aceita um CPF ou uma das quatro opções:

| Digite | O que acontece |
|---|---|
| um CPF de 11 dígitos | Localiza o cadastro. Se não existir, oferece cadastrar (primeiro nome e data de nascimento). Depois pede a prioridade, de 1 a 5, e coloca o paciente na fila. |
| `1` | Mostra o próximo paciente e pede confirmação (`s` ou `n`) para chamá-lo. |
| `2` | Mostra quantos aguardam e lista a fila, na ordem interna do heap (não na ordem de chamada). Dali, a opção `1` remove quem desistiu, pelo CPF. |
| `3` | Mostra o relatório do dia, por tempo de espera decrescente, com insertion sort (`1`) ou merge sort (`2`). |
| `0` | Encerra. |

Prioridades: 1 emergência, 2 muito urgente, 3 urgente, 4 pouco urgente, 5 não urgente.

Ao digitar, use apenas números no menu (uma letra trava o programa), informe só o primeiro nome e
escreva a data como três números separados por espaço, por exemplo `19 11 1974`. CPFs que começam
com zero não são aceitos.

## Como repetir os testes de tempo

Os testes leem o arquivo `cadastros_1000000.csv`, que vem com o repositório: 1.000.000 de cadastros
fictícios, um por linha, no formato `cpf,nome,dia,mes,ano`.

**Versão final:**

```
gcc gerador.c cadastro.c fila.c ordenacao.c -o gerador
./gerador
```

| Opção | O que mede | Em seguida, digite |
|---|---|---|
| `1` | A busca por CPF e a chamada do próximo paciente | o número de cadastros: `10000`, `100000` ou `1000000` |
| `2` | A ordenação do relatório, com insertion sort e com merge sort | o número de registros: `10000` ou `100000` |
| `0` | Encerra | |

**Versão da Fase 1**, com busca linear e varredura da fila, lendo o mesmo arquivo:

```
git switch fase-1-medicao
gcc main.c -o fase1
./fase1
```

Nessa versão as medições ficam em duas opções do menu que não aparecem na tela: `9` mede a busca e a
chamada, e `8` mede a ordenação. Os números a digitar em seguida são os mesmos da tabela acima. Com
1.000.000 de cadastros, a busca linear leva cerca de 15 segundos. Para voltar à versão final, use
`git switch main`.

Os tempos mudam de uma máquina para outra. O que se repete é a proporção entre os tamanhos e entre
as duas versões.

## Arquivos

| Arquivo | Conteúdo |
|---|---|
| `main.c` | o menu do sistema |
| `cadastro.h`, `cadastro.c` | o cadastro dos pacientes e a tabela hash |
| `fila.h`, `fila.c` | a fila de espera, o heap binário e a lista de atendidos |
| `ordenacao.h`, `ordenacao.c` | insertion sort e merge sort |
| `gerador.c` | o programa que mede os tempos |
| `Makefile` | alternativa para compilar os dois programas de uma vez |
| `cadastros_1000000.csv` | os dados dos testes |
