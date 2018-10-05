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
	printf("incio el thread\n");
	
	//    HANDLE hStdout;
    Cliente *cliente;
/*
    TCHAR msgBuf[BUF_SIZE];
    size_t cchStringSize;
    DWORD dwChars;


    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if( hStdout == INVALID_HANDLE_VALUE )
        return 1;
 */
    cliente = (Cliente*)lpParam;

    //StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters = %d, %d\n"), pDataArray->val1, pDataArray->val2); 
    //StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    //WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);

	printf("termino el thread\n");
	
	return 0;
}
