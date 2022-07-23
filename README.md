# project-5

## O que é

Essa é uma implementação de duas chamadas de sistema, setuserweight e getuserweight, no kernel linux.  setuserweight altera o peso para um usuário e getuserweight retorna o peso associado a um usuário(donos de threads ou processos).
Essas chamadas e pode ter diversas aplicações, como por exemplo no escalonamento de processos de modo que a CPU fosse dividida de acordo com os pesos dos usuários desse sistema operacional.
Este vídeo explica a implementação e os testes: https://youtu.be/tT16Q5pZFv8


## Instruções

O kernel utilizado como base para implementação do projeto foi do usuário greg.

Para obtê-lo, faça o seguinte comando no seu terminal:

> git clone git://git.kernel.org/pub/scm/linux/kernel/git/gregkh/staging.git -b staging-testing --depth=1

O patch com as modificações deste projeto é o arquivo 0001-project-5-patch.patch.

Para aplicar o patch, coloque o arquivo 0001-project-5-patch.patch dentro do repositório clonado acima e rode o seguinte comando:

> git apply --reject --whitespace=fix 0001-project-5-patch.patch


## Implementação

A implementação das duas chamadas de sistema estão em kernel/get_set_userweight.c.
A implementação foi feita considerando a arquitetura x86-64. Se refira ao link abaixo para resolver problemas de compatibilidade com outras arquiteturas:

> https://www.kernel.org/doc/html/latest/process/adding-syscalls.html

## Testes

Os testes são feitos pelo programa tests/main.c.

Antes de executar os testes, veja a seção Dependências abaixo.

Para rodar os testes: execute os seguintes comandos na pasta staging do repositório clonado de greg:

> virtme-configkernel --defconfig

> make -j$(nproc)

E para rodar a máquina virtual, use o path absoluto para a pasta staging/ do repositório clonado e a substitua abaixo por /path/to/kernel e use o path absoluto para a pasta tests/ e a substituia por /path/to/folder/contaning/code abaixo:

> virtme-run --kdir=/path/to/kernel --rwdir=/path/to/folder/contaning/code

Agora você deve estar com a máquina virtual aberta.

Proceda à pasta de testes (mesmo path que no seu computador) e execute:

> gcc main.c -o tests
> ./tests

E os testes serão executado.

## Dependências

É necessário instalar o Virtme. O Virtme é uma máquina virtual para rodar sua própria implementação do kernel linux no seu computador. Referência:

> https://docs.lkcamp.dev/unicamp_group/boot/#install-virtme-as-a-normal-user


## Referências

Tutorial do lkcamp de inicialização do ambiente de desenvolvimento kernel linux:

> https://docs.lkcamp.dev/unicamp_group/boot/

Tutorial do lkcamp de implementação de chamadas de sistema:

> https://docs.lkcamp.dev/unicamp_group/systemcalls/

