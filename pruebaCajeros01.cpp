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
	srand(time(0)); 
	int tiempoCompra;
	tiempoCompra = rand()/100;
	printf("%d\n", tiempoCompra);
	tiempoCompra = rand()/100;
	printf("%d\n", tiempoCompra);
	tiempoCompra = rand()/100;
	printf("%d\n", tiempoCompra);
	tiempoCompra = rand()/100;
	printf("%d\n", tiempoCompra);
	//printf("iniciado metodo principal\n");
	printf("hola\n");
	
	
	
    Cliente clientes[N_CLIENTES];
    DWORD   dwHilos[N_CLIENTES];
    HANDLE  hilos[N_CLIENTES]; 
//CREACION DE THREADS
    for( int i=0; i< N_CLIENTES; i++ )
    {
    
        pDataArray[i] = (PMYDATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));

        if( pDataArray[i] == NULL )
        {
            ExitProcess(2);
        }

        pDataArray[i]->val1 = i;
        pDataArray[i]->val2 = i+100;

        hThreadArray[i] = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            MyThreadFunction,       // thread function name
            pDataArray[i],          // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadIdArray[i]);   // returns the thread identifier 

        if (hThreadArray[i] == NULL) 
        {
           ExitProcess(3);
        }
    } 

    WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);
//LIBERAMIENTO DE THREADS
    for(int i=0; i<MAX_THREADS; i++)
    {
        CloseHandle(hThreadArray[i]);
        if(pDataArray[i] != NULL)
        {
            HeapFree(GetProcessHeap(), 0, pDataArray[i]);
            pDataArray[i] = NULL;    // Ensure address is not reused.
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
