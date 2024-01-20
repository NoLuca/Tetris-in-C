#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 20

char field[HEIGHT][WIDTH];

// Struttura per contenere le informazioni sui pezzi
typedef struct{
int x, y;
char shape[4][4];
}Piece;

// Array delle forme
char shapes[14][4][4] = {
{
{' ', ' ', ' ', ' '},
{' ', ' ', ' ', ' '},
{' ', 'x', 'x', ' '},
{' ', 'x', 'x', ' '}
},
{
{' ', ' ', ' ', ' '},
{' ', ' ', ' ', ' '},
{' ', 'x', ' ', ' '},
{' ', 'x', 'x', 'x'}
},
{
{' ', ' ', ' ', ' '},
{' ', ' ', ' ', ' '},
{' ', ' ', ' ', 'x'},
{' ', 'x', 'x', 'x'}
},
{
{' ', ' ', ' ', ' '},
{' ', 'x', ' ', ' '},
{' ', 'x', ' ', ' '},
{' ', 'x', 'x', ' '}
},
{
{' ', 'x', 'x', ' '},
{' ', 'x', ' ', ' '},
{' ', 'x', ' ', ' '},
{' ', ' ', ' ', ' '}
},
{
{' ', ' ', ' ', ' '},
{' ', ' ', ' ', ' '},
{' ', ' ', 'x', ' '},
{' ', 'x', 'x', 'x'}
},
{
{' ', ' ', ' ', ' '},
{' ', 'x', ' ', ' '},
{' ', 'x', 'x', ' '},
{' ', 'x', ' ', ' '}
},
{
{' ', ' ', ' ', ' '},
{' ', ' ', 'x', ' '},
{' ', 'x', 'x', ' '},
{' ', ' ', 'x', ' '}
},
{
{' ', ' ', ' ', ' '},
{' ', ' ', ' ', ' '},
{' ', 'x', 'x', 'x'},
{' ', ' ', 'x', ' '}
},
{
{' ', ' ', ' ', ' '},
{' ', ' ', ' ', ' '},
{' ', 'x', 'x', ' '},
{' ', ' ', 'x', 'x'}
},
{
{' ', ' ', ' ', ' '},
{' ', ' ', 'x', ' '},
{' ', 'x', 'x', ' '},
{' ', 'x', ' ', ' '}
},
{
{' ', ' ', ' ', ' '},
{' ', 'x', ' ', ' '},
{' ', 'x', 'x', ' '},
{' ', ' ', 'x', ' '}
},
{
{' ', ' ', ' ', 'x'},
{' ', ' ', ' ', 'x'},
{' ', ' ', ' ', 'x'},
{' ', ' ', ' ', 'x'}
},
{
{' ', ' ', ' ', ' '},
{' ', ' ', ' ', ' '},
{' ', ' ', ' ', ' '},
{'x', 'x', 'x', 'x'}
},
};

// Inizializzazione del campo di gioco con spazi vuoti
void initField() {
	int i, j;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			field[i][j] = ' ';
		}
	}
}

// Disegno del campo di gioco sulla console
void drawField(Piece piece) {
    int i, j, score;

    // Stampa il bordo superiore del campo
    printf("+");
    for (i = 0; i < WIDTH; ++i) {
        printf("-");
    }
    printf("+\n");

    // Stampa la forma nella posizione corretta all'interno del campo di gioco
    for (i = 0; i < HEIGHT; i++) {
        printf("|"); // Stampa il lato sinistro del campo        
        for (j = 0; j < WIDTH; j++) {
            if (i >= piece.y && i < piece.y + 4 && j >= piece.x && j < piece.x + 4 && piece.shape[i - piece.y][j - piece.x] != ' ') {
                printf("\033[1;31m%c\033[1;0m", piece.shape[i - piece.y][j - piece.x]);
            } else {
                printf("\033[1;32m%c\033[1;0m", field[i][j]);
            }
        }
        printf("|\n"); // Stampa il lato destro del campo
    }

    // Stampa il bordo inferiore del campo
    printf("+");
    for (i = 0; i < WIDTH; ++i) {
        printf("-");
    }
    printf("+\n");

    // Salvare in file il punteggio
    FILE *file = fopen("score.txt", "r");
    fscanf(file,"%d", &score);
    fclose(file);
    printf("Score: %d\n", score);
}

