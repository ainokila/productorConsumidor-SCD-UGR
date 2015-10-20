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
	sem_t produc ;
	sem_t consum ; 
	sem_t escribePantalla; 
	sem_t queueMutex ;
	
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

				
					sem_wait(&produc);

					sem_wait(&escribePantalla);                    //Espera permiso para poder escribir
					int dato = (int) producir_dato() ;
					sem_post(&escribePantalla);			// Devuelve el permiso de escribir

					sem_wait(&escribePantalla);                    //Espera permiso para poder escribir
					int dato = (int) producir_dato() ;
					sem_post(&escribePantalla);			// Devuelve el permiso de escribir

					sem_post(&consum);
					
				
	   
	  		}

	  return NULL ;
	}
	// ---------------------------------------------------------------------

	void* consumidor( void* ){   

	  for( unsigned i = 0 ; i < num_items ; i++ ){   

		int dato ;		
			
			sem_wait(&consum);
			
			sem_wait(&queueMutex);				//Espero al mutex de la cola para poder guardar datos.
			dato=cola.getElemento();
			sem_post(&queueMutex);				//Devuelvo el mutex , ya he escrito en Cola

			sem_wait(&escribePantalla);				//Espera permiso de escritura
			consumir_dato( dato ) ;				//escribePantalla	
			sem_post(&escribePantalla);				//Devuelve permiso de escritura
			sem_post(&produc);
		
		
		
	  }
	  return NULL ;
	}
	//----------------------------------------------------------------------

	int main(){

		sem_init( &produc, 0, 10 );
		sem_init( &consum, 0, 0 );
		sem_init( &escribePantalla, 0, 1 );
		sem_init( &queueMutex, 0, 1 );	

		pthread_t produce;
		pthread_t consume ; 

		//Lanzo los dos hilos , consumidor y productor 
		pthread_create(&produce,NULL,&productor,NULL);
		pthread_create(&consume,NULL,&consumidor,NULL);

		pthread_join( produce, NULL );
		pthread_join( consume, NULL );
			
		
		sem_destroy(&escribePantalla);
		sem_destroy(&produc);
		sem_destroy(&consum);
		sem_destroy(&&queueMutex);

		cout << "Fin de la tarea";

	   return 0 ; 
	}
