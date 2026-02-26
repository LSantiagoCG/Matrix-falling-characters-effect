
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

using namespace std;
#pragma warning(disable:4996);

//Globales
//**********************************************************

#define FPS 30.0 //Constante FPS

//Elementos de allegro que se utilizarán para el juego
//**********************************************************
ALLEGRO_DISPLAY* pantalla;
ALLEGRO_FONT* fuente; //Crea pantalla y fuente

//LAS SIGUIENTES DOS FUNCIONES SE USAN PARA GENERAR NUMEROS RANDOM "VERDADEROS"
//EN PERIODOS DE TIEMPO MUY CORTOS
long g_seed = 1; //Crea una variable long int (más memoria que un int 
inline int fastrand() { //Rand mas rapido
	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}
char GenerarRandom() //genera un caracter aleatorio entre A y X
{
	char c = 'A' + fastrand() % 24;
	return c;
}




void inicializarstack(char stack[6]) { //Inicializa la cadena de char
	for (int i = 0; i > 6; i++) {
		stack[i] = ' ';
	}
}


/*
CADA LINEA DE CARACTERES QUE VA CAYENDO SE GUARDA EN UN STACK, QUE ES UN ARREGLO DE CHAR
COMO SE VAN GENERANDO CARACTERES NUEVOS ENTONCES LOS CARACTERES EN EL STACK SE VAN DESPLAZANDO
A MODO DE UNA PILA FIFO, PERO COMO ES UN ARREGLO ENTONCES SE IMPLEMENTA MAS FACIL DE LA SIGUIENTE MANERA
*/
void desplazar(char stack[6]) { //El ultimo caracter pasa al penultimo, el segundo al primero, etc
	stack[5] = stack[4];
	stack[4] = stack[3];
	stack[3] = stack[2];
	stack[2] = stack[1];
	stack[1] = stack[0];
}

/*
DIBUJAR LO QUE HACE ES EN BASE A UN STACK DEFINIDO Y UN CONJUNTO DE COORDENADAS IMPRIME EN EL DISPLAY
LA SECUENCIA DE CARACTERES ASCII, Y A SU VEZ LLAMA A LA FUNCION DESPLAZAR PARA SACAR DEL STACK EL CARACTER MAS VIEJO Y
AÑADIR AL INICIO EL MAS NUEVO
*/
char actual[1]; //Crea una variable para saber cual caracter se esta evaluando
void dibujar(char caracter[1], char stack[6], int x, int y) {
	actual[0] = stack[0];
	al_draw_text(fuente, al_map_rgb(0, 200, 0), x, y - 20, ALLEGRO_ALIGN_CENTRE, actual);//Fuente definida, verde fuerte, mismo x,
	actual[0] = stack[1];																 // y baja en 20 pixeles, centrado y se imprime el segundo caracter
	al_draw_text(fuente, al_map_rgb(0, 150, 0), x, y - 40, ALLEGRO_ALIGN_CENTRE, actual);//Fuente definida, verde menos fuerte, mismo x,
	actual[0] = stack[2];																 // y baja en 40 pixeles, centrado y se imprime el tercer caracter
	al_draw_text(fuente, al_map_rgb(0, 100, 0), x, y - 60, ALLEGRO_ALIGN_CENTRE, actual);//Asi continua
	actual[0] = stack[3];
	al_draw_text(fuente, al_map_rgb(0, 50, 0), x, y - 80, ALLEGRO_ALIGN_CENTRE, actual);
	actual[0] = stack[4];
	al_draw_text(fuente, al_map_rgb(0, 25, 0), x, y - 100, ALLEGRO_ALIGN_CENTRE, actual);
	actual[0] = stack[5];
	al_draw_text(fuente, al_map_rgb(0, 10, 0), x, y - 120, ALLEGRO_ALIGN_CENTRE, actual);

	al_draw_text(fuente, al_map_rgb(255, 255, 255), x, y, ALLEGRO_ALIGN_CENTRE, caracter);//Blanco, mismo x y y, centrado, imprime un nuevo caracter aleatorio como primer caracter

	al_flip_display();//Actualiza el display/pantalla

	desplazar(stack);//Desplaza todos los caracteres
	stack[0] = caracter[0];//Asigna el nuevo caracter como el primer caracter
}

int ranNum(int f, int i) {
	int ram = rand() % (f - i);
	return ram;
}

void gotoxy(int x, int y)
{
	COORD posicion;
	posicion.X = x;
	posicion.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posicion);
}

