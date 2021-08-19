#include "ejercicio01.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//FUNCIONES PARA TODAS LAS PILAS

//Función para crear un nodo
nodo* crear_nodo(INFO info, int p){
    nodo* t = (nodo*)malloc(sizeof(nodo));
    t->sig = NULL;
    t->ant = NULL;
    t->info = info;
    t->p = p;	//prioridad
    t->val = 0;	//valor numérico
    t->oper = operador(info);	//determina si info es un operador (true) o un operando (false)
    return t;
}

//Función que determina si info es un operador (true) o un operando (false)
bool operador(INFO info){
	//Si info NO ES algún operador
	if(info != '+'  &&  info != '-'  &&  info != '*'  &&  info != '/'  &&  info != '?'  &&  info != '%'  &&  info != '^'  &&  info != '('  &&  info != ')')
		return false;
	//Si info ES un operador
	else
		return true;
}

//Función que crea una pil
PILA *crear_pila(){
    PILA* l = (PILA*) malloc(sizeof(PILA));
    l->top = l->bottom = NULL;
    l->num = 0;
    return l;
}

//Función que revisa si una pila está vacía
bool es_vacia(PILA *l){
    if(l->top == NULL && l->bottom ==NULL) return true;
    return false;
}

//Función que vacía una pila
bool vaciar(PILA *l){
    if(es_vacia(l)) return false;
    
    nodo* temporal = l->top;
    while(temporal != NULL){
        l->top = temporal->sig;
        free(temporal);
        temporal = l->top;
    }
    l->top = l->bottom = NULL;
    l->num = 0;
    return true;
}

//Función que imprime una pila
void imprimir_pila(PILA *l){
	for(nodo* t = l->bottom; t != NULL; t = t->ant)
	    printf("%c", t->info);
	printf("\n");
}



/////////////////////////////////////////////////////////////////////////////////////////////
//FUNCIONES PARA CONVERTIR DE INFIJA A POSTFIJA

//Función que asigna una prioridad numérica a los operadores
int prioridad(INFO info){
	int p;
    if (info == '^') p = 3;
    else if (info == '?') p = 3;
    else if (info == '*') p = 2;
    else if (info == '/') p = 2;
    else if (info == '%') p = 2;
    else if (info == '+') p = 1;
    else if (info == '-') p = 1;
    else if (info == '(') p = 0;
    else if (info == ')') p = -1; 
    else p = 4;
    return p;
}

//Función que reorganiza un caracter dependiendo si en o no un operador
void clasificar(char info, PILA *auxiliar, PILA *postfija){
	if(!operador(info))	//Si en un OPERANDO
		push(postfija, info);	//Agregar a la pila postfija
	else	//Si en un OPERADOR
		push_clasificar(auxiliar, postfija, info);	//Mandar a push_clasificar()
}

//Función para agregar un elemento a una pila
bool push(PILA *l, INFO info){
	if(info != ')'){	//NUNCA se agrega un ')' a una pila
		nodo* nuevo = crear_nodo(info, prioridad(info));
	    if(l->top == NULL && l->bottom == NULL){	//Si la pila está vacía
	        l->top = l->bottom = nuevo;
	        l->num++;
	        return true;
	    }else{	//Si la pila NO está vacía
	        nuevo->sig = l->top;
	        l->top->ant = nuevo;
	        l->top = nuevo;
	        l->num++;
	        return true;
	    }
	}
    return false;
}