// Funzione per controllare le collisioni
int checkCollision(Piece piece) {
    int i, j;
    // Doppio ciclo per scorrere la matrice della forma del pezzo
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (piece.shape[i][j] != ' ') {
                int x = piece.x + j;
                int y = piece.y + i;

                // Controlla se il pezzo si trova al di fuori dei bordi del campo
                if (x < 0 || x >= WIDTH || y >= HEIGHT) {
                    return 1;
                }
                // Controlla se il pezzo si trova al fondo del campo di gioco
                if (y == HEIGHT - 1) {
                    return 1;
                }
                // Controlla se il pezzo si sovrappone ad una parte già presente nel campo
                if (y >= 0 && field[y][x] != ' ') {
                    return 1;
                }
                // Controlla se il pezzo si sovrappone ad una parte già presente nel prossimo movimento
                if (y+1 >= 0 && field[y+1][x] != ' ') {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// Muove il pezzo
void movePiece(Piece *piece, int dx) {
    // salva la posizione attuale del pezzo
    int x = piece->x;
    int y = piece->y;
    // sposta il pezzo di dx
    piece->x += dx;

    // controlla la collisione
    if (checkCollision(*piece)) {
        // se c'è collisione, ripristina la posizione originale
        piece->x = x;
        piece->y = y;
    }
}

// Funzione per generare un nuovo pezzo
Piece newPiece() {
	Piece piece;
    // Imposta le cordinate del pezzo a metà del campo di gioco e l'altezza a zero
	piece.x = WIDTH / 2 - 2;
	piece.y = 0;

	// Genera una forma a caso
	int forme = rand() % 14;
	int i, j;
    // Doppio ciclo per scorrere la matrice della forma del pezzo
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			piece.shape[i][j] = shapes[forme][i][j];
		}
	}

	return piece;

}

// Controlla se ci sono righe complete nel campo di gioco, se si le resetta e aumenta lo score
void removeFullRow(int score) {
    int i, j, k;

    //Scorre il campo di gioco per individuare righe complete
    for (i = 0; i < HEIGHT; i++) {
        int fullRow = 1;
        for (j = 0; j < WIDTH; j++) {
            if (field[i][j] == ' ') {
                fullRow = 0;
                break;
            }
        }
        if (fullRow) {

            // Inserire qua lo score dal file txt
            FILE *file = fopen("score.txt", "r");
            fscanf(file,"%d", &score);
            fclose(file);

            score++;

            // Esportare qua lo score nel file .txt
            freopen("score.txt", "w", file); 
            fprintf(file, "%d\n", score); 
            fclose(file); 

            // Rimuove la riga completa spostando le righe sopra di essa in giù di una posizione
            for (k = i; k > 0; k--) {
                for (j = 0; j < WIDTH; j++) {
                    field[k][j] = field[k-1][j];
                }
            }
        }
    }
}

void addPieceToField(Piece piece, int score) {
    int i, j;

    // Doppio ciclo per scorrere la matrice della forma del pezzo
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            // Controllare se il carattere in posizione i e j della forma del pezzo è diverso da spazio vuoto
            if (piece.shape[i][j] != ' ') {
                // Se è diverso, inserisce il carattere nella posizione corrispondente del campo di gioco
                int x = piece.x + j;
                int y = piece.y + i;

                // Controlla se le conrdinate del pezzo si trovano entro i limiti del campo di gioco
                if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) {
                    // Questo if controlla se la posizione x e y del nuovo pezzo è occupata se si è game over
                    if (field[y][x] != ' ') {

                        // Inserire qua lo score dal file txt
                        FILE *file = fopen("score.txt", "r");
                        fscanf(file,"%d", &score);
                        fclose(file);

                        freopen("score.txt", "w", file); 
                        fprintf(file, "%d\n", score); 
                        fclose(file); 

                        system("cls");

                        printf("   _____                         ____         \n");
                        printf("  / ____|                       / __ \\       \n");
                        printf(" | |  __  __ _ _ __ ___   ___  | |  | |__   __  ___  _ __\n");
                        printf(" | | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | |\\ \\ / / / _ \\| '_ /\n");
                        printf(" | |__| | (_| | | | | | |  __/ | |__| | \\ V / |  __/| |\n");
                        printf("  \\_____|\\__,_|_| |_| |_|\\__    \\____/   \\_/   \\__  |_|\n");

                        printf("Hai totalizzato %d punti\n", score);
                        exit(0);
                    }
                    field[y][x] = piece.shape[i][j];
                    removeFullRow(score);
                }
            }
        }
    }
}

