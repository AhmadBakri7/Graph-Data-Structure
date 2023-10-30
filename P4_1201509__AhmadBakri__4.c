#include <stdio.h>
#include <stdlib.h>
#define infinitey 10000
// define a struct
struct NSTR
{
    char STR [20];
    struct NSTR* start;
};

/* Ahmad Bakri 1201509***/
int CityCounter = 0;
int ElementInQueue = 0;
int FullCost =0;
char  TotalPath [100];

///////////////////////////////////////////////////////
// define the graph
struct Graph
{
    int W;
    char TheCity [20];
    struct Graph* TheNext;
    struct Graph* TheParent;
    int cost;
};
/////////////////////////////////////////////////////////

// functions prototype
void ThePrint(struct Graph* F);
void Dykstra(struct Graph* AdjList[], int index);
int Chicking(struct Graph* P[CityCounter], struct Graph* y);
int TheCost(struct Graph* A[],struct Graph* l);
void LestAdd(char S [20], struct NSTR* R);
void Insertion(struct Graph* o,struct Graph* A[CityCounter]);
int Delete(struct Graph*s[CityCounter]);


////////////////////////////////////////////////////////
int main()
{
    // define a struct to make a list
    struct NSTR* TheLof = (struct NSTR*)malloc(sizeof(struct NSTR));
    TheLof->start = NULL;

    FILE *Cities;
    //openning file
    Cities = fopen("cities.txt", "r");
    //////////
    char TheStore[100];//define an array
    //////////
    char * STR;
    while(fgets(TheStore,100,Cities))//read the file for the first one
    {
        STR = strtok(TheStore,"\t");//cut the name of the city from the file
        char W[20];
        strcpy(W,STR);
        LestAdd(W,TheLof);
    }
    rewind(Cities);
///////////////////////////
    struct Graph* TheList [CityCounter];  // list
    for(int i = 0 ; i < CityCounter; i ++)
    TheList[i] = (struct Graph*)malloc(sizeof(struct Graph));
    struct NSTR* MP = TheLof->start;

    // initialization
    for(int i = 0 ; i < CityCounter ; i++)
    {
        strcpy(TheList[i]->TheCity,MP->STR);
        // Every node in the List we copy Just one name from  list
        TheList[i]->W = 0;
        TheList[i]->cost =0;
        TheList[i]->TheNext = NULL;

        if(MP->start == NULL)
            break;
        MP = MP->start;
    }

    char SecondLine[100];//define an array
    // read the file one more
    while(fgets(SecondLine,100,Cities))
    {
        char * T = strtok(SecondLine,"\t");
        char P1[20];
        char P2[20];

        strcpy(P1,T);
        T = strtok(NULL,"\t");
        strcpy(P2,T);

        T = strtok(NULL,"km");
        int x = atoi(T);

        struct Graph* Graph = (struct Graph*)malloc(sizeof(struct Graph));
        Graph->W = x;
        // Establish node and add values
        strcpy(Graph->TheCity,P2);
        for(int i = 0 ; i < CityCounter ; i++)
        {
            if(strcmp(TheList[i]->TheCity,P1)==0)
            {
                // insert
                Graph->TheNext = TheList[i]->TheNext;
                TheList[i]->TheNext = Graph;
            }
        }

    }
    int U=0;
    while(U != 10)
    {
        printf("{1}loads the file and construct the graph\n");
        printf("{2}read the source city and compute the Dijkstra algorithm (single source shortest path)\n");
        printf("{3}print the full route of the shortest path including the distance between each two cities and the total shortest cost\n");
        printf("{4}prints the information of step 3 to a file called shortest_path.txt and exits the program\n");
        printf("choose one \n");
        scanf("%d",&U);
        if(U==1)
        {
            for(int i = 0 ; i< CityCounter ; i++)
            {
                // print List
                printf("[%s] is connected to the following cities : ",TheList[i]->TheCity);
                //call the function
                ThePrint(TheList[i]);
            }
        }
        else if(U==2)
        {
            printf("Enter City :");// the city from:
            char c [20] ;
            scanf("%s",c);

            int index;
            for(int i = 0 ; i < CityCounter ; i++)
            {
                if(strcmp(TheList[i]->TheCity,c) == 0)
                    index = i;
            }
            if(index < CityCounter)
                Dykstra(TheList,index);
            else printf("the city does not exist\n");
        }
        else if (U==3)
        {
            strcpy(TotalPath,"");
            printf("DEST:");

            char D[20];
            scanf("%s",D);

            printf("\nThe road is : \n");
            for(int i = 0 ; i<CityCounter ; i++)
            {
                if(strcmp(TheList[i]->TheCity,D) == 0)
                {
                    struct Graph* G = TheList[i];
                    while(G)
                    {
                        if(G->TheParent == NULL)
                        {
                            strcat(TotalPath,"[");
                            strcat(TotalPath,G->TheCity);
                            strcat(TotalPath,"] ");
                            printf(" [%s] ",G->TheCity);
                            break;
                        }
                        printf("[%s] <--",G->TheCity);
                        strcat(TotalPath,"[");
                        strcat(TotalPath,G->TheCity);
                        strcat(TotalPath,"] ");
                        strcat(TotalPath,"<--");
                        G = G->TheParent;
                    }
                    printf("\nTotal Distance  is : %d\n ", TheList[i]->cost);
                    FullCost = TheList[i]->cost;
                }
            }
        }
        else if (U==4)
        {
            FILE *Result;// writtingto the file
            Result = fopen("shortest_path.txt", "w");
            fprintf(Result,"Shortest Path : \n %s \n total cost is %d", TotalPath,FullCost);
            fclose(Result);
            return 0;
        }
        printf("\n");
    }


    return 0;
}