//Función que maneja a los OPERADORES
bool push_clasificar(PILA *auxiliar, PILA *postfija, INFO info){	//Recibe una pila que ocupa como auxilia y otra donde va guardando postfija
    nodo* nuevo = crear_nodo(info, prioridad(info));  
    if(!es_vacia(auxiliar)){ //si la pila auxiliar NO está vacía revisar:
    	if(nuevo->p != 0  && nuevo->p != -1){ //si el operador es diferente a '(' y diferente a ')'
    		while(!es_vacia(auxiliar)  &&  nuevo->p <= auxiliar->top->p) //mientras la prioridad del operador nuevo <= prioridad de la cima de auxiliar
    			pop(auxiliar, postfija, info); //sacar la cima
	    } else{ //si es '(' o ')'
	    	if(nuevo->p == -1){ //si es ')'	    	
	    		while(auxiliar->top->p != 0)//mientras la cima de auxiliar NO sea '('
	    			pop(auxiliar, postfija, info); //sacar la cima
	    		pop(auxiliar, postfija, info); //sacar '('
	    	}
	    }
    }
    push(auxiliar, info); //meter operador nuevo a la pila auxiliar
    return false;
}

//Función que hace pop a la pila auxiliar, pero antes copia la info a la pila de postfija
void pop(PILA *auxiliar, PILA *postfija, INFO info){
    if(es_vacia(auxiliar)) return;
    
    if(auxiliar->top->info != '(')	//Si la info de la cima de auxiliar NO es '('
		push(postfija, auxiliar->top->info); //agrega la info de la cima de auxiliar a la pila de salida
	
	//borrar la cima de auxiliar
	if(auxiliar->num > 1){	//Si hay más de un elemento hacer:    
	    auxiliar->top = auxiliar->top->sig;
		auxiliar->top->ant->sig = NULL;
		free(auxiliar->top->ant);
		auxiliar->top->ant = NULL;
		auxiliar->num--;
	} else	//Si en la pila SOLO hay un elemento:
		vaciar(auxiliar);	
}

//Función que copia los nodos de la pila auxiliar a la pila postfija
bool trasvasar(PILA *auxiliar, PILA *postfija){
	if(es_vacia(auxiliar)) return false;
	//Si la pila NO está vacía
    nodo* temporal = auxiliar->top;
    while(temporal != NULL){
    	push(postfija, temporal->info); //agrega la info de temporal a la pila postfija
	    temporal = temporal->sig;
    }
    vaciar(auxiliar);	//Al final vacía la pila auxiliar
    return true;
}



/////////////////////////////////////////////////////////////////////////////////////////////
//FUNCIONES PARA EVALUAR LA EXPRESIÓN POSTFIJA

//Función que pide al usuario los valores de los operandos manualmente
void leer_valores(PILA *postfija){
	nodo *temporal = postfija->bottom;	//temporal ayuda a recorrer cada elemento de postfija
	
	//Se crea una pila auxiliar para que no se pida más de una vez la misma letra
	PILA *auxiliar;
	auxiliar = crear_pila();
	
	do{	//Ciclo que recorre cada elemento de postfija
		if(temporal->oper == false){	//Si temporal es un OPERANDO:
			bool i = false;
			if(!es_vacia(auxiliar)){	//Si la pila auxiliar NO ESTÁ vacía
				nodo *temporalAUX = auxiliar->top;
				for(int e = 0; e < auxiliar->num; e++){	//Ciclo para recorrer cada elemento de auxiliar
					if(temporalAUX->info == temporal->info){	//Si la variable de auxiliar == a la variable de postfija
						temporal->val = temporalAUX->val;	//Asignar el valor de auxiliar a postfija
						i = true;	//true es que SÍ se repite el operando
					}
					
					//Para seguir recorriendo a auxiliar
					else if(temporalAUX->sig != NULL)
							temporalAUX = temporalAUX->sig;
				}
			}
			if(i == false){	//Si el operando NO se repite
				printf("\n\t%c:\t", temporal->info);
				scanf("%f", &temporal->val);	//Pedir el valor al usuario
				push_evaluar(auxiliar, temporal->info, temporal->val);	//Agregar ese nuevo operando a auxiliar
			}
			
		}
		temporal = temporal->ant;		
	}while(temporal != postfija->top);
}

