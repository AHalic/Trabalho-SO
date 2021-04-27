#ifndef VSH_EXECUTE_H
#define VSH_EXECUTE_H

/**
 * Executa um processo. Se for um processo bg, eh importante saber sua posicao na linha de comando
 * e a matriz de pipes. 
 * @param {char* command}  String do comando que sera executado (Formato: exce argv)
 * @param {int bg} 0 se for um comando fg, bg > 0 se for bg. Eh o numero de comandos - 1;
 * @param {int* fd[2]} Matriz de pipes com a quantidade de pipes igual ao parametro bg.
 * @param {int pos}
 * 
 * @return Numero do processo do filho.
 */
int execute_command(char* command, int bg, int fd[bg][2], int pos);


/**
 * Executa todos os processos dado um numero de comando e um vetor de string. 
 * @param {int n_commands} Quandtidade de numero de comandos que devem ser executados;
 * @param {char** commands_vector} Vetor de Strings em que cada String eh um executavel + seus argumentos.
 * 
 * @return Numero da sessao dos processos.
 */
int execute_programs(int n_commands, char** commands_vector);

#endif