const int columnas=20;
//int xCoor = 0; //va de 0 hasta 720, sino no pinta, viejo intento, usese el int coorX[]


void coorRan(int coorXX[]) {
	for (int i = 0; i < columnas; i++)
	{
		coorXX[i] = rand() % 721;
	}
}
void charRan(char charR[]) { //extra, crea un array de caracteres random
	char Uno;
	for (int i = 0; i < columnas; i++)
	{
		Uno= GenerarRandom();
		charR[i] = Uno;
	}
}

//EXTRA>saca tama�o de arrays
char newPrinted[257 - 65];
//int length= (sizeof(newPrinted) / sizeof(*newPrinted));//length of array
int length(char charN[]) { //longitud o size de un array hcar

	int len= (sizeof(newPrinted) / sizeof(*newPrinted));
	return len;
}
int Ilength(int charN[]) { //longitud o size de un array int

	int len = (sizeof(newPrinted) / sizeof(*newPrinted));
	return len;
}

void NewChar(char charN) { //SI SE QUISIERA IMPRIMIR LA MATRIX CON CARACETRES ASCCI Y VER TODOS LOS IMPRESOS NUEVOS.
	bool repeated = false;
	for (int i = 0; i < length(newPrinted); i++)
	{
		if (charN==newPrinted[i])
		{	
			repeated = true;
			continue;
		}
	}
	if (repeated !=true)
	{
		for (int i = 0; i < length(newPrinted); i++)
		{
			if (newPrinted[i]==NULL)
			{
				newPrinted[i] = charN;
			}
		}
	}
}
//FIN DE EXTRA

//pistas, utilizadas
int pistaX[721]; //las pistas estan en x

//esta funcion evalua si ha habido impresiones en la pista, se comienza sin ninguna pista, y cada vez que el valor es nuevo o no esta repetido porque
//no se ubica en el array pistaX, este se agrega al array, en un momento dado el array se llenara con todas las posibles pistas, pistaX se lleno con -5's
//de esta forma ninguna coordenada x da ese valor y nos aseguramos de que por un valor aleatorio de crea que se utilizo una pista que no
void Pistas(int xPista) {  
	
	bool repeated = false; 
	for (int i = 0; i < Ilength(pistaX); i++)
	{
		
		if (pistaX[i] == xPista)
		{
			repeated = true;
			break;
		}
		break;
	}
	if (repeated == false)
	{
		for (int i = 0; i < Ilength(pistaX); i++)
		{
			if (pistaX[i] == -1)
			{
				pistaX[i] = xPista;
				break;
			}
		}
	}
}
//AGRUPACIONES CAIDAS EN CADA PISTAS
int nVeces[721]; //numero de agrupaciones por pista
int NpistaX[721];//inecesario//mas ordenado

//NpistaX se le aplica esta funcion, cada posicion del array representa una pista o columna o x=i en un plano cartesiano
void LlenePista(int Pista[]) { 
	for (int i = 0; i < Ilength(NpistaX); i++)
	{
		Pista[i] = i;
	}
}

//cada pista esta en 0 veces, para nVeces y pistaX
//la idea es rellenar el array para hacer comapraciones, nVeces tiene todos sus espacios con num=0 y pistaX=-5 para que asi no haya posibildiad e valor aleatorio y de 
//error al comparar
void Llene(int Pista[],int num) { 
	//cada pista esta en 0 veces, para nVeces
	for (int i = 0; i < Ilength(pistaX); i++)
	{
		Pista[i] = num;
	}
}

//funcion principal, si NpistaX es de la misma posicion que xColum, se le suma 1 al nVeces
void nPistas(char xColum) {
	
	//for (int i = 0; i < Ilength(pistaX); i++)
	//{

		//if (NpistaX[i] == xColum)//si alguna de las pistass registradas es iguala  la que esta ahora, se le suma uno al respectivo de nVeces
		//{
			nVeces[xColum]=nVeces[xColum]+1;
			//continue;
		//}

	//}
	
}

/// fin de agrupaciones




int nVecesLetra[27];
int cont1 = 0; //letras pintadas
char Abc[27];//dejamos un poco mas de espacio, no deberia ser necesario pero para evitar errores de 1 espacio de mas, Abc de abecedario
int totalAbc = 0;
void llenarAbc(char abc[]) {
	
	for (int ch = 'A'; ch <= 'Z'; ch++) {
		//do ch-'a' because we start at index 0
		abc[ch - 'A'] = ch;
	}
}

