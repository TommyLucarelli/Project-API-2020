#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM 100

int op_curr = 0;
int op_totale = 0;
int righe_scritte = 0;

typedef struct elemento {
    char *riga;
    struct elemento *p_stor;
} elem;

typedef struct storico {
    char comando;
    int ind1, ind2, righe_scritte;
    elem *p_elem;
    elem **snapshot;
} stor;

int pot(int base, int esp);

void allunga_storico(stor** storia, char com, int ind1, int ind2);

void change(int ind1, int ind2, elem **tes, stor** storia);

void delete(int ind1, int ind2, stor **storia, int molt, elem** tes);

void print(int ind1, int ind2, elem **tes, stor **storia);

void McFly(int n, elem **tes, stor **storia);


int main() {
    elem **testo; //testa dell'array della struttura TESTO
    stor **storia;
    char *in, comando, buffer[1025];
    int ind1, ind2, n, i, j, elev, com, molt = 1, x, len, undo_totali, prev, flag = 0, flr = 0, dim=100;
    int c_op = 0, actual = 0, dis = 0, c_num_op_sempre=0;
    testo = (elem **) calloc(DIM, sizeof(elem *));
    storia = (stor **) calloc(dim, sizeof(stor*));
    //settaggio del primo elemento dell'array storico
    storia[0] = (stor*) malloc(sizeof(stor));
    storia[0]->comando = 'b';
    storia[0]->righe_scritte = 0;
    storia[0]->snapshot = NULL;
    fgets(buffer, 1025, stdin);
    len = strlen(buffer) + 1;
    in = malloc(len * sizeof(char));
    strncpy(in, buffer, len * sizeof(char));
    while (in[0] != 'q') //assunzione che i dati in input siano sempre corretti
    {
        i = 0;
        ind1 = 0;
        ind2 = 0;
        n = 0;
        j = 0;
        while (in[i] != ',' && in[i] != 'u' && in[i] != 'r') {
            i++;
        }
        if (in[i] == ',') {
            i--;
            do {
                elev = pot(10, i);
                ind1 = ind1 + (in[j] - 48) * elev;
                i--;
                j++;
            } while (in[j] != ',');
            i = 0;
            while (in[i] != 'c' && in[i] != 'd' && in[i] != 'p')
                i++;
            j++;
            com = i; // mi ricordo in che posizione è il comando
            i = i - j - 1;
            do {
                elev = pot(10, i);
                ind2 = ind2 + (in[j] - 48) * elev;
                i--;
                j++;
            } while (in[j] != 'c' && in[j] != 'd' && in[j] != 'p');
            comando = in[com];
            undo_totali = dis - actual;
            if (undo_totali != 0) {
                McFly(undo_totali, testo, storia); //ov_undos dovrebbe essere pos per undo e neg per redo, controllare
            }
            switch (comando) {
                case 'c':
                    c_num_op_sempre++;
                    c_op = actual + 1;
                    actual = c_op;
                    dis = c_op;
                    flr = 0;
                    x = DIM * molt;
                    prev = x;
                    if (ind2 > x) //faccio crescere il vettore di DIM ogni volta
                    {
                        while (ind2 > x) {
                            molt++;
                            x = DIM * molt;
                        }
                        testo = (elem **) realloc(testo, x * sizeof(elem *));
                        for (i = prev; i < x; i++)//potrebbe non servire
                            testo[i] = NULL;
                    }
                    if(c_num_op_sempre >= dim) //aumento dimensioni array storico
                    {
                        prev = dim;
                        dim = dim*2;
                        storia = (stor **) realloc(storia, dim * sizeof(stor *));
                        for (i = prev; i < dim; i++)
                            storia[i] = NULL;
                    }
                    allunga_storico(storia, comando, ind1, ind2);
                    change(ind1, ind2, testo, storia);
                    break;
                case 'd':
                    c_num_op_sempre++;
                    c_op = actual + 1;
                    actual = c_op;
                    dis = c_op;
                    flr = 0;
                    if(c_num_op_sempre >= dim)
                    {
                        prev = dim;
                        dim = dim*2;
                        storia = (stor **) realloc(storia, dim * sizeof(stor *));
                        for (i = prev; i < dim; i++)
                            storia[i] = NULL;
                    }
                    allunga_storico(storia, comando, ind1, ind2);
                    delete(ind1, ind2, storia, molt, testo);
                    break;
                case 'p':
                    dis = actual;
                    print(ind1, ind2, testo, storia);
                    break;
            }
        } else {
            flag = 1;
            com = i;
            i--;
            do {
                elev = pot(10, i);
                n = n + (in[j] - 48) * elev;
                i--;//va a -1
                j++;
            } while (in[j] != 'u' && in[j] != 'r');
            if (in[com] == 'u') {
                flr = 1;
                actual = actual - n;
                if (actual < 0)
                    actual = 0;
                //curr = undo(n, testo, curr);

            } else if (in[com] == 'r') {
                if (flr == 1) {
                    actual = actual + n;
                    if (actual > c_op)
                        actual = c_op;
                    //curr = redo(n, testo, curr);
                }
            }

        }
        free(in);
        fgets(buffer, 1025, stdin);
        len = strlen(buffer) + 1;
        in = malloc(len * sizeof(char));
        strcpy(in, buffer);
    }
    free(in);
    return 0;
}

