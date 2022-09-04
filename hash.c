#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIBERADO 0
#define OCUPADO 1
#define DIMENSAO 7

typedef struct cliente {

	int codcliente;
	char name[100];

} TC;

typedef struct hash {
    
    TC usuario;
    int next;
    int bandeira;
    
} TH;

const int SIZE_HASH = (sizeof(int) * 3) + (sizeof(char) * 100);

const int BANDEIRA = (sizeof(int) * 2) + (sizeof(char) * 100);

int calculo(int n)
{
    return n % DIMENSAO;
}
void cria()
{
    FILE *fp;
    if((fp = fopen("tabhash.dat", "wb")) == NULL)
    {
        printf("error\n");
        exit(1);
    }
    
    TH table;
    table.usuario.codcliente = -1;
    char name[100] = "";
    table.next = 0;
    table.bandeira = 0;
    
    for(int i = 0; i < DIMENSAO; i++)
    {
        fwrite(&table.usuario.codcliente, sizeof(int), 1, fp);
        fwrite(&i, sizeof(int), 1, fp);
        fwrite(&table.bandeira, sizeof(int), 1, fp);
        fwrite(name, sizeof(char), sizeof(table.usuario.name), fp);
    }
    
    fclose(fp);
}

void busca(int codcliente, int *fim, int *z)
{
    
    FILE *fp;
    if((fp = fopen("tabhash.dat", "rb")) == NULL)
    {
        printf("error\n");
        exit(1);
    }
    
    int c = 0;
    *z = 0;
    *fim = calculo(codcliente);
    TH table;
    
    while(*z == 0)
    {
        fseek(fp, SIZE_HASH * (*fim), SEEK_SET);
        
        
        fread(&table.usuario.codcliente, sizeof(int), 1, fp);
        fread(&table.next, sizeof(int), 1, fp);
        fread(&table.bandeira, sizeof(int), 1, fp);
        fread(table.usuario.name, sizeof(char), sizeof(table.usuario.name), fp);
        
        if(table.bandeira == 0)
        {
            c = *fim;
        }
        
        if(table.usuario.codcliente = codcliente && table.bandeira == 1)
        {
            *z = 1;
        }
        
        else
        {
            if(*fim == table.next)
            {
                *z = 2;
                *fim = c;
            }
            else
            {
                *fim = table.next;
            }
            
        }
        
    }
    fclose(fp);
}

void insercao(TC o)
{
    
    int fim = 0;
    int z = 0;
    int i = 1;
    int c = -1;
    
    busca(o.codcliente, &fim, &z);
    
    if(z != 1)
    {
        FILE *fp;
        if((fp = fopen("tabhash.dat", "rb+")) == NULL)
        {
            printf("error\n");
            exit(1);
        }
    
    
    if(fim != 1)
    {
        c = fim;
    }
    else
    {
        c = calculo(o.codcliente);
        while(i <= DIMENSAO)
        {
            fseek(fp, (SIZE_HASH * c) + BANDEIRA, SEEK_SET);
                
            int bandeira;
            fread(&bandeira, sizeof(int), 1, fp);
            
            if(bandeira == 1)
            {
                c = (c + 1) % DIMENSAO;
                i++;
            }
            else
            {
                i = DIMENSAO + 2;
            }
        }
        if(i == DIMENSAO + 1)
        {
            printf("overflow\n");
            return;
        }
        int aux;
        int movimento = SIZE_HASH * calculo(o.codcliente) + sizeof(int) + (sizeof(char) * 100);
        
        fseek(fp, movimento, SEEK_SET);
        fread(&aux, sizeof(int), 1, fp);
        
        fseek(fp, movimento, SEEK_SET);
        fwrite(&c, sizeof(int), 1, fp);
        
        if(aux != calculo(o.codcliente))
        {
            movimento =  SIZE_HASH * c + sizeof(int) + (sizeof(char) * 100);
            
            fseek(fp, movimento, SEEK_SET);
            fwrite(&aux, sizeof(int), 1, fp);
        }
    }
    int bandeira = 1;
    fseek(fp, SIZE_HASH * c, SEEK_SET);
    fwrite(&o.codcliente, sizeof(int), 1, fp);
    fwrite(&o.name, sizeof(char), sizeof(o.name), fp);
    fseek(fp, sizeof(int), SEEK_SET);
    fwrite(&bandeira, sizeof(int), 1, fp);
    fclose(fp);
    
}
    else
    {
        printf("chave existente\n");
    }
}

void main()
{
    cria();
    
    TC u1;
    u1.codcliente = 6;
    strncpy(u1.name, "john",100);
    insercao(u1);
    
    TC u2;
    u2.codcliente = 3;
    strncpy(u2.name, "ken",100);
    insercao(u2);
    
    TC u3;
    u3.codcliente = 15;
    strncpy(u3.name, "ryu",100);
    insercao(u3);
    
    TC u4;
    u4.codcliente = 12;
    strncpy(u4.name, "sagat",100);
    insercao(u4);
    
    TC u5;
    u5.codcliente = 14;
    strncpy(u5.name, "balrog",100);
    insercao(u5);
}