void nVecesAbc(char caracter) {

	
		for (int i = 0; i < length(Abc); i++)
		{

			if (Abc[i] == caracter)//si alguna de las pistass registradas es iguala  la que esta ahora, se le suma uno al respectivo de nVeces
			{
				nVecesLetra[i] = nVecesLetra[i] + 1;
				continue;
			}

		}

	
}
void totalABC() {
	for (int i = 0; i < 26; i++)
	{

		totalAbc = totalAbc + nVecesLetra[i];

	}
}



void GuardaraArray(int letrasV[], int pistas[], int nVeces[], int tiempo) {
	FILE* archivo; //se crea puntero del tipo file
	fopen_s(&archivo, "ARCHIVO.txt", "w+"); //se abre o crea un archivo txt, que se e asignara a la variable archivo, se usa modo de escribir o w+ el cual crea un archivo nuevo vacio independiente si ya habia uno antes
	int i = 0;
	if (NULL == archivo) { //si hubo un fallo y no se pudo crear el archivo
		printf("No se pudo abrir el archivo.");
	}
	else {
		//PtrTArticulo AUX = Lista; //aux se iguala al primer elemento, el que apunta lista
		fprintf(archivo, "TOTAL_PISTAS\n");
		fprintf(archivo, "%i\n", (cont1 / 30));
		fprintf(archivo, "TOTAL_LETRAS\n");
		fprintf(archivo, "%i\n", totalAbc);
		fprintf(archivo, "LETRAS\n");
		while (i < 26) { //mientras que aux sea un elemento y no sea algo vacio o null
			fprintf(archivo, "%i\n", letrasV[i]);
			i++;
		}
		i = 0;
		fprintf(archivo, "PISTAS UTILIZADAS\n");
		while (i < Ilength(pistas)) { //mientras que aux sea un elemento y no sea algo vacio o null
			fprintf(archivo, "%i\n", pistas[i]);
			i++;
		}
		i = 0;
		fprintf(archivo, "VECES POR PISTA\n");
		while (i < 722) { //mientras que aux sea un elemento y no sea algo vacio o null
			fprintf(archivo, "%i\n", nVeces[i]);
			i++;
		}

	}
	fclose(archivo); //se cierra el archivo y ya los datos quedaron guardados
}

void CargarArray() {
	//PtrTArticulo Nuevo; //se crea para cargarle los datos de este Nuevo al txt
	FILE* archivo; //se crea var para almacenar el txt
	fopen_s(&archivo, "ARCHIVO.txt", "r"); //abre archivo en modo lectura
	if (NULL == archivo) { //no abrio ningun archivo
		printf("No se pudo abrir el archivo");
	}
	else {

		int i = 0;
		char totalPistaNum[20];
		char total[20];

		char TitulototalLetras[20];
		char totalLetras[20];

		char title[20];
		char title2[20];
		char title3[20];
		char num[20];
		char num2[20];
		char num3[20];
		system("cls");
		while (!feof(archivo)) { //mientras no acabe de leerse el archivo

			fscanf(archivo, "%s\n", total, 20);
			fscanf(archivo, "%s\n", totalPistaNum, 20);
			cout << total<<  endl;
			cout << totalPistaNum << endl;

			fscanf(archivo, "%s\n", TitulototalLetras, 20);
			fscanf(archivo, "%s\n",  totalLetras, 20);
			cout << TitulototalLetras << endl;
			cout << totalLetras<< endl;

			fscanf(archivo, "%s\n", title, 20);//se lee el string y se copia lo que estaba en cadena en nuevo nombre, linea 142
			cout << title << endl;
			while (i < 26) { //mientras que aux sea un elemento y no sea algo vacio o null
				fscanf(archivo, "%s\n", num);
				cout << Abc[i] << ". " << num << endl;
				i++;
			}
			i = 0;
			fscanf(archivo, "%s\n", title2, 20);//se lee el string y se copia lo que estaba en cadena en nuevo nombre, linea 142
			cout << title2 << endl;
			fscanf(archivo, "%s\n", title2, 20);//se lee el string y se copia lo que estaba en cadena en nuevo nombre, linea 142
			cout << title2 << endl;
			while (i < (Ilength(pistaX)-1)) { //mientras que aux sea un elemento y no sea algo vacio o null
				fscanf(archivo, "%s\n", num2);
				cout << num2 << "   ";
				i++;
			}
			i = 0;
			fscanf(archivo, "%s\n", title3, 20);//se lee el string y se copia lo que estaba en cadena en nuevo nombre, linea 142
			cout << title3 << endl;
			fscanf(archivo, "%s\n", title3, 20);//se lee el string y se copia lo que estaba en cadena en nuevo nombre, linea 142
			cout << title3 << endl; 
			fscanf(archivo, "%s\n", title3, 20);//se lee el string y se copia lo que estaba en cadena en nuevo nombre, linea 142
			cout << title3 << endl;
			fscanf(archivo, "%s\n", title3, 20);//se lee el string y se copia lo que estaba en cadena en nuevo nombre, linea 142
			cout << title3 << endl;
			while (i < 722) { //mientras que aux sea un elemento y no sea algo vacio o null
				fscanf(archivo, "%s\n", num3);
				cout << i << ". " << num3 << endl;
				i++;
			}
			
		}
	}
	fclose(archivo);//cerramos el archivo
}

