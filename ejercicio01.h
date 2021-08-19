#ifndef EJERCICIO01_H
#define EJERCICIO01_H

#include <stdbool.h>

typedef struct _nodo nodo;
typedef char INFO;
typedef float VAL;

struct _nodo{
    INFO info;
    nodo *sig;
    //se agrega un nodo a anterior para facilitar la lectura e impresión de la pila
    nodo *ant;
    
    int p; //prioridad del caracter
    VAL val; //valor numérico del caracter
    bool oper;	//oper == false (en un operando)	oper == true (es un operador)
};


typedef struct _pila PILA;

struct _pila{
    nodo *top;
    nodo *bottom;
    int num;
};

//Funciones normales de una pila
nodo* crear_nodo(INFO info, int p);
bool operador(INFO info);
PILA *crear_pila();
bool es_vacia(PILA *l);
bool vaciar(PILA *l);
void imprimir_pila(PILA *l);

//Funciones agregadas para convertir infija a postfija
int prioridad(INFO info);
void clasificar(char info, PILA *entrada, PILA *postfija);
bool push(PILA *l, INFO info);
bool push_clasificar(PILA *auxiliar, PILA *postfija, INFO info);
void pop(PILA *auxiliar, PILA *postfija, INFO info);
bool trasvasar(PILA *auxiliar, PILA *postfija);

//Lectura de valores para la evaluación de la expresión
void leer_valores(PILA *postfija);
void leer_archivo(PILA *postfija);

//Funciones agregadas para evaluar la expresión
void evaluar(PILA *postfija, PILA *resultado);
bool push_evaluar(PILA *l, INFO info, VAL valor);
void evaluar_clasificar(PILA *postfija, PILA *resultado, INFO info);
void pop_evaluar(PILA *l);

#endif
