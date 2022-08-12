#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int nNodi, lCassifica, nGrafi = 0, lCoda;

typedef struct
{
    int nodo;
    int peso;
} Nodo;

typedef struct
{
    int grafo;
    int sommaPesi;
} ElClassifica;

void ReadInt(int *n);
void MinHeapify(Nodo A[nNodi], int n);
void DecrementaPri(Nodo A[nNodi], int i);
Nodo CancellaMin(Nodo A[nNodi]);
void AggiungiClassifica(ElClassifica classifica[lCassifica], int grafo, int sommaPesi);
void AggiornaMaxClassifica(ElClassifica classifica[lCassifica], int n);
void AggiungiGrafo(int grafo[nNodi][nNodi - 1], ElClassifica classifica[lCassifica]);
int Dijkstra(int grafo[nNodi][nNodi - 1]);
int main()
{
    ReadInt(&nNodi);
    ReadInt(&lCassifica);
    ElClassifica classifica[lCassifica];
    int grafo[nNodi][nNodi - 1], i, min;
    char c;
    while ((c = getchar_unlocked()) != EOF)
    {
        if (c == 'A')
            AggiungiGrafo(grafo, classifica);
        else if (c == 'T')
        {
            min = (nGrafi < lCassifica) ? nGrafi : lCassifica;
            for (i = 0; i < min - 1; i++)
                printf("%d ", classifica[i].grafo);
            if (min > 0)
                printf("%d", classifica[i].grafo);
            printf("\n");
        }
    }
    return 0;
}
void ReadInt(int *n)
{
    char x = getchar_unlocked();
    *n = 0;
    while (!isdigit(x))
        x = getchar_unlocked();
    while (isdigit(x))
    {
        *n = (*n * 10) + (x - '0');
        x = getchar_unlocked();
    }
}
int Dijkstra(int grafo[nNodi][nNodi - 1])
{
    int sum = 0, i, ndist;
    Nodo coda[nNodi - 1];
    lCoda = nNodi - 1;
    for (i = 0; i < nNodi - 1; i++)
    {
        coda[i].nodo = i + 1;
        coda[i].peso = -1;
    }
    Nodo u = {0, 0};
    while (lCoda > 0 && u.peso >= 0)
    {
        for (i = 0; i < lCoda; i++)
        {
            ndist = u.peso + grafo[u.nodo][coda[i].nodo - 1];
            if (ndist != u.peso && (coda[i].peso < 0 || coda[i].peso > ndist)) // se ndist è uguale a u.peso allora il peso dell'arco è 0
            {
                coda[i].peso = ndist;
                DecrementaPri(coda, i);
            }
        }
        if (lCoda > 0)
        {
            u = CancellaMin(coda);
            if (u.peso > 0)
                sum += u.peso;
        }
    }
    return sum;
}
void AggiungiGrafo(int grafo[nNodi][nNodi - 1], ElClassifica classifica[lCassifica])
{
    int i, j;
    for (i = 0; i < nNodi; i++)
    {
        ReadInt(&grafo[i][0]); // ignoro la prima colonna del grafo
        for (j = 0; j < nNodi - 1; j++)
            ReadInt(&grafo[i][j]);
    }
    AggiungiClassifica(classifica, nGrafi++, Dijkstra(grafo));
}
void MinHeapify(Nodo A[nNodi], int n)
{
    int l, r, posmin = n;
    Nodo temp;
    do
    {
        n = posmin;
        l = 2 * n + 1;
        r = l + 1;
        if ((A[l].peso < A[n].peso || A[n].peso < 0) && A[l].peso > 0 && l < lCoda)
            posmin = l;
        else
            posmin = n;
        if ((A[r].peso < A[posmin].peso || A[n].peso < 0) && A[r].peso > 0 && r < lCoda)
            posmin = r;
        if (posmin != n)
        {
            temp = A[n];
            A[n] = A[posmin];
            A[posmin] = temp;
        }
    } while (posmin != n);
}
void DecrementaPri(Nodo A[nNodi], int i)
{
    Nodo temp;
    int p = (i - 1) / 2;
    while (i > 0 && (A[p].peso > A[i].peso || A[p].peso < 0))
    {
        temp = A[i];
        A[i] = A[p];
        A[p] = temp;
        i = p;
        p = (i - 1) / 2;
    }
}
Nodo CancellaMin(Nodo A[nNodi])
{
    Nodo min = A[0];
    lCoda--;
    if (lCoda > 0)
    {
        A[0] = A[lCoda];
        MinHeapify(A, 0);
    }
    return min;
}

void AggiungiClassifica(ElClassifica classifica[lCassifica], int grafo, int sommaPesi)
{
    if (grafo < lCassifica)
    {
        classifica[grafo].grafo = grafo;
        classifica[grafo].sommaPesi = sommaPesi;
        ElClassifica temp;
        int p = (grafo - 1) / 2;
        while (grafo > 0 && (classifica[p].sommaPesi < classifica[grafo].sommaPesi || (classifica[p].sommaPesi == classifica[grafo].sommaPesi && classifica[p].grafo < classifica[grafo].grafo)))
        {
            temp = classifica[grafo];
            classifica[grafo] = classifica[p];
            classifica[p] = temp;
            grafo = p;
            p = (grafo - 1) / 2;
        }
    }
    else if (classifica[0].sommaPesi > sommaPesi)
    {
        classifica[0].grafo = grafo;
        classifica[0].sommaPesi = sommaPesi;
        AggiornaMaxClassifica(classifica, 0);
    }
}
void AggiornaMaxClassifica(ElClassifica classifica[lCassifica], int n)
{
    int l, r, posmax = n;
    ElClassifica temp;
    do
    {
        n = posmax;
        l = 2 * n + 1;
        r = l + 1;
        if (l < lCassifica && (classifica[l].sommaPesi > classifica[n].sommaPesi || (classifica[l].sommaPesi == classifica[n].sommaPesi && classifica[l].grafo > classifica[n].grafo)))
            posmax = l;
        else
            posmax = n;
        if (r < lCassifica && (classifica[r].sommaPesi > classifica[posmax].sommaPesi || (classifica[r].sommaPesi == classifica[posmax].sommaPesi && classifica[r].grafo > classifica[posmax].grafo)))
            posmax = r;
        if (posmax != n)
        {
            temp = classifica[n];
            classifica[n] = classifica[posmax];
            classifica[posmax] = temp;
        }
    } while (posmax != n);
}