//Función que lee los valores de los operandos desde un archivo
void leer_archivo(PILA *postfija){
	nodo *temporal = postfija->bottom;
	
	//Se abre el archivo
	FILE *f;
    f = fopen("valores.txt", "r");
    INFO letra;
    float valor;
	
	do{	//Ciclo para recorrer postfija
		if(temporal->oper == false){	//Si en un operando
			bool r = false;	//false = no encontrado
	    	while(!feof(f)  &&  r == false){	//Mientras no se llegue al final del archivo y no se encuentre la variable buscada
	    		fscanf(f, "%c %f", &letra, &valor);	//Leer el archivo y obtener la letra y valor de esa línea
	    		if(letra == temporal->info){	//Si la letra del archivo es igual a la que se busca
	    			temporal->val = valor;	//Asignar el valor del archivo a la variable en postfija
	    			r = true;	//true = encontrado
	    		}
	    	}
	    	rewind(f);	//Coloca el puntero del archivo al inicio de éste
		}
		temporal = temporal->ant;		
	}while(temporal != postfija->top);
	
	fclose(f);	//Cierra el archivo
}

//Función que discrimina entre operadores y operandos 
void evaluar(PILA *postfija, PILA *resultado){
	nodo* temporal = postfija->bottom;
	while(temporal != NULL){	//Con este ciclo se recorre la pila postfija nodo a nodo
		if(temporal->oper == false)	//Si es un OPERANDO
			push_evaluar(resultado, temporal->info, temporal->val);	//Agregar a la pila resultado
		else	//Si es un OPERADOR
			evaluar_clasificar(postfija, resultado, temporal->info);	//Mandar a evaluar_clasificar()
		
        temporal = temporal->ant;
    }
	
}

//Función que agrega un elemento a una pila
bool push_evaluar(PILA *l, INFO info, VAL valor){	//Recibe una pila, la información a agregar y el valor numérico
		nodo* nuevo = crear_nodo(info, prioridad(info));
	    if(l->top == NULL && l->bottom == NULL){	//Si la pila está vacía
	        l->top = l->bottom = nuevo;
	        nuevo->val = valor;	//Valor numérico 
	        l->num++;
	        return true;
	    }else{	//Si NO está vacía
	        nuevo->sig = l->top;
	        l->top->ant = nuevo;
	        l->top = nuevo;
	        nuevo->val = valor;	//Valor numérico
	        l->num++;
	        return true;
	    }
    return false;
}

//Función que opera los valores de la pila resultado de acuerdo a un operador
void evaluar_clasificar(PILA *postfija, PILA *resultado, INFO info){	//recibe la pila postfija, la de salida y el operador
	//Se crean variables auxiliares para la cima(b) de la pila postfija, el elemento siguiente(a) y el resultado c
	VAL a = resultado->top->sig->val;	//segundo elemento de la pila postfija
	VAL b = resultado->top->val;	//cima de la pila postfija
	VAL c = 0;	//resultado
	
	//Se operan a y b con el operador(info) y se guarda en c
	if (info == '^')
		c = pow(a, b);
    else if (info == '?')
    	c = sqrt(b);
    else if (info == '*')
    	c = a * b;
    else if (info == '/')
    	c = a / b;
    else if (info == '%'){
    	c = a / b;
    	int cAUX = (int)c;
    	c = a-(b*cAUX);
    }
    else if (info == '+')
    	c = a + b;
    else if (info == '-')
    	c = a - b;
        
    if(info != '?'){	//Si el operador NO es raíz
    	resultado->top->sig->val = c;	//Se reescribe el valor de a (segundo elemento de postfija) por el de c (el resultado)
	    pop_evaluar(resultado);	//Se elimina la cima de postfija
    } else	//Si ES raíz
    	resultado->top->val = c;	//Se reescribe el valor de b (cima de postfija) por el de c (el resultado)
}

//Función que hace pop a una pila
void pop_evaluar(PILA *l){
    if(es_vacia(l)) return;
	if(l->num > 1){	    
	    l->top = l->top->sig;
		l->top->ant->sig = NULL;
		free(l->top->ant);
		l->top->ant = NULL;
		l->num--; 
	} else
		vaciar(l);	
}

