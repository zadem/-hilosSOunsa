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

CRITICAL_SECTION SeccionCritica;

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
	//iniciar seccion critica
	InitializeCriticalSectionAndSpinCount(&SeccionCritica, 0x00000400);
	
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
	DWORD   dwHilosCaja[3];
	for(int i = 0 ; i < 3 ; i++){
		int* argumento = (int*)(i + 1);
		hilosCaja[i] = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            AtendiendoThread,       // thread function name
            argumento,          // argument to thread function 
            0,                      // use default creation flags 
            &dwHilosCaja[i]);   // returns the thread identifier 

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
	
	printf("Cliente %d empezo su compra\n", cliente->id);
	while(cliente->tCompra){
		cliente->tCompra--;
		Sleep(100);
	}
	printf("Cliente %d termino su compra\n", cliente->id);
	cliente->estado = ESPERANDO_ATENCION;
	
	return 0;
}

DWORD WINAPI AtendiendoThread(LPVOID lpParam ){
	int* nCaja = (int*)lpParam;
	printf("Caja %d creada\n", nCaja);
	int indice;
	
	while((indice = buscarSiguienteCliente()) != YA_NO_HAY_CLIENTES){
		if(indice == CLIENTES_NO_LISTOS){
			continue;
		}
		
		Cliente* cliente = clientes[indice];
		printf("Caja %d empezo a atender al cliente: %d\n", nCaja, cliente->id);
		while(cliente->tAtencion){
			cliente->tAtencion--;
			Sleep(100);
		}
		cliente->estado = ATENDIDO;
		printf("Caja %d termino a atender al cliente: %d\n", nCaja, cliente->id);	
	}
	printf("Caja %d terminada, ya no hay clientes\n", nCaja);
	
	return 0;
}

int buscarSiguienteCliente(){
	EnterCriticalSection(&SeccionCritica);//entrar a seccion critica
	
	int contador = 0;
	for(int i = 0 ; i < N_CLIENTES ; i++){
		
		if(clientes[i]->estado == ESPERANDO_ATENCION){
			clientes[i]->estado = ATENDIENDO;
			LeaveCriticalSection(&SeccionCritica); //dejando seccion critica
			return i;
		}else if(clientes[i]->estado == ATENDIENDO || clientes[i]->estado == ATENDIDO){
			contador++;
		}
	}
	LeaveCriticalSection(&SeccionCritica); //dejando seccion critica
	
	if(contador == N_CLIENTES){
		return YA_NO_HAY_CLIENTES;
	}
	
	return CLIENTES_NO_LISTOS;
}
