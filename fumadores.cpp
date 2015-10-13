#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"
using namespace std;

	//__________________________VARIABLES______________________________________

	sem_t semaforoEstanco;
	sem_t productoPreparado[3];

	//________________________________________________________________________

	// ----------------------------------------------------------------------------
	// función que simula la acción de fumar  como un retardo aleatorio de la hebra

	void fumar() 
	{ 
	   //  inicializa la semilla aleatoria  (solo la primera vez)
	   static bool primera_vez = true ;
	   if ( primera_vez )
	   {   primera_vez = false ;
	      srand( time(NULL) );    
	   }
	   
	   // calcular un numero aleatorio de milisegundos (entre 1/10 y 2 segundos)
	   const unsigned miliseg = 100U + (rand() % 1900U) ; 

	   // retraso bloqueado durante 'miliseg' milisegundos
	   usleep( 1000U*miliseg ); 
	}
	// ----------------------------------------------------------------------------

	// falta: resto de funciones 
	// ..............

	void * estanquero(void*){

		while(true){

			sem_wait(&semaforoEstanco);
		
			//PRODUZCO LO QUE QUIERO
			int auxiliar;
			auxiliar=rand()%3;
		
			//Lo guardo en producto generado
			productoGenerado=auxiliar;

			switch (auxiliar){
				case 0: 
					cout << "Estanquero ha generado papel\n";
					
				    	break;
				case 1: 
					cout << "Estanquero ha generado tabaco\n";
					
				   	 break;
				case 2: 
					cout << "Estanquero ha generado cerillas\n";
					
					 break;
				
			}
			sem_signal(&productoPreparado[posicion]);
		
		
		}

	return NULL;
	}

	void* fumador(void* posicion){

		while(true){

			int valor = (int) posicion;
			//Espero producto generado

			sem_wait(&productoPreparado[posicion]);
			
			cout << "Fumador " << posicion << "  va a fumar \n";
			
			sem_signal(&semaforoEstanco);

			fumar();

			cout << "Fumador " << posicion << "  termina de fumar \n" ;
			
		}

	return NULL;
	}

	

	// ----------------------------------------------------------------------------

	int main()
	{
	  // falta: creación hebras ....
		
		//Inicializo los semaforos :

		sem_init(&semaforoEstanco,0,1);

		for(int i= 0 ; i<3 ; i++ ) {

			sem_init(&productoPreparado[i],0,0);
			

		}
		
		//Lanzo los hilos , el de los fumadores con un for.

		pthread fumadores[3];
		pthread estanco;
	
		pthread_create(&estanco,NULL,estanquero,NULL);

		for(int i = 0 ; i <3 ; i++ ) {
			void* auxiliar = (void * ) i ;
			pthread_create(&fumadores[i],NULL,fumador,auxiliar);
		}

		



		//Join 
		for(int i = 0 ; i <3 ; i++ ) {
			void* auxiliar = (void * ) i ;
			pthread_join(fumadores[i],NULL);
		}
		pthread_join(estanco,NULL);


	return 0 ;
	}
