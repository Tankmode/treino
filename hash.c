#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct cliente {

	int codcliente;
	char name[100];
	int proximo;
	int status;

} TC;

typedef struct hash {
    
    TC usuario;
    
} TH;

const int TAMANHO = (sizeof(int) * 3) + (sizeof(char) * 100);
const int STATUS = (sizeof(int) * 2) + (sizeof(char) * 100);

int calculo(int n)
{
    return n % 7;
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
    table.usuario.proximo = 0;
    table.usuario.status = 0;
    
    for(int i = 0; i < 7; i++)
    {
        fwrite(&table.usuario.codcliente, sizeof(int), 1, fp);
        fwrite(name, sizeof(char), sizeof(table.usuario.name), fp);
        fwrite(&i, sizeof(int), 1, fp);
        fwrite(&table.usuario.status, sizeof(int), 1, fp);
    }
    
    fclose(fp);
}

void insercao(TC o)
{
    
    int destino = 0;
    int achou = 0;
    int i = 1;
    int c = -1;
    
    busca(o.codcliente, &achou, &destino);
    
    if(achou != 1)
    {
        FILE *fp;
        if((fp = fopen("tabhash.dat", "rb+")) == NULL)
        {
            printf("error\n");
            exit(1);
        }
        
       
      
    if(destino != 1)//se for verdadeiro, quer dizer que não está ocupado
    {
        c = destino;
    }
    else
    {
        c = calculo(o.codcliente);
        while(i <= 7)// enquanto não for maior que o tamanho 7
        {
            fseek(fp, (TAMANHO * c) + STATUS, SEEK_SET);
                
            int status;
            fread(&status, sizeof(int), 1, fp);
            
            if(status == 1)//ESTÁ OCUPADO TEM QUE TER DESLOCAMENTO
            {
                c = (c + 1) % 7;
                i++;
            }
            else
            {
                i = 7 + 2;
            }
        }
        if(i == 7 + 1)// SE FOR VERDADEIRO OVERFLOW TEM ALGO A CONSERTAR
        {
            printf("overflow\n");
            return;
        }
       
        int movimento = TAMANHO * calculo(o.codcliente) + sizeof(int) + (sizeof(char) * 100);
        int aux;
        
        fseek(fp, movimento, SEEK_SET);
        fread(&aux, sizeof(int), 1, fp);
        
        fseek(fp, movimento, SEEK_SET);
        fwrite(&c, sizeof(int), 1, fp);
        
        if(aux != calculo(o.codcliente))// se a chave não existir outra igual
        {
            movimento =  TAMANHO * c + sizeof(int) + (sizeof(char) * 100);
            
            fseek(fp, movimento, SEEK_SET);
            fwrite(&aux, sizeof(int), 1, fp);
        }
    }
    
    fseek(fp, TAMANHO * c, SEEK_SET);
    fwrite(&o.codcliente, sizeof(int), 1, fp);
    fwrite(&o.name, sizeof(char), sizeof(o.name), fp);
    fseek(fp, sizeof(int), SEEK_SET);
    int status = 1;
    fwrite(&status, sizeof(int), 1, fp);
    fclose(fp);
    
}
    else
    {
        printf("chave existente\n");
    }
}

void busca(int codcliente, int *achou, int *destino)
{
    
    FILE *fp;
    if((fp = fopen("tabhash.dat", "rb")) == NULL)
    {
        printf("error\n");
        exit(1);
    }
    
    int c = 0;
    *achou = 0;
    *destino = calculo(codcliente);
    TH table;
    
    while(*achou == 0)
    {
        fseek(fp, TAMANHO * (*destino), SEEK_SET);
        
        
        fread(&table.usuario.codcliente, sizeof(int), 1, fp);
        fread(table.usuario.name, sizeof(char), sizeof(table.usuario.name), fp);
        fread(&table.usuario.proximo, sizeof(int), 1, fp);
        fread(&table.usuario.status, sizeof(int), 1, fp);
        
        if(table.usuario.status == 0)//se for verdadeiro está liberada
        {
            c = *destino;
        }
        
        if(table.usuario.codcliente = codcliente && table.usuario.status == 1)//se for verdadeira
        {
            *achou = 1;
        }
        
        else
        {
            if(*destino == table.usuario.proximo)
            {
                *achou = 2;
                *destino = c;
            }
            else
            {
                *achou = table.usuario.proximo;
            }
            
        }
        
    }
    fclose(fp);
}

void main()
{
    cria();
    
    TC u1;
    u1.codcliente = 49;
    strncpy(u1.name, "JUST",100);
    insercao(u1);
    
    TC u2;
    u2.codcliente = 51;
    strncpy(u2.name, "FAKER",100);
    insercao(u2);
    
    TC u3;
    u3.codcliente = 59;
    strncpy(u3.name, "JOKER",100);
    insercao(u3);
    
    TC u4;
    u4.codcliente = 10;
    strncpy(u4.name, "JACK",100);
    insercao(u4);
    
    TC u5;
    u5.codcliente = 103;
    strncpy(u5.name, "JOE",100);
    insercao(u5);
}





