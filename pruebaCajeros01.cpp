#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include<time.h>

#define COMPRANDO 1
#define PAGANDO 2

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
	
	return 0;
}

DWORD WINAPI ComprandoThread( LPVOID lpParam ){
	printf("incio el thread\n");
	/*
	    HANDLE hStdout;
    PMYDATA pDataArray;

    TCHAR msgBuf[BUF_SIZE];
    size_t cchStringSize;
    DWORD dwChars;


    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if( hStdout == INVALID_HANDLE_VALUE )
        return 1;
 */
    pDataArray = (Cliente)lpParam;

    //StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters = %d, %d\n"), pDataArray->val1, pDataArray->val2); 
    //StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    //WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);

	printf("termino el thread\n");
	return 0;
}