ALLEGRO_SAMPLE* musica;

int main() {
	if (!al_init()) {//Si allegro no se ha inicializado
		fprintf(stderr, "No se puede iniciar allegro!\n");
		return -1; //Retorna error
	}

	//Esta línea de código permite que la ventana tenga la capacidad de cambiar de tamaño
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);

	pantalla = al_create_display(720, 480); //Se crea una pantalla de 720x480

	al_set_window_position(pantalla, 200, 200); //Coloca el display/pantalla en (200, 200)
	al_set_window_title(pantalla, "MATRIX"); //Titulo MATRIX
	if (!pantalla) {//Si la pantalla no se puede crear
		fprintf(stderr, "No se puede crear la pantalla!\n");
		return -1;//Error
	}
	//Líneas para obtener las funcionalidades del uso de las fuentes
	//*******************
	al_init_font_addon();//Inicializa fuente
	al_init_ttf_addon();//Inicializa ttf
	//*******************
	//Línea para obtener las funcionalidades de las imágenes
	//*******************
	al_init_image_addon();//Inicializa imagenes
	//*******************


	//Líneas para obtener las funcionalidades de los audios
	//*******************
	al_install_audio();//Inicializa audio
	
	al_reserve_samples(1000);//Reserva 1000 instancias de muestra
	//*******************
	al_install_keyboard();
	ALLEGRO_KEYBOARD_STATE teclado;
	fuente = al_load_font("Georgia.ttf", 16, NULL);//Determina la fuente como Georgia.ttf con tamaño 16

	//Timers que se necesitarán para el juego
	//**********************************************************
	ALLEGRO_TIMER* primerTimer = al_create_timer(5 / FPS); //Se crea un timer que se activa cada 5/FPS segundos
	ALLEGRO_TIMER* segundoTimer = al_create_timer(5.0 / FPS); //Se crea un timer que se activa cada 5.0/FPS segundos
	ALLEGRO_TIMER* tercerTimer = al_create_timer(1.0 / FPS); //Se crea un timer que se activa cada 1.0/FPS segundos
	ALLEGRO_TIMER* cuartoTimer = al_create_timer(3.0 / FPS); //Se crea un timer que se activa cada 3.0/FPS segundos
	//**********************************************************

	//Se crea una cola de eventos
	ALLEGRO_EVENT_QUEUE* colaEventos = al_create_event_queue(); //Cola eventos

	//Registro de los eventos
	//**********************************************************
	al_register_event_source(colaEventos, al_get_timer_event_source(primerTimer)); //Se activa el timer 1
	al_register_event_source(colaEventos, al_get_timer_event_source(segundoTimer)); //Se activa el timer 2
	al_register_event_source(colaEventos, al_get_timer_event_source(tercerTimer)); //Se activa el timer 3
	al_register_event_source(colaEventos, al_get_timer_event_source(cuartoTimer)); //Se activa el timer 4
	//al_register_event_source(colaEventos, al_get_keyboard_event_source());
	
	al_register_event_source(colaEventos, al_get_keyboard_event_source());
	time_t inicioT, finalT;
	//**********************************************************

	//Inicialización de los timer
	//**********************************************************
	al_start_timer(primerTimer); //Se inicializan los 3 timer
	al_start_timer(segundoTimer);
	al_start_timer(tercerTimer);
	al_start_timer(cuartoTimer);
	//**********************************************************

	musica = al_load_sample("Music.wav");         
	
	ALLEGRO_SAMPLE_INSTANCE* musicaInstance = al_create_sample_instance(musica);

	al_set_sample_instance_playmode(musicaInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(musicaInstance, al_get_default_mixer());


	bool hecho = true; //Se crea una variable para saber si el programa debe continuar en bucle
	ALLEGRO_KEYBOARD_STATE estadoTeclado; //Se crea una variable para detectar que pasa en el teclado
	//al_get_keyboard_state(&estadoTeclado);
	int x = 300; //se definen valores para x y y
	int y = 10;
	char caracter[1];//Se crean 2 variables array para almacenar 1 char
	char caracter2[1];
	char stack[6]; //Se crea un array de 6 char
	char stack2[6]; //borrar


	 
	int coorX[columnas]; //llenado con coordenadas aleatorias
	int coorY[columnas];
	int stepY[columnas+1];//cada columna matrix tiene un bajon
	coorRan(coorX);
	coorRan(coorY);
	LlenePista(NpistaX); //llenado de pistas para evitar valroes random en el array
	Llene(nVeces,0);
	Llene(nVecesLetra, 0);
	Llene(pistaX,-1);
	//char charR[columnas];
	//charRan(charR);
	LlenePista(stepY);
	llenarAbc(Abc);
	//int contArch = 0; si se quiere termianr el programa luego de ciertas repeticiones
	//for (int a = 0; a < Ilength(coorX); a++) //tratandod e toamr en cuenta las pistas del inicio, se comento porq tiraba valores muy raros
	//{	
		//if (coorX[a]<0)
		//{
		//	nPistas(coorX[a] % -721);
			//Pistas(coorX[a] % -721);
		//}
		///nPistas(coorX[a]%721);
		//Pistas(coorX[a] % 721);
	//}
	inicioT = time(NULL);
	
	while (hecho) {

		ALLEGRO_EVENT eventos; //Se crea una variable tipo evento
		
		

		al_wait_for_event(colaEventos, &eventos); //Se espera a que se agregue un evento a la cola de eventos
		//al_clear_to_color(transparente);
		if (eventos.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			//Si se presiona la tecla escape se sale del juego
			switch (eventos.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				hecho = false;
			}
		}
		if (eventos.type == ALLEGRO_EVENT_TIMER) { //si el tipo de evento es un timer
			
			if (eventos.timer.source == cuartoTimer) { //Si el evento es el primer timer
				al_clear_to_color(al_map_rgb(0, 0, 0)); //se limpia la pantalla a negro
				al_play_sample_instance(musicaInstance);
				//caracter[0] = GenerarRandom(); //Se crea un caracter random*
				//AQUI VA LO QUE PASARIA EN EL EVENTO DADO POR EL PRIMER TIMER
				//dibujar(caracter, stack, x, y); //Se dibujan los caracteres*

				//xCoor = rand() % 720; Problema: se pasas cambiado de lado a lado porq el dibujar dibuja una vez y luego cambia posicion y la oclumna no ha bajado hasta ymax
				

				


				for (int i = 0; i < columnas; i++)
				{	
					//IntentoA(prioritario):
					//int trueYcoor = coorY[i] + y;

					//intento viejo
					//dibujar(caracter, stack, xCoor, y);
					//caracter[0] = GenerarRandom(); no funciona, le pone los caracteres de esat columna a la original, podemos alamcenar en array
					
					//Intento char random
					caracter[0] = GenerarRandom();
					
					//Pistas(coorX[i]);
					//nPistas(coorX[i]);//estadisticaas, si se ponen aqui suma cada vez que aparece hasta llegar al final de la pantalla
					nVecesAbc(caracter[0]);//en este caso si funciona, porq queremos ver que tanto se imprime cada letra
					dibujar(caracter, stack, coorX[i], coorY[i] + stepY[i]);
					//dibujar(caracter, stack, coorX[i], coorY[i] + y);
					cont1++;//se logra1 //estadisticas

					//intento efectivo
					//dibujar(caracter, stack, coorX[i], coorY[i]+y); //intento A:este juego con cambio de coordenadas x y con respecto a columan original
					
					//dibujar(caracter, stack, coorX[i], y+40); //este solo cambio de coordenadas x
					stepY[i] = stepY[i] + 20;
				}
				
				

				//y = y + 20; //Se hace para abajo la coordenada Y, cuando solo era una bajada para todos
				//tamano de la impresion
			
				//cout << "Una vez dibujada";
				//system("pause"); para comprobar cada dibujado en pantalla

			}

			
			/*if (y >= 600) { //si la coordenada y es mayor o igual que 700
				y = 1; //La coordenada y se le asigna 10
				
				cout << "Una pista"; //este cuenta cuando ya paso la primera columna, para estadisticas sirve
				//funcional
				x = ranNum(700, 0); //esto mueve en x cada vez que se imprime la columna de nuevo*
				coorRan(coorX);
				coorRan(coorY);
				
				gotoxy(1,4);
				cout << (cont1/30);//se logra 1
				//cout << (cont1);
				gotoxy(30,4);
				cout << pistaX[movy];
				gotoxy(1,6);
				cout << nVeces[movy];
				gotoxy(1, 8);
				cout << Abc[movy];
			}*/
			//lo de arriba fue una adaptacion con respecto a lo suminsitrado, ya despues se quito y se hizo mas logica propia, no comentada

			for (int i = 0; i < columnas; i++)
			{
				//int trueYcoor = coorY[i] + y;
				int trueYcoor = coorY[i] + stepY[i];
				if (trueYcoor >= 700)
				{
					Pistas(coorX[i]);
					nPistas(coorX[i]);
					coorY[i] = rand() % 300;
					coorX[i] = rand() % 719; //intento sin traba
					//y = 10; //La coordenada y se le asigna 10 //intento sin traba
					stepY[i] = 10;
				}
				
			}
			

			gotoxy(1, 4);
			cout << (cont1 / 30);//se logra 1
			gotoxy(60, 4);
			cout << "el contador";
			//cout << (cont1);
			gotoxy(30, 4);
			//nVces//pistaX
			cout << pistaX[2];
			gotoxy(1, 6);
			cout << pistaX[4];
			gotoxy(30, 6);
			cout << pistaX[3];
			gotoxy(1, 10);
			cout << pistaX[5];
			gotoxy(30, 8);
			cout << pistaX[6];
			gotoxy(1, 8);
			cout << pistaX[7];
			gotoxy(30, 10);
			cout << pistaX[8];
			gotoxy(1, 12);
			cout << pistaX[9];
			gotoxy(30, 12);
			cout << pistaX[10];
			gotoxy(1, 14);
			cout << pistaX[11];

			gotoxy(1, 16);
			cout << "Letra "<<Abc[1];//B
			gotoxy(30, 16);
			cout << nVecesLetra[1];//B veces


			
			/*
			contArch++;
			if (contArch == 700)
			{
				hecho = false;
			}*/
		}




		al_flip_display();

		
	}
	finalT = time(NULL);
	totalABC();
	GuardaraArray(nVecesLetra, pistaX, nVeces,1);
	CargarArray();
	//al_destroy_sample(musica);
	cout << "Press anything to exit";
	al_clear_to_color(al_map_rgb(0, 0, 0));
	int sum = (cont1 / 30);
	
	
	
	
	char Sum[15];
	char contUno[15];
	char timeT[15];
	_itoa_s(difftime(finalT, inicioT), timeT, 10);
	_itoa_s(sum, Sum, 10);
	_itoa_s(totalAbc, contUno, 10);
	

	al_draw_text(fuente, al_map_rgb(0, 200, 10), 100, 100, ALLEGRO_ALIGN_CENTRE, "Letras pintadas");
	al_draw_text(fuente, al_map_rgb(0, 200, 10), 250, 100, ALLEGRO_ALIGN_CENTRE, contUno);

	al_draw_text(fuente, al_map_rgb(0, 200, 10), 100, 120, ALLEGRO_ALIGN_CENTRE, "Pistas Pintadas");
	al_draw_text(fuente, al_map_rgb(0, 200, 10), 250, 120, ALLEGRO_ALIGN_CENTRE, Sum);

	al_draw_text(fuente, al_map_rgb(0, 200, 0), 40, 160, ALLEGRO_ALIGN_LEFT, "Tiempo de ejecucion (s)");

	al_draw_text(fuente, al_map_rgb(0, 200, 0), 350, 160, ALLEGRO_ALIGN_RIGHT, timeT);

	

	al_flip_display();
	
	al_destroy_timer(primerTimer);
	
	al_destroy_timer(segundoTimer);
	
	al_destroy_timer(tercerTimer);

	al_destroy_timer(cuartoTimer);
	
	al_destroy_font(fuente);
	
	system("pause");
	al_destroy_display(pantalla);
	system("pause");
	al_destroy_event_queue(colaEventos);
	system("pause");
	return 1;
}