#include "ejercicio01.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	int i, num, resultado;
	char entrada[100];
    PILA *auxiliar, *postfija;
    auxiliar = crear_pila();
    postfija = crear_pila();
	
	//Se hace la lectura de la cadena infija desde un archivo
	FILE *I;
    I = fopen("cadena_infija.txt", "r");
	fgets(entrada, 100, I);
	fclose(I);
	
	num = strlen(entrada);	//cantidad de caracteres de la cadena infija
	
	//Se imprime la cadena infija
	printf("\n\n\tLa cadena de entrada es:\t");
	for(i = 0; i < num; i++)
		printf("%c", entrada[i]);
	
	//Se lee uno a uno los caracteres de infija y se mandan a clasificar()
	for(i = 0; i < num; i++)
		//Se clasifican los caracters de acuerdo a si son operandores o operandos (y de acuerdo a su prioridad)
		clasificar(entrada[i], auxiliar, postfija);
	
	//Se vacían los elementos de auxiliar hacia portfija
	trasvasar(auxiliar, postfija);
	
	//Se imprime la expresión postfija
	printf("\n\n\tLa cadena de salida queda como:\t");
    imprimir_pila(postfija);
    
    printf("\n\n\t¿Desea guardar la expresion postfija en un archivo? (S / N)\t");
    char respuesta;
    scanf("%s", &respuesta);
    
    //Se guarda la expresión postfija en un archivo
    if(respuesta == 'S'){
    	nodo *temporal = postfija->bottom;
    	char c_postfija[postfija->num];
    	for(int i = 0; i < postfija->num; i++){
    		c_postfija[i] = temporal->info;
    		temporal = temporal->ant;
    	}
    	FILE *archivo = fopen("Postfija.txt", "w+");
        if (archivo==NULL)
            printf("\n\tNo fue posible abrir el archivo");
        fputs("\n\tNOTACION POSTFIJA\n\tSu expresion es:\n\t", archivo);
        fputs(c_postfija, archivo);
        fclose(archivo);
        printf("\n\tCADENA GUARDADA\n");
    }
    
    //Se realiza la lectura de valores numéricos, ya sea manualmente o desde un archivo
    printf("\n\n\t¿Desea ingresar los valores manualmente o de un archivo? (M / A)\t");
    scanf("%s", &respuesta);
    if(respuesta == 'A')
    	leer_archivo(postfija);
    else
    	leer_valores(postfija);
	
	//Se evalua postfija y se imprima
	evaluar(postfija, auxiliar);
	printf("\n\n\tEl resultado es:\t%f", auxiliar->bottom->val);
	
	//Se vacían y liberan las pilas
    vaciar(auxiliar);
    vaciar(postfija);
    free(auxiliar);
    free(postfija);
    
    printf("\n");
}