int pot(int base, int esp) {
    int res = 1;
    while (esp) {
        if (esp % 2)
            res = res * base;
        esp = esp / 2;
        base = base * base;
    }
    return res;
}

void allunga_storico(stor **storia, char com, int ind1, int ind2)
{
    op_curr++;
    op_totale = op_curr;
    if(storia[op_totale] != NULL)
        free(storia[op_totale]);
    storia[op_totale] = (stor*) malloc(sizeof(stor));
    storia[op_totale]->comando = com;
    storia[op_totale]->ind1 = ind1;
    storia[op_totale]->ind2 = ind2;
    storia[op_totale]->snapshot = NULL;
    storia[op_totale]->p_elem = NULL;
}

void change(int ind1,int ind2, elem **tes, stor **storia) //non so gesire 0,0c
{
    elem *p, *precedente = NULL;
    char buffer[1025], *x;
    int i, len;
    i = ind1 - 1;
    if(ind2 > righe_scritte)
        righe_scritte = ind2;
    fgets(buffer, 1025, stdin);
    len = strlen(buffer) + 1;
    x = malloc(len * sizeof(char)); //free
    strncpy(x, buffer, len * sizeof(char));
    do {
        p = (elem *) malloc(sizeof(elem));
        p->riga = malloc(len * sizeof(char));
        strncpy(p->riga, x, len * sizeof(char));
        p->p_stor = NULL;
        tes[i] = p;
        if (storia[op_totale]->p_elem == NULL) {
            storia[op_totale]->p_elem  = p;
        } else {
            precedente->p_stor = p; //in pratica creo la lista di righe che mi permetterà di fare il redo
        }
        precedente = p;
        free(x);
        fgets(buffer, 1025, stdin);
        len = strlen(buffer) + 1;
        x = malloc(len * sizeof(char));
        strncpy(x, buffer, len * sizeof(char));
        i++;
    } while (x[0] != '.');
    free(x);
    storia[op_totale]->righe_scritte = righe_scritte;
}

void delete(int ind1, int ind2, stor **storia, int molt, elem **tes) {
    int canc, i;
    elem** p;
    if(ind2 > righe_scritte)
        ind2 = righe_scritte;
    if(ind1 > righe_scritte)
        ind1 = righe_scritte + 1; //così canc fa zero e non cambia nulla al numero di righe scritte dell'operazione
    canc = ind2-ind1+1;
    if(ind2 != righe_scritte)//vuok dire che sto cancellando in mezzo
    {
        memmove(tes + ind1 - 1, tes + ind2, (righe_scritte - ind2) * sizeof(elem *));
        for (i = righe_scritte - canc ; i < righe_scritte; i++)//si potrebbe togliere
            tes[i] = NULL;
    }//se invece cancella gli ultimi non faccio nulla, tanto l'array è sporco
    //snapshot dopo aver cancellato
    righe_scritte = righe_scritte - canc;
    if(righe_scritte < 0)
        righe_scritte = 0;
    storia[op_totale]->righe_scritte = righe_scritte;
    p = (elem **) malloc(righe_scritte * sizeof(elem *));
    memmove(p, tes, righe_scritte * sizeof(elem *)); //sistemare
    storia[op_totale]->snapshot = p;
}

void print(int ind1, int ind2, elem **tes, stor **storia)
{
    int n, x, i;
    n = storia[op_curr]->righe_scritte;
    if (ind1 == 0)
        i = ind1;
    else
        i = ind1 - 1;
    while (i < ind2 && i < n)
    {
        fputs(tes[i]->riga, stdout); //THIS IS THE PROBLEM, sta provando a scrivere a una riga che è null
        i++;
    }
    x = ind2 - ind1 - i + 1;
    for(i=0; i<x; i++)
        printf(".\n");
}

void McFly(int n, elem **tes, stor **storia)
{
    int sistemate = 0, i, x, j=0, ind1, ind2;
    elem* p;
   op_curr = op_curr - n;
   if(op_curr < 0) //in teoria il problema dovrebbe essere già risolto nel main
       op_curr = 0;
   else if(op_curr > op_totale)
       op_curr = op_totale;
   x = storia[op_curr]->righe_scritte;
    //perdita di tempo, ma necessaria per far funzionare il programma
   for(i=0; i < x; i++)
   {
       if(tes[i] != NULL)
            tes[i] = NULL;
   }
   i = op_curr;
   while(sistemate < x) //da controllare
   {
       if(storia[i]->comando == 'd') //sovrascivo tutto, tranne quelli già sistemati
       {
           while(j < x && j < storia[i]->righe_scritte)
           {
               if(tes[j] == NULL){
                   if(storia[i]->snapshot[j] != NULL) //teoricamente inutile
                   {
                       tes[j] = storia[i]->snapshot[j];
                       sistemate++;
                   }
               }
               j++;
           }
       }else{
           ind1 = storia[i]->ind1 - 1;
           ind2 = storia[i]->ind2;
           p = storia[i]->p_elem;
           for(j = ind1; j < ind2; j++)
           {
               if(tes[j] == NULL) {
                       tes[j] = p;
                       sistemate++;
                   p = p->p_stor;
               }else{
                   p = p->p_stor;
               }
           }
       }
       i--;
   }
   righe_scritte = storia[op_curr]->righe_scritte;
}
