
# Project 4 - Patch the linux

## Introdução
O objetivo desse projeto foir realizar a adicao de duas chamadas de sistema no kernel linux (getuserweight e setuserweight). Para isso foi preciso enteder algumas chamadas e conceitos nao triviais do sistema alem de enteder a funcionalidade de outras funcoes definidas no proprio kernel.

## Funcionamento

Para realizar a chamada de sistema seguimos o tutorial do LKcamp para [criacao de chamadas de sistema](https://docs.lkcamp.dev/unicamp_group/systemcalls/) on de modificamos os seguinte arquivos:

```BASH
staging/arch/x86/entry/syscalls/syscall_64.tbl

staging/include/linux/syscalls.h

staging/include/uapi/asm-generic/unistd.h

staging/kernel/Makefile

staging/kernel/sys_ni.c

staging/kernel/get_n_setuserweight.c
```

adicionando as duas chamadas de sistema representadas como chamadas 440 (set) e 441 (get) dentro da branch de linux que nos utilizamos.

O arquivo `get_n_setuserweight.c` eh a implementacao das duas chamadas de sistema propostas e pod ser encontrado neste mesmo repositorio, junto com o arquivo de testes. 

Para aplicar as modificacoes, basta utilizar o arquivo .patch neste mesmo repositorio e aplica-lo na branch staging do mantenedor gregkh.

## Explicacao

Para um explicacao de tudo que foi feito basta clicar nesse [video](#)

## Reproducao

Para clonar a brach basta utiliza o seguinte comando:

```BASH
git clone git://git.kernel.org/pub/scm/linux/kernel/git/gregkh/staging.git
```
em seguida criar um branch sua a partir da master, adicionar o arquivo .patch e aplica-lo com o comando:

```BASH
git apply --reject --whitespace=fix changes.patch
```
no diretorio raiz do kernel baixado.

OBS: vale lembrar que eh preciso verificar a arquitetura da maquina que voce esta utilizando, caso seja algo diferente de x86, cheque o tutorial do LKcamp para fazer a modificacao no arquivo `syscall_XX.tbl` correto. Alem disso vale lembrar que o numero de chamadas de sistema podem mudar em versoes futuras do linux, logo cheque para ver se os indices 440 e 441 estao disponiveis. Caso n estejam adicione as chamdas de sistema nos dois numeros seguintes da ultima chamada registrada.

Apos realizar o tutorial de instalacao do kernel e aplicar o arquivo .patch, basta inicializar o virtme com permissao de leitura e escrita no diretorio em que salvou o `teste.c` deste repositorio

```BASH
virtme-configkernel --defconfig

make ARCH=x86 -j4

virtme-run --kdir=/caminho/para/o/kernel/instalado --rwdir=/caminho/para/seu/diretorio/com/o/teste
```

em seguida va para o diretorio com o teste dentro do terminal do kernel instalado compile o codigo com o comando:

```BASH
gcc teste.c -o teste
```

e execute o teste com o comando

```BASH
./teste uid peso
```

onde uid e peso sao os valores definidos por voce.

# Curiosidades

Para a implementacao das chamdas de sistema foram utilizadas algumas funcoes e palavras especiais do proprio linux, entao vamos deixa-las explicadas aqui:

SYSCALL_DEFINEn : Eh um macro utilizado dentro do kernel para facilitar a definicao de chamdas de sistema. o n indica o numero de argumentos da chamda, podendo ir de 0 ate 6 (o numero de registradores livres nas arquiteturas comums), sendo que qualquer chamada que precise de mais dados precisa utilizar um ponteiro de struct.

current_uid: uma funcao definida em `staging/include/linux/cred.h` que consiste em um macro (ou abreviacao) para conseguir acessar o uid do usuario atual atraves do current_cred que eh uma struct com as credencias da tarefa sendo executada no momento

current_user_ns: macro que retorna o namespace do usuario atual. Eh definida no arquivo `staging/include/linux/uidgid.h`. Nao conseguimos informacao mais aprofundada sobre essa macro.

from_kuid_munged: funcao definida em `staging/include/linux/uidgid.h` recupera o uid do usuario atual com base nas outras duas macros acima passadas como parametro. Ele garante que nao aconteca um overflow de uid. Ou seja, evita que utilizemos um uid fora da faixa estipulada.

EACCES: Palavra especial definida em `/usr/include/asm-generic/errno-base.h` apenas indica o numero de erro correto seguindo a convencao do linux. No caso, este eh o erro de permissao negada (13).

EINVAL: palavra especial definida em `/usr/include/asm-generic/errno-base.h` apenas indica o numero do erro correto seguindo a convencao do linux. No caso, este eh o erro de argumento invalido (22).
