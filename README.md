<div align="center">
	<h1> pipex </h1>
	<p> Este proyecto te permitirá descubrir, mediante el uso en tu programa, el funcionamiento de un mecanismo de UNIX que ya conoces.</p>
	<img src="https://wakatime.com/badge/user/a0e860d2-9914-4fed-8143-b9fd5cf5e6c1/project/d29be413-913b-493f-b5e4-ac27d08d4446.svg?style=flat"/>
	<br />
	<img src="https://img.shields.io/badge/norme-OK-success?style=flat"/>
	<img src="https://img.shields.io/badge/leaks-KO-red?style=flat"/>
	<img src="https://img.shields.io/badge/bonus-not%20done-red?style=flat"/>
	<img src="https://img.shields.io/badge/-unknown%2F100-inactive?style=flat&logo=42&logoColor=000" />
	<br />
	<p>(este proyecto no será evaluado, pero es necesario para Minishell, un proyecto posterior)</p>
</div>

<h2 align="center">📜 PROYECT DESCRIPTION 📜</h2>

Este proyecto busca profundizar un poco en dos conceptos que ya conoces: Las redirecciones y los pipes. Es un proyecto de introducción para proyectos de UNIX más grandes que verás más tarde en el cursus.

<h2 align="center">🌲 PROYECT TREE 🌲</h2>

```
.
├── Makefile
├── README.md
├── libft/
├── include/
│   └── pipex.h
├── infile
├── objs/
├── outfile
└── src/
    └── pipex.c

```

<h2 align="center">🚶 PROYECT ROADMAP 🚶</h2>

<h3 align="center"> 🧠 BRAINSTORM PREVIO </h3>

*  Aprender sobre enviroment variables
*  fork y procesos padre e hijo
*  Manuales de las funciones permitidas

<h2 align="center">⚙️ PROYECT GENERAL PROCEDURE ⚙️</h2>

## FUNCIONES AUTORIZADAS:

| FUNCIONES | LIBRERÍA | descripcion | estructura |
| --- | --- | --- | --- |
| open | <fcntl.h> | abre un archivo y devuelve su fd | int    open(char *path, int mode) |
| close | <unistd.h> | cierra un fd | int close(int fd) |
| read | <unistd.h> | lee un trozo del fd | ssize_t read(int fd, void buf[.count], size_t count) |
| write | <unistd.h> | escribe por pantalla | ssize_t read(int fd, void buf[.count], size_t count) |
| malloc | <stdlib.h> | aloja memoria para una variable | void *malloc(size_t size) |
| free | <stdlib.h> | libera memoria alojada | void free(void *ptr) |
| perror | <stdio.h> <errno.h> | printea por pantalla el ultimo error encontrado | void perror(const char *s) |
| strerror | <string.h> | devuelve un string que describe el error encontrado | char *strerror(int errnum) |
| access | <unistd.h> | Comprueba si tiene acceso a una dirección | int access(const char *pathname, int mode) |
| dup | <unistd.h> | Se aloja un nuevo fd a un fd antiguo | int dup(int oldfd) |
| dup2 | <unistd.h> | igual que dup pero indicando el nuevo fd | int dup2(int newfd, int oldfd) |
| execve | <unistd.h> | ejecuta el programa encontrado en el path | int execve(const char *pathname, char *const _Nullable argv[],char *const _Nullable envp[]) |
| exit | <stdlib.h> | Cierra el programa | void exit(int status) |
| fork | <unistd.h> | Crea un subproceso llamado proceso hijo | pid_t fork(void) |
| pipe | <unistd.h> | Crea un canal de comunicación entre subprocesos | int pipe(int pipefd[2]) |
| unlink | <unistd.h> | Elimina un nombre del archivo del sistema | int unlink(const char *pathname) |
| wait | <sys/wait.h> | espera a que termine el proceso hijo | pid_t wait(int *_Nullable wstatus) |
| waitpid | <sys/wait.h> | En caso de tener varios hijos, indica a cual esperar | pid_t waitpid(pid_t pid, int *_Nullable wstatus, int options) |

## EXPLICACIÓN DE LAS FUNCIONES MÁS IMPORTANTES PARA EL PROYECTO:

### FUNCIONAMIENTO DE PIPE():
```bash
// cada cmd necesista un stdin (entrada) y devuelve una salida (a stdout)
   
    🗄infile                                             🗄outfile
  stdin de cmd1                                       stdout de cmd2            
       |                        PIPE                        ↑
       |           |---------------------------|            |
       ↓             |                       |              |
     cmd1⚙️    -->    end[1]      ↔        end[0]   -->    ⚙️cmd2           
                     |                       |
            cmd1   |---------------------------|  end[0]
           output                            recibe de end[1]
         escribe en                         y envia la salida de
          el end[1]                      cmd1 al cmd2 como entrada
```

la función pipe recibe un array con dos numeros que representan la salida y la entrada de la tuberia y las une. De esta forma, lo que representan los numeros del array no es lo que se ha guardado si no fds, de tal manera que array[1] escribe en su propio fd algo que puede ser leido por array[2] y viceversa.

### FUNCIONAMIENTO DE FORK():

Cuando se llama a la función **`fork()`**, el sistema operativo crea un nuevo proceso que es una duplicación del proceso padre. Este nuevo proceso se denomina proceso hijo. El proceso hijo recibe su propio identificador de proceso único (PID), distinto del proceso padre.

Los valores que devuelve fork son:

