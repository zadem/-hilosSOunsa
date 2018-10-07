#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include<time.h>

#define COMPRANDO 1
#define PAGANDO 2
#define N_CLIENTES 5

DWORD WINAPI ComprandoThread( LPVOID lpParam );

typedef struct Cliente{
	int id;
	int estado;
	int tCompra;
	int tAtencion;
};

int _tmain(){
	//semilla
	srand(time(0)); 
	
    Cliente *clientes[N_CLIENTES];
    DWORD   dwHilos[N_CLIENTES];
    HANDLE  hilos[N_CLIENTES]; 
//CREACION DE THREADS
    for( int i=0; i< N_CLIENTES; i++ ){
    
        clientes[i] = (Cliente*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Cliente));

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

DWORD WINAPI ComprandoThread( LPVOID lpParam ){
	Cliente *cliente = (Cliente*)lpParam;
	
	
	printf("%d incio el thread\n", cliente->id);
    printf("%d Id: %d\n", cliente->id, cliente->id);
    printf("%d Estado: %d\n", cliente->id, cliente->estado);
    printf("%d Tiempo Compra: %d\n", cliente->id, cliente->tCompra);
    printf("%d Tiempo de atencion: %d\n", cliente->id, cliente->tAtencion);
	
	
	while(cliente->tCompra){
		cliente->tCompra--;
		printf("%d tiempo compra: %d\n", cliente->id, cliente->tCompra);
		Sleep(100);
	}
	printf("%d termino el thread\n", cliente->id);
	return 0;
}
