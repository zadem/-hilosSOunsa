#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include<time.h>
#include <iostream>

using namespace std;

#define COMPRANDO 1
#define ESPERANDO_ATENCION 2
#define ATENDIENDO 3
#define ATENDIDO 4

#define CLIENTES_NO_LISTOS -1
#define YA_NO_HAY_CLIENTES -2
#define N_CLIENTES 5

DWORD WINAPI ComprandoThread( LPVOID lpParam );
DWORD WINAPI AtendiendoThread( LPVOID lpParam );
int buscarSiguienteCliente();



typedef struct Cliente{
	int id;
	int estado;
	int tCompra;
	int tAtencion;
};

Cliente **clientes;

int _tmain(){
	//semilla
	srand(time(0)); 
	
    //clientes[N_CLIENTES];
    clientes = new Cliente*[N_CLIENTES];
    DWORD   dwHilos[N_CLIENTES];
    HANDLE  hilos[N_CLIENTES]; 
//CREACION DE THREADS
    for( int i=0; i< N_CLIENTES; i++ ){
    
        clientes[i] = (Cliente*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Cliente));

        if( clientes[i] == NULL )
        {
            ExitProcess(2);
        }

		clientes[i]->id = i;
		clientes[i]->estado = COMPRANDO;
		clientes[i]->tAtencion = rand()/100;
		clientes[i]->tCompra = rand()/100;
        

        hilos[i] = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            ComprandoThread,       // thread function name
            clientes[i],          // argument to thread function 
            0,                      // use default creation flags 
            &dwHilos[i]);   // returns the thread identifier 

        if (hilos[i] == NULL) 
        {
           ExitProcess(3);
        }
    } 

//hilos cajas
	HANDLE  hilosCaja[3]; 

	for(int i = 0 ; i < 3 ; i++){
		hilosCaja[i] = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            AtendiendoThread,       // thread function name
            NULL,          // argument to thread function 
            0,                      // use default creation flags 
            NULL);   // returns the thread identifier 

        if (hilosCaja[i] == NULL) 
        {
           ExitProcess(3);
        }
	}
	WaitForMultipleObjects(3, hilosCaja, TRUE, INFINITE);






    WaitForMultipleObjects(N_CLIENTES, hilos, TRUE, INFINITE);
//LIBERAMIENTO DE THREADS
    for(int i=0; i< N_CLIENTES; i++)
    {
        CloseHandle(hilos[i]);
        if(clientes[i] != NULL)
        {
            HeapFree(GetProcessHeap(), 0, clientes[i]);
            clientes[i] = NULL;    // Ensure address is not reused.
        }
    }
	
	return 0;
}

DWORD WINAPI ComprandoThread(LPVOID lpParam ){
	Cliente *cliente = (Cliente*) lpParam;
	
	/*
	printf("%d incio el thread\n", 		cliente->id);
    printf("%d Id: %d\n", 				cliente->id, cliente->id);
    printf("%d Estado: %d\n", 			cliente->id, cliente->estado);
    printf("%d Tiempo Compra: %d\n",	cliente->id, cliente->tCompra);
    printf("%d Tiempo de atncion: %d\n",cliente->id, cliente->tAtencion);
	
	*/
	while(cliente->tCompra){
		cliente->tCompra--;
		//printf("%d TC: %d\n", cliente->id, cliente->tCompra);
		Sleep(100);
	}
	printf("%d termino el thread\n", cliente->id);
	
	cliente->estado = ESPERANDO_ATENCION;
	return 0;
}

DWORD WINAPI AtendiendoThread(LPVOID lpParam ){
	printf("caja creada\n");
	int indice;
	
	while((indice = buscarSiguienteCliente()) != YA_NO_HAY_CLIENTES){
		//printf("indice _: %d\n", indice);
		if(indice == CLIENTES_NO_LISTOS){
			continue;
		}
		
		Cliente* cliente = clientes[indice];
		
		
		printf("atendiendo al cliente : %d\n", indice);
		
		printf("%d incio el thread\n", 		cliente->id);
	    printf("%d Id: %d\n", 				cliente->id, cliente->id);
	    printf("%d Estado: %d\n", 			cliente->id, cliente->estado);
	    printf("%d Tiempo Compra: %d\n",	cliente->id, cliente->tCompra);
	    printf("%d Tiempo de atncion: %d\n",cliente->id, cliente->tAtencion);
		
		
		while(cliente->tAtencion){
			cliente->tAtencion--;
			//printf("%d TA: %d\n", cliente->id, cliente->tAtencion);
			Sleep(100);
		}
		printf("%d termino el thread\n", cliente->id);
		
		cliente->estado = ATENDIDO;
		
	}
	
	printf("caja terminada, ya no hay clientes\n");
	return 0;
}

int buscarSiguienteCliente(){
	int contador = 0;
	for(int i = 0 ; i < N_CLIENTES ; i++){
		
		if(clientes[i]->estado == ESPERANDO_ATENCION){
			clientes[i]->estado = ATENDIENDO;
			return i;
		}else if(clientes[i]->estado == ATENDIENDO || clientes[i]->estado == ATENDIDO){
			
			contador++;
		}
	}
	
	if(contador == N_CLIENTES){
		return YA_NO_HAY_CLIENTES;
	}
	
	
	return CLIENTES_NO_LISTOS;
}
