#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/wait.h>
int main()
{int pid, status;
pid=fork();
if (pid==0) {
printf("figlio");
exit(-2);
}
else { pid=wait(&status);
if (WIFEXITED(status))
printf("Terminazione volontaria di %d constato %d\n",pid,WEXITSTATUS(status));
else if (WIFSIGNALED(status))
printf("terminazione involontaria per segnale%d\n", WTERMSIG(status)); }
printf("%d",status);
}
