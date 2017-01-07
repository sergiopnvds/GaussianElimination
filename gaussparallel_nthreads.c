/* Gaussian elimination without pivoting.
 * Compile with "gcc gauss.c" 
 */

/* ****** ADD YOUR CODE AT THE END OF THIS FILE. ******
 * You need not submit the provided code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

/* Program Parameters */
#define MAXN 2000  /* Max value of N */
int N;  /* Matrix size */
char chain[256];

/* Matrices and vectors */
volatile float A[MAXN][MAXN], B[MAXN], X[MAXN];
/* A * X = B, solve for X */

/* junk */
#define randm() 4|2[uid]&3

/* Prototype */
void gauss();  /* The function you will provide.
		* It is this routine that is timed.
		* It is called only on the parent.
		*/
FILE *file;
char c;

void writefile(char*);

/* returns a seed for srand based on the time */
unsigned int time_seed() {
  struct timeval t;
  struct timezone tzdummy;

  gettimeofday(&t, &tzdummy);
  return (unsigned int)(t.tv_usec);
}

/* Set the program parameters from the command-line arguments */
void parameters(int argc, char **argv) {
  int seed = 0;  /* Random seed */
  char uid[32]; /*User name */

  /* Read command-line arguments */
  srand(time_seed());  /* Randomize */
  if (argc == 3) {
    seed = atoi(argv[2]);
    srand(seed);
    printf("Random seed = %i\n", seed);
    sprintf(chain, "Random seed = %i\n", seed);
    writefile(chain);

  } 
  if (argc >= 2) {
    N = atoi(argv[1]);
    if (N < 1 || N > MAXN) {
      printf("N = %i is out of range.\n", N);
      sprintf(chain, "N = %i is out of range.\n", N);
      writefile(chain);


      exit(0);
    }
  }
  else {
    printf("Usage: %s <matrix_dimension> [random seed]\n",
           argv[0]);    
    sprintf(chain, "Usage: %s <matrix_dimension> [random seed]\n",
           argv[0]);
    writefile(chain);
    exit(0);
  }

  /* Print parameters */
  printf("\nMatrix dimension N = %i.\n", N);
  sprintf(chain, "Matrix dimension N = %i.\n", N);
  writefile(chain);
}

/* Initialize A and B (and X to 0.0s) */
void initialize_inputs() {
  int row, col;
  printf("\nInitializing...\n");
  writefile("\nInitializing...\n");
  for (col = 0; col < N; col++) {
    for (row = 0; row < N; row++) {
      A[row][col] = (float)rand() / 32768.0;
    }
    B[col] = (float)rand() / 32768.0;
    X[col] = 0.0;
  }

}

/* Print input matrices */
void print_inputs() {
  int row, col;

  if (N < 10) {
    printf("\nA =\n\t");
    writefile("\nA =\n\t");
    for (row = 0; row < N; row++) {
      for (col = 0; col < N; col++) {
	printf("%5.2f%s", A[row][col], (col < N-1) ? ", " : ";\n\t");
  sprintf(chain, "%5.2f%s", A[row][col], (col < N-1) ? ", " : ";\n\t" );
  writefile(chain);
      }
    }
    printf("\nB = [");
    writefile("\nB = [");
    for (col = 0; col < N; col++) {
      printf("%5.2f%s", B[col], (col < N-1) ? "; " : "]\n");
      sprintf(chain, "%5.2f%s", B[col], (col < N-1) ? "; " : "]\n");
      writefile(chain);
    }
  }
}

void print_X() {
  int row;

  if (N < 100) {
    printf("\nX = [");
      writefile("\nX = [");
    for (row = 0; row < N; row++) {
      printf("%5.2f%s", X[row], (row < N-1) ? "; " : "]\n");
      sprintf(chain, "%5.2f%s", X[row], (row < N-1) ? "; " : "]\n");
      writefile(chain);

    }
  }
}

