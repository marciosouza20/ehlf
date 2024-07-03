/* Este programa executa um find para gerar uma lista dos tipos de arquivo hospedados no home
 * dos usuarios do sistema
 * Original copiado de https://stackoverflow.com/questions/646241/c-run-a-system-command-and-get-output 
 * Adaptado por Marcio de Souza Oliveira <m.desouza20@gmail.com> */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

char *find_executable_in_path(const char *binary) {
    char *path_env = getenv("PATH");
    if (!path_env) {
        return NULL;
    }

    char *path = strdup(path_env);
    if (!path) {
        return NULL;
    }

    char *dir = strtok(path, ":");
    while (dir) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, binary);

        if (access(full_path, X_OK) == 0) {
            free(path);
            return strdup(full_path);
        }

        dir = strtok(NULL, ":");
    }

    free(path);
    return NULL;
}

int main() {
    char *binary_path = find_executable_in_path("ls");
    if (!binary_path) {
        fprintf(stderr, "Executable not found in PATH\n");
        return EXIT_FAILURE;
    }

    char *argv[] = { binary_path, "-l", NULL };

    if (execve(binary_path, argv, environ) == -1) {
        perror("execve");
        free(binary_path);
        exit(EXIT_FAILURE);
    }

    // This line will not be reached if execve is successful
    printf("execve failed\n");
    free(binary_path);
    return EXIT_FAILURE;
}
