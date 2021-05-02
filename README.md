# Vaccine Shell

A Vaccine Shell é uma shell simples, que implementa alguns comandos específicos e processos de forma background e foreground, mais sobre eles nas seções seguintes. Cada linha de comando é iniciada pela string "vsh> ".

## Como executar

Para compilar a shell, é preciso rodar no terminal os seguintes comandos:

```sh
make
make clean
```

Caso queira rodar o programa sem o uso do valgrind, é possível utilizar ```make run``` ou ```./vsh```. Para rodar o programa com valgrind, é possível utilizar ```make val```.

## Processos

Os processos podem ser ou em background ou em foregroud. Cada processo pode receber até 3 argumentos. Para rodar um programa em foreground é preciso escrever na linha de comando o nome do programa, sem uso de caracteres especiais, da seguinte forma:

```ex.: "./exec arg1 arg2 arg3"```

Para executar processos em background é preciso utilizar o caracter especial " | " para separar os comandos diferentes, na seguinte forma:

```ex. " prog1 | prog2"```

A saída de um processo é a entrada do processo seguinte. O primeiro processo recebe a entrada em stdin e o último processo imprime no stdout.

## Comandos internos

A Vaccine Shell possui dois comandos especiais:

- armageddon;
- liberamoita.

### armageddon

O comando armageddon é utilizado para terminar a execução da shell. Caso ela esteja executando alguns comandos em background, estes são terminados antes de encerrar a shell. Para usar o armageddon, é utilizada a função "quit_shell".

### liberamoita

Caso alguns processos deixarem zombies, o liberamoita pode ser utilizado para "limpar" tudo isso. Obs.: caso o processo pai do zombie ainda esteja em execução, ele não é terminado. Para usar o liberamoita, é utilizada a função "destroy_zombies".

## Sinais e como são tratados 

A shell apresenta um tratador de sinais para os sinais SIGUSR1 e SIGUSR2 (o tratador se encontra na biblioteca vsh_handler, função handle_sigusr_vsh()). Enquanto um sinal do tipo SIGUSR1 ou SIGUSR2 for tratado, os sinais SIGINT, SIGQUIT e SIGTSTP são bloqueados. 

Quando um processo tiver executando em foreground, a shell ignora os sinais SIGINT, SIGQUIT e SIGTSTP. Quando ele termina de executar, a shell volta a receber os sinais.

Um processo em background bloqueia os sinais SIGINT, SIGQUIT e SIGTSTP.

Os sinais SIGUSR1 e SIGUSR2 tem efeitos diferentes em processos foreground e background. Em foreground eles são ignorados, já em background ao receber o sinal, o processo e seus irmãos serão terminados.

## Bibliotecas

Para rodar a Vaccine Shell é preciso ter as bibliotecas padrões de C, além de "vsh_commands", "vsh_errors", "vsh_execute", "vsh_handler", "vsh_io" e "vsh_list".

### vsh commands

Na biblioteca "vsh_commands", há 3 funções referentes a comandos internos da shell e liberação de memória, são elas:

- quit_shell
- destroy_zombies
- destroy_commands

Atráves de ```quit_shell``` é possível encerrar a shell e todos os processos que estavam sendo executados.
A função ```destroy_zombie``` libera os processos zombies.
A função ```destroy_commands``` libera a memória alocada para armazenar as strings referentes aos comandos lidos.

### vsh errors

A biblioteca "vsh_errors" é referente a identificação de erros. Ela possui 5 funções, são elas:

- error_execvp: return 101;
- error_pipe: return 102;
- error_fork: return 103;
- error_dup: return 104;
- error_setsid: return 105.

### vsh execute

A biblioteca "vsh_execute" é referente as funções utilizadas para execução dos processos em background e foreground. Ela apresenta 2 funções (além de 2 estáticas utilizada para as auxiliar), são elas:

- execute_command
- execute_programs

A função ```execute_programs``` é responsável pela execução de todos os processos informados na linha de comando. A função ```execute_command```, utilizada dentro da função anterior, execute apenas um processo, sendo ele background ou foregroud.

### vsh handler

A biblioteca "vsh_handler" é responsável pelas funções tratadoras de sinais, além das configurações das máscaras de sinais. Ela apresenta 5 funções:

- handle_sigusr_vsh
- configure_signals_vsh
- configure_signals_vsh_sigusr
- configure_signals_vsh_ignore
- configure_signals_fg

A função ```handle_sigusr_vsh``` apresenta a seguinte mensagem quando recebe SIGUSR1 ou SIGUSR2:

```
    _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--.
   (.(.)----'`        ^^'                /^   ^^-._
    (    `                 \\             |    _    ^^-._
    VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\\  (_..,______^^-.
     `^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  )
              (((`   (((`          (((`  (((`        `'--'^
I feel weird...
```

As funções ```configure``` são para configurar os tratadores da shell e dos processos foreground. O ```configure_signals_vsh_sigusr``` é responsável por configurar a função de tratamento do SIGUSR1 e SIGUSR2 para a shell. O ```configure_signals_vsh``` faz com que SIGINT, SIGQUIT e SIGTSTP sejam os tratados da forma padrão. O ```configure_signals_vsh_ignore``` faz com que os sinais SIGINT, SIGQUIT e SIGTSTP sejam ignorados pela shell. O ```configure_signals_fg``` faz com que os processos foreground tratem SIGINT, SIGQUIT e SIGTSTP com os tratadores padrões. 

### vsh io

A biblioteca "vsh_io" é responsável pelas funções referentes a entrada e saída como a impressão de "vsh> " na linha de comando, além de leitura dos comandos e gerenciamento dos pipes (open e close). Ela apresenta 4 funções (e 2 estáticas):

- show_command_line
- read_command_line
- close_pipe
- open_pipe

A função ```show_command_line``` é responsável pela impressão de "vsh> ". A função ```read_command_line``` é responsável pela leitura da linha de comando e o tratamento da string lida. A função ```close_pipe``` fecha os pipes (exceto um indicado como parâmetro) e a função ```open_pipe``` abre todos os pipes da matriz de pipe. 

## vsh list

A biblioteca "vsh_list" apresenta a estrutura de dados lista encadeada e suas funções.  

***
Trabalho acessível no [github](https://github.com/luanagabrielescosta/Trabalho-SO), feito por Beatriz Maia, Luana Costa e Sophie Dilhon.