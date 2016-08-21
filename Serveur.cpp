#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "Ecran.h"
#include "Commun.dat"

int	idQ,idS,idM;
char	*pShm;

MESSAGE	Message;

int	main()
{
if ((idQ = msgget(CLE,IPC_CREAT | IPC_EXCL |0666)) == -1)
   { perror("Err de msgget()");
     exit(1);
   }

Trace("idQ = %d : idM = %d",idQ,idM);
int idRechercheChemin;
char BuffQ[20];
sprintf(BuffQ,"%d",idQ);

while (1)
   { if (msgrcv(idQ,&Message,sizeof(MESSAGE) - sizeof(long),1L,0) == -1)
        { perror("Err. de msgrcv()");
          exit(1);
        }
     Trace("Message recu\n");
     switch (Message.Requete)
        { case RECHERCHE : printf("Message RECHERCHE\n");
              
              
              if ((idRechercheChemin = fork()) == -1)
                 { perror("Err de fork()");
                   exit(1);
                 }
              if (!idRechercheChemin)
                 { execl("./RechercheChemin","RechercheChemin",BuffQ,NULL);
                   perror("Err de execlp()");
                   exit(1);
                 }
              Message.lType = idRechercheChemin;
              Trace("----%d",idRechercheChemin);
              if (msgsnd(idQ,&Message,sizeof(MESSAGE) - sizeof(long),0) == -1)
                  { perror("(serveur)Err de msgsnd()");
                    exit(1);
                  }
             
	
             break;
        }
      
   }
}