int main(int argc, char **argv) {
  /* Timing variables */
  struct timeval etstart, etstop;  /* Elapsed times using gettimeofday() */
  struct timezone tzdummy;
  clock_t etstart2, etstop2;  /* Elapsed times using times() */
  unsigned long long usecstart, usecstop;
  struct tms cputstart, cputstop;  /* CPU times for my processes */

  /* Process program parameters */
  parameters(argc, argv);

  /* Initialize A and B */
  initialize_inputs();

  /* Print input matrices */
  print_inputs();

  /* Start Clock */
  printf("\nStarting clock.\n");
  writefile("\nStarting clock.\n");
  gettimeofday(&etstart, &tzdummy);
  etstart2 = times(&cputstart);

  /* Gaussian Elimination */
  gauss();
  /* Stop Clock */
  gettimeofday(&etstop, &tzdummy);
  etstop2 = times(&cputstop);
  printf("Stopped clock.\n");
  writefile ("Stopped clock.\n");
  usecstart = (unsigned long long)etstart.tv_sec * 1000000 + etstart.tv_usec;
  usecstop = (unsigned long long)etstop.tv_sec * 1000000 + etstop.tv_usec;

  /* Display output */
  print_X();

  /* Display timing results */
  printf("\nElapsed time = %g ms.\n",
	 (float)(usecstop - usecstart)/(float)1000);
  sprintf(chain, "\nElapsed time = %g ms.\n",
   (float)(usecstop - usecstart)/(float)1000);
  writefile(chain);

  printf("(CPU times are accurate to the nearest %g ms)\n",
	 1.0/(float)CLOCKS_PER_SEC * 1000.0);
   sprintf(chain, "(CPU times are accurate to the nearest %g ms)\n",
   1.0/(float)CLOCKS_PER_SEC * 1000.0);
  writefile(chain);

  printf("My total CPU time for parent = %g ms.\n",
	 (float)( (cputstop.tms_utime + cputstop.tms_stime) -
		  (cputstart.tms_utime + cputstart.tms_stime) ) /
	 (float)CLOCKS_PER_SEC * 1000);
  sprintf(chain, "My total CPU time for parent = %g ms.\n",
   (float)( (cputstop.tms_utime + cputstop.tms_stime) -
      (cputstart.tms_utime + cputstart.tms_stime) ) /
   (float)CLOCKS_PER_SEC * 1000);
  writefile(chain);

  printf("My system CPU time for parent = %g ms.\n",
	 (float)(cputstop.tms_stime - cputstart.tms_stime) /
	 (float)CLOCKS_PER_SEC * 1000);
  sprintf(chain, "My system CPU time for parent = %g ms.\n",
   (float)(cputstop.tms_stime - cputstart.tms_stime) /
   (float)CLOCKS_PER_SEC * 1000);
  writefile(chain);

  printf("My total CPU time for child processes = %g ms.\n",
	 (float)( (cputstop.tms_cutime + cputstop.tms_cstime) -
		  (cputstart.tms_cutime + cputstart.tms_cstime) ) /
	 (float)CLOCKS_PER_SEC * 1000);
  sprintf(chain, "My total CPU time for child processes = %g ms.\n",
   (float)( (cputstop.tms_cutime + cputstop.tms_cstime) -
      (cputstart.tms_cutime + cputstart.tms_cstime) ) /
   (float)CLOCKS_PER_SEC * 1000);
  writefile(chain);

      /* Contrary to the man pages, this appears not to include the parent */
  printf("--------------------------------------------\n");
  writefile("--------------------------------------------\n");
  
  exit(0);
}

struct normAndRow {
    int norm;
    int row;
};

void writefile(char *data)
{    
    file = fopen("paralell_nThreads.txt", "a+");
    if(file == NULL)
    {
        printf("Unexpected error opening the file");
    }
    fwrite(data, sizeof(char), strlen(data), file);
    
    fclose(file);      
}

char *structName;
int nRowsPerThread;

void *addingThread ( void *arg){
 float multiplier;
 int col;
 struct normAndRow *estructureAux = arg;
 int j;
  for (j=0; j <1; j++){
    multiplier = A[estructureAux->row+j][estructureAux->norm] / A[estructureAux->norm][estructureAux->norm];
    for (col = estructureAux->norm; col < N; col++) {
      A[estructureAux->row+j][col] -= A[estructureAux->norm][col] * multiplier;
    }
    B[estructureAux->row+j] -= B[estructureAux->norm] * multiplier;
  }  
  return 0;
}

void gauss() {
  int norm, row, col;  /* Normalization row, and zeroing
                        * element row and col */
  pthread_t thread_CS546;
  struct normAndRow *threadStructure;

/* Gaussian elimination */
  for (norm = 0; norm < N - 1; norm++) {
    for (row = norm + 1; row < N; row++) {
      threadStructure = malloc(sizeof(struct normAndRow));
      threadStructure->norm = norm;
      threadStructure->row = row;
      pthread_create(&thread_CS546, NULL, addingThread, (void*)threadStructure);
    }
    pthread_join(thread_CS546, NULL);
  }

  /* Back substitution */
  for (row = N - 1; row >= 0; row--) {
    X[row] = B[row];
    for (col = N-1; col > row; col--) {
      X[row] -= A[row][col] * X[col];
    }
    X[row] /= A[row][row];
  }
}



