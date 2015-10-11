#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>

using namespace std ;

	   
     	
	class Cola {

		private :

			static const int TAM = 10 ;
			int cola[TAM];
			int numElementos ;
		public:
			Cola(){

				numElementos = 0 ;

			}

			void aniade(int elemento){
				cola[numElementos]=elemento;
				numElementos++;
			}

			bool lleno(){

				bool solucion=false ; 
				if(numElementos==TAM){
					solucion = true;
				}
				return solucion;

			}
		
			int getElemento(){

				int solucion = -1;

				if(numElementos!=0){

					solucion = cola[0];
					for(int i = 1 ; i < numElementos ; i++ ){
						cola[i-1]=cola[i];
					}
					numElementos--;
				}else{
					cerr << "No puedes obtener elemento " ;
				}
				return solucion ;
			}

			int getNum(){
				return numElementos;
			}


	};

	//******************************************************************************************
	//DECLARACION VARIABLES GLOBALES:

	Cola cola;
	sem_t semaforo ; 
	sem_t escribe;                  
	const unsigned num_items  = 40 ;

	//******************************************************************************************


	unsigned producir_dato()
	{
	  static int contador = 0 ;
	  cout << "producido: " << contador << endl << flush ;
	  return contador++ ;
	}
	// ---------------------------------------------------------------------

	void consumir_dato( int dato )
	{
		cout << "dato recibido: " << dato << endl ;
	}
	// ---------------------------------------------------------------------

	void* productor( void* ){   
		
	 		 for( unsigned i = 0 ; i < num_items ; i++ ){ 

				if(!cola.lleno()){

					sem_wait(&escribe);                    //Espera permiso para poder escribir
					int dato = (int) producir_dato() ;
					sem_post(&escribe);			// Devuelve el permiso de escribir

					cola.aniade(dato);
					sem_post(&semaforo);//Incremento 1 el valor del semaforo , ya que hemos leido 1 dato.
			  	}else{
					i--;
				}
				
	   
	  		}

	  return NULL ;
	}
	// ---------------------------------------------------------------------

	void* consumidor( void* ){   

	  for( unsigned i = 0 ; i < num_items ; i++ ){   

		int dato ;

		sem_wait(&semaforo);
		
		if(cola.getNum()!=0){

			dato=cola.getElemento();

			sem_wait(&escribe);				//Espera permiso de escritura
			consumir_dato( dato ) ;				//Escribe	
			sem_post(&escribe);				//Devuelve permiso de escritura

		}else{
			i--;
		}
		
		
	  }
	  return NULL ;
	}
	//----------------------------------------------------------------------

	int main(){

		sem_init( &semaforo, 0, 0 );
		sem_init( &escribe, 0, 1 );	//Lo inicializo a 1 para que se pueda escribir , el primer proceso que lo coja

		pthread_t produce;
		pthread_t consume ; 

		//Lanzo los dos hilos , consumidor y productor 
		pthread_create(&produce,NULL,&productor,NULL);
		pthread_create(&consume,NULL,&consumidor,NULL);

		pthread_join( produce, NULL );
		pthread_join( consume, NULL );
			
		sem_destroy(&semaforo);
		sem_destroy(&escribe);

		cout << "Fin de la tarea";

	   return 0 ; 
	}