//Funzione per ruotare il pezzo, lo messa come commento perchè causa bug, nel main si vede come è stato implementato per gestire l'input

void rotatePiece(Piece *piece) {
    char temp[4][4];
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temp[i][j] = piece->shape[i][j];
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            piece->shape[i][j] = temp[3-j][i];
        }
    }

    //Controlla se la rotazione è attaccata al bordo
    while (checkCollision(*piece)) {
        // Se la rotazione causa una collisione, annulla la rotazione
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                piece->shape[i][j] = temp[i][j];
            }
        }
        // Ripristina la forma originale
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                temp[i][j] = piece->shape[i][j];
            }
        }
        return;
    }
}

int main() {
    srand(time(NULL)); // Inizzializza il valore random con un valore diverso

    system("cls");
    //Asci art per il menu'
    printf("  .___________. _______ .___________..______   __      _______.\n");
    printf("  |           ||   ____||           ||   _  \\ |  |    /       |\n");
    printf("  `---|  |----`|  |__   `---|  |----`|  |_)  ||  |   |   (----`\n");
    printf("      |  |     |   __|      |  |     |      / |  |    \\   \\    \n");
    printf("      |  |     |  |____     |  |     |  |\\  \\.|  |.----)   |   \n");
    printf("      |__|     |_______|    |__|     | _| `._||__||_______/    \n");

    printf("\nPremi [invio] per iniziare");
    getchar();

    // Inizializzazione score
    FILE *file = fopen("score.txt", "w");
    fprintf(file, "%d", 0); 
    fclose(file); 

    int score=0;

    // Inizializzazione del campo di gioco
    initField();

    // Variabile per il controllo del tempo
    clock_t startTime = clock();

    // Generazione di un nuovo pezzo
    Piece piece = newPiece();

    while (1) {
        // Pulizia della console e disegno del campo di gioco
        system("cls");
        drawField(piece);

        int pressedKey;
        if(kbhit()){

            // Lettura dell'input dell'utente
            fflush(stdin);
            pressedKey = getch();

            // Gestione dell'input dell'utente
            if (pressedKey == 'a') {

                movePiece(&piece, -1);

            } else if (pressedKey == 'd') {

                movePiece(&piece, 1);

            } else if (pressedKey == 's') {

                /*Controlla se il pezzo ha raggiunto il fondo, 
                questa parte è ripetatuta più volte nel main, 
                qua viene utilizzata per far scendere 
                più velocemente il pezzo*/

                if (checkCollision(piece)) {

                // Aggiunge il pezzo al campo di gioco
                addPieceToField(piece, score);

                // Genera un nuovo pezzo
                piece = newPiece();
                } else {
                    // Muove il pezzo verso il basso
                    piece.y++;
                }
            } else if (pressedKey == 'w') {
                rotatePiece(&piece);
            }
        }

        // Controlla se il pezzo ha raggiunto il fondo
        if (checkCollision(piece)) {

            // Aggiunge il pezzo al campo di gioco
            addPieceToField(piece, score);

            // Genera un nuovo pezzo
            piece = newPiece();
        }

        // Controlla se è trascorso abbastanza tempo dall'ultima volta che il pezzo è sceso
        if ((clock() - startTime) / CLOCKS_PER_SEC > 0.2) {

            if (checkCollision(piece)) {

                // Aggiunge il pezzo al campo di gioco
                addPieceToField(piece, score);

                // Genera un nuovo pezzo
                piece = newPiece();

            } else {
                // Muove il pezzo verso il basso
                piece.y++;
            }

            startTime = clock();
        }

        // Attesa prima della prossima mossa
        Sleep(30);
        
    }

    return 0;
}