void Dykstra(struct Graph* A[], int index)
{

    for(int i =0; i < CityCounter ; i++)
    {
        A[i]->cost = infinitey;
        A[i]->TheParent = NULL;
    }
    A[index]->cost =0;
//////////////////////////////////////////////
    struct Graph* PQ [CityCounter];
    for(int j = 0 ; j<CityCounter; j++)
        PQ[j]= NULL;
    for(int i = 0 ; i < CityCounter ; i++)
    {
        Insertion(A[i],PQ);
        ElementInQueue++;
    }
//////////////////////////////////////////////
    while(ElementInQueue != 0)
    {
        int Min = Delete(PQ);
        PQ[Min] = NULL;
        ElementInQueue--;
////////////////////////////////////////////
        struct Graph* u = A[Min];
        struct Graph* j = u->TheNext;
        while(j != NULL)
        {
            if((Chicking(PQ,j)) )
            {
                int num = u->cost + j->W;
                int TheIndex = TheCost(A,j);
                if(num < A[TheIndex]->cost)
                {
                    A[TheIndex]->cost = num;
                    A[TheIndex]->TheParent = u;
                }
            }
            ///////////////////////////////////
            j = j->TheNext;
        }
    }
}

// This function return the index of the node with the min cost in the array
int Delete(struct Graph*s[CityCounter])
{

    int n =1000;
    int w = 0;
    for(int i = 0 ; i < CityCounter ; i++)
    {
        if(s[i]!= NULL && s[i]->cost < n)
        {
            n = s[i]->cost;
            w = i;
        }
    }
    return w;
}
//This function print list
void ThePrint(struct Graph* F)
{
    struct Graph* T = F->TheNext;
    while(T)
    {
        printf("[%s %d]  ", T->TheCity,T->W);
        T = T->TheNext;
    }
    printf("\n");
}
// This function inserts a node in the array at the first empty
void Insertion(struct Graph* o,struct Graph* A[CityCounter])
{

    for(int i = 0 ; i < CityCounter ; i++)
    {
        if(A[i] == NULL)
        {
            A[i] = o;
            break;
        }
        else
            continue;
    }
}
 // This function adds a string to list
void LestAdd(char S [20], struct NSTR* R)
{

    struct NSTR* q = (struct NSTR*)malloc(sizeof(struct NSTR));
    strcpy(q->STR,S);
    struct NSTR* p = R;
    while(p != NULL )
    {
        if(strcmp(p->STR,S) == 0)
            break;
        else if(p->start == NULL && strcmp(p->STR,S) != 0)
        {
            p->start = q;
            q->start = NULL;
            CityCounter++;
        }
        else
            p = p->start;
    }
}
 // This function returns the index of a node in the list
int TheCost(struct Graph* A[],struct Graph* l)
{
    for(int i = 0 ; i < CityCounter ; i++)
    {
        if(strcmp(A[i]->TheCity,l->TheCity)==0)
        {
            return i;
        }
    }
}
// This function checks if a given node is in the array
int Chicking(struct Graph* P[CityCounter], struct Graph* y)
{
    int F = 0;
    for(int i = 0 ; i < CityCounter ; i++)
    {
        if(P[i] == NULL)
        {
            continue;
        }
        if(strcmp(P[i]->TheCity,y->TheCity) == 0)
        {
            F = 1;
            break;
        }
    }
    return F;
}

