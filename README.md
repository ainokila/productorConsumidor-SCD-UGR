# Problema Productor-Consumidor

Alumno : Cristian Velez Ruiz 

Asignatura : Sistemas Concurrentes y Distribuidos - UNIVERSIDAD DE GRANADA 2015

Curso : *

Grupo : *

_____________________________________________________________________________________________________________

Productor-Consumidor : 

La estructura que utilizo para almacenar los datos producidos es una cola , en la que al tener un tamaño fijo he decidido crearla de forma estatica , con
un valor de 10.

Esta ejecucion consta de dos thread , uno llamado produce que actua de productor y otro consume que actuara de consumidor:

	pthread_create(&produce,NULL,&productor,NULL);
	pthread_create(&consume,NULL,&consumidor,NULL);

Los semaforos que he utilizado son : 
	
	sem_t produc ;
	sem_t consum ; 
	sem_t escribePantalla;
        sem_t queueMutex ;

Y los he inicializado de la siguiente manera : 

	sem_init( &produc, 0, 10 ) y sem_init( &consum, 0, 0 ): El semaforo de produc esta a 10 ya que podra producir como mucho hasta 10 valores , 
	este semaforo actua conjuntamente con el de consum , ya que cuando este decrece hasta 0 como mucho , el de consum aumentara hasta 10 diciendo que podria consumir 
	como mucho hasta 10 valores , la suma de los valores de estos semaforos en toda la ejecucion valdra 10.

	sem_init( &escribePantalla, 0, 1 ): Este semaforo  es para poder escribir en pantalla , cuando se vaya a utilizar un cout o cerr , este semaforo se pondra  a 0 hasta que 
	no termine , y cuando termine se pondra a 1 , para que ningun otra salida de pantalla la interrumpa.

	sem_init( &queueMutex, 0, 1 ): Muy parecido al escribe pantalla , pero en este caso es para evitar conflictos de meter y sacar datos de la Cola.

	
_____________________________________________________________________________________________________________

Fumadores : 

En este programa no uso ninguna estructura de datos auxiliar para almacenar los datos , ya que cuando se produce por ejemplo papel , el mismo estanquero desbloquea al proceso que 
necesitase papel.

Los 4 thread que he utilizado en esta implementacion son :
 
	pthread_t fumadores[3]: En este array almaceno a 3 fumadores , el 0 necesita papel , el 1 tabaco y el 2 mechero , estos los desbloqueara el estanquero dependiendo de 
				lo que ha producido.
	pthread_t estanco: Sera el primero que se lance y hasta que un fumador no recoja lo que ha generado no volvera a crear otro material .

Los semaforos usados han sido : 

	sem_t semaforoEstanco: En cuanto genere un 1 elemento se para y hasta que no lo recojan , no volvera a producir , por tanto su inicializacion sera a 1.

	sem_t productoPreparado[3]; De la misma manera dependiendo de lo que se haya producido el estanquero mandara una señal para que se "recoja" el producto y pueda fumar.


Y los he inicializado de la siguiente manera : 

	sem_init(&semaforoEstanco,0,1): Nada mas lanzarse sera lo primero que se ejecute para crear tabaco , papel o cerillas , ya que sin eso los demas fumadores no podran ejecutarse y 
					permanceran dormidos , a la espera de su producto.


	for(int i= 0 ; i<3 ; i++ ) {

		sem_init(&productoPreparado[i],0,0): Hasta que el estanquero no llame a alguno de ellos a fumar estaran dormidos , cuando terminen de fumar , volvera a dormirse.
			

	}
	
	
Contacto : www.cristianvelez.es