- -1 si la función ha fallado
- 0 si estamos en el proceso hijo
- más de 0 si estamos en el proceso padre

Al hacer un fork y crear un nuevo proceso, el orden de ejecución del padre y el hijo es cosa nuestra.

Si por ejemplo queremos que se **ejecuten de forma concurrente**, podriamos hacer algo como esto: 

```c
pid_t pid = fork();

	if (pid < 0) {
		// Error occurred
		ft_printf("Fork failed.\n");
		return 1;
	} else if (pid == 0) {
		// Child process
		while (1) {
			ft_printf("A\n");
			sleep(1);
		}
	} else {
		// Parent process
		while (1) {
			ft_printf("B\n");
			sleep(2);
		}
	}
```

Esto hará que cada dos segundos el padre escriba una B y que cada segundo el hijo escriba una A.

Los dos procesos se estarian ejecutando a la vez, cada uno con su propio PID.

```csharp
➜  pipex git:(main) ✗ ./pipex
B
A
A
A
B
A
A
B
A
A
```

Devolvería algo así por la terminal

Si por ejemplo lo que queremos hacer es que **el hijo se ejecute primero**, podemos conseguirlo diciendo al padre que espere:

```c
int	main(void)
{
	pid_t	ppid;

	ppid = fork();
	if (ppid == -1)
	{
		ft_printf("Error en fork");
		exit(EXIT_FAILURE);
	}
	if (ppid == 0)
		ft_printf("Child process\n");
	else
	{
		wait(0);
		ft_printf("Parent process\n");
	}
	return (0);
}
```

Aqui hacemos que el padre termine hasta que el hijo termine de hacer su proceso y despues se ejecutará el del padre. 

Cuando estamos usando `fork()`, está estandarizado que la función `wait(0)` se encarga de esperar a que un proceso hijo termine. En caso de tener varios hijos, se puede especificar cual mediante funciones como `waitpid()`

```csharp
➜  pipex git:(main) ✗ ./pipex
Child process
Parent process
```

Esto es lo que mostraria en terminal el programa anterior.

Por ultimo, puede que queramos que el **padre se ejecute primero** y a continuación los hijos. Hay varias formas de conseguir esto:

```c
int	main(void)
{
	pid_t	ppid;

	ppid = fork();
	if (ppid == -1)
	{
		ft_printf("Error en fork");
		exit(EXIT_FAILURE);
	}
	if (ppid > 0)
		ft_printf("Parent process\n");
	else
	{
		wait(0);
		ft_printf("Child process\n");
	}
	return (0);
}
```

```bash
int	main(void)
{
	pid_t	ppid;

	ppid = fork();
	if (ppid == -1)
	{
		ft_printf("Error en fork");
		exit(EXIT_FAILURE);
	}
	if (ppid == 0)
		ft_printf("Child process\n");
	else
	ft_printf("Parent process\n");
	return (0);
}
```

Ambos codigos anteriores mostrarían en la terminal lo siguiente:

```csharp
➜  pipex git:(main) ✗ ./pipex
Parent process
Child process
```

### FUNCIONAMIENTO DE DUP2():

Esta función recibe por argumentos dos números (fd) de forma que el primero es el nuevo fd y el segundo el antiguo fd. Un programa facil para entender como funciona dup2 podria ser:

```c
int	main(void)
{
	int	fd_file;

	fd_file = open("testfile.txt", O_WRONLY);
	dup2(fd_file, 1);
	ft_printf("Esto no se escribe en la terminal\n");
	return (0);
}
```

Este programa, pese a que `ft_printf()` muestre las cosas por defecto en la salida 1 (terminal o STDOUT) no mostraría nada en la pantalla, si no que mostraría eso en una archivo `testfile.txt`.

## FUNCIONAMIENTO DEL PROGRAMA PIPEX

A la hora de realizar el proyecto, tenemos que tener muy en cuenta tanto las funciones previas como algunas otras, la teoría del programa es:

- **Creamos** un **pipe y** hacemos **fork()**.
- El programa hijo **cambiará sus fd de entrada y salida** por el primer archivo y la zona de escritura de la pipe, respectivamente.
- Despues de esto y configurar los cierres de la pipe, tendrá que **ejecutar cmd1** sobre infile y escribir el resultado de ello en la pipe, para que despues sea leido por el proceso padre.
- El proceso padre **cambiará la salida y entrad**a por el outfile y la zona de lectura de la pipe, respectivamente.
- A continuación, **aplicará cmd2** sobre lo que lee en la pipe y escribirá el resultado en el outfile.

Sin embargo, ejecutar el comando que recibimos no es tan facil. Para ello debemos utlizar el argumento **ENVP.**

El array envp es un argumento que podemos indicar que sea recibido por defecto en el programa, el cual incluye las variables de entorno del ordenador.

<aside>
💡 Las variables de entorno son un conjunto de información sobre la configuración del sistema. Podemos ver esto con el comando **env** en la terminal. Entre la información que incluye podemos encontrar todos los paths donde el sistema almacena las funciones de la consola.

</aside>

La parte del path es la que queremos usar en nuestro programa para saber dónde esta el cmd que nos han pedido ejecutar, **los comandos pueden estar en varias ubicaciones diferentes**, por lo que tendremos que ir probando (con la funcion `access`) hasta dar con la localización del comando que queremos usar. Una vez encontrado, lo ejecutaremos mediante el uso de la función `execve`. Esta función recibirá: El **path del comando** a ejecutar, un array con todo lo que se ejecutará **(el comando + las flags)** y el array **envp**.
