//Fernando Garrote De La Macorra
//A01027503

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void procesos(int *, int *, int, char);

void procesos(int * origen, int * anterior, int faltantes, char testigo) {
    char a;
    if (faltantes == 1) {
        while (1) {
            close(anterior[1]);
            read(anterior[0], &a, sizeof(char));
            printf("\n—-> Soy el proceso con PID %d y recibí el testigo %c, el cual tendre por 5 segundos", getpid(), a);
            sleep(5);
            printf("\n<—- Soy el proceso con PID %d y acabo de enviar el testigo %c", getpid(), testigo);
            write(origen[1], &a, sizeof(char));
        }
    }
    else {
        int siguiente[2];
        pipe(siguiente);
        pid_t pid;
        pid = fork();

        if (pid == 1) {
            printf("\n Hubo un error al crear el proceso hijo");
            exit(1);
        }
        else
          if (pid == 0) {
            procesos(origen, siguiente, faltantes - 1, testigo);
        }
        else {
            while (1) {
                close(anterior[1]);
                read(anterior[0], &a, sizeof(char));
                printf("\n—-> Soy el proceso con PID %d y recibí el testigo %c, el cual tendre por 5 segundos", getpid(), a);
                sleep(5);
                printf("\n<—- Soy el proceso con PID %d y acabo de enviar el testigo %c", getpid(), testigo);
                write(origen[1], &a, sizeof(char));
              }
        }
    }
}

int main(int argc, char * const * argv) {
    char * val = NULL, testigo = 'T';
    int valor = 0, origen[2], indice, dat;

    while ((dat = getopt (argc, argv, "n:")) != -1) {
        switch (dat) {

            case 'n':
                val = optarg;
                if (atoi(val))
                    valor = atoi(val);
                else
                    printf("\n Se requiere un numero valido");
                break;

            case '?':
                if (optopt == 'n')
                    fprintf (stderr, "\n Se requiere un argumento para la opcion -%c", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "\n Opcion '-%c' no valida .", optopt);
                else
                    fprintf (stderr, "\n Opcion desconocida '\\x%x'", optopt);
                return 1;

            default:
                abort();
        }
    }

    if (valor < 1)
        return 1;

    pipe(origen);
    write(origen[1], &testigo, sizeof(char));
    procesos(origen, origen, valor, testigo);

    return 0;
}
