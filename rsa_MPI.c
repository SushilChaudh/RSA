
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <unistd.h>

#define MIN(a,b)  ((a)<(b)?(a):(b))
#define BLOCK_LOW(id,p,n)  ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n) (BLOCK_LOW(((id)+1),p,n)-1)
//#define BLOCK_SIZE(id,p,n) (floor(n/p))
//#define BLOCK_SIZE(id,p,n) ((BLOCK_LOW(((id)+1),p,n))-(BLOCK_LOW(id,p,n)))
#define BLOCK_OWNER(index,p,n) (((p)*(index)+1)-1)/(n))

int cipher =1, plain =1, len;

int gcd (int a, int b){
    int tmp, r;
    while(1){
        r = a % b;
        if(r == 0){
            return b;
            break;
        }
        a = b;
        b = r;
    }
}

int publickey(int totient){
    int e;
    int coprime;         //coprime checker
    for (e = 2 ; e < totient; e++){
        //check coprime
        coprime = gcd(totient, e);
        if (coprime == 1){
            //take the 1st coprime as e:
            break;
        }
    }
    return e;
}

int privatekey(int e, int totient){

    // e*d = 1 mod phi_n
    // e*d = Z * phi_n +1
    // suppose Z = 1
    int z, d;
    for (z=1; z<totient; z++){
        if (((z*totient) + 1)%e ==0){
            d = ((z*totient) + 1)/e ;
            //taking the 1st number only
            break;
	}
    }
}
void encrypt(int e, int n, char msg[], int c[]){

    int i, k;
    len = strlen(msg);
    for (k=0; k<len; k++){
        for (i = 1; i<=e; i++){
            cipher = cipher * msg[k];
            cipher= cipher % n;
        }
    c[k] = cipher;
    cipher =1;

    }
}

void decrypt(int d, int n, int c[], int m[]){
    int j, k;
    for (k=0; k<len; k++){
        for (j = 1; j<=d; j++){
            plain = plain * c[k];
            plain = plain % n;
        }
    m[k] = plain;
    plain = 1;

    }

}

int main(int argc, char *argv[])
{
	int id; //id of the process
	int proc; //number of the processes
	//local variable
    	int p =151 ;           //first prime
    	int q =157 ;           //second prime
    	int n ;              //modulus
    	int phi_n;           //totient
    	int e;               //public key
    	int d;               //private key
    	char msg[2000];     // plain text
    	
	int low_value; //starting value
	int high_value; //end value
	int size; //size of the block
	int l;
	int* allmessage; //decrypted message
	int* allencrypt; //encrypted message
	double elapsed_time, total_time;
	n = p*q;
    	phi_n = (p-1)*(q-1);

	FILE* ptr;
    	ptr = fopen("plain.txt", "r");
    	while(fgets(msg, sizeof(msg), ptr)!=NULL);
    	fclose(ptr);

	int cipher =1, plain =1;
	MPI_Init (&argc, &argv);
 	MPI_Barrier(MPI_COMM_WORLD);
 	elapsed_time = -MPI_Wtime();
 	MPI_Comm_rank (MPI_COMM_WORLD, &id);
 	MPI_Comm_size (MPI_COMM_WORLD, &proc);

	l = strlen(msg);
	//printf("The length of the data is %d\n",l);
	low_value = BLOCK_LOW(id,proc,l-1);
 	high_value = BLOCK_HIGH(id,proc,l-1);
 	//size = BLOCK_SIZE(id,proc,l-1);
	size = floor(l/proc);
	char message[size];
	//printf("%d\n",size);
	for(int a = 0; a < size; a++)
	{
		message[a] = msg[low_value+a];
	}
	//printf("%s\n",message);
	//public key
	e = publickey(phi_n);

    	// lets find private key
    	d = privatekey(e, phi_n);
	int c[size];
	int m[size];
	encrypt(e, n, message, c);

    	//lets decrypt
    	decrypt(d, n, c, m);
	int k, j;
	if(!id)
	{
		allencrypt = (int *)malloc(proc*size*sizeof(int));
		allmessage = (int *)malloc(proc*size*sizeof(int));
		printf("The public key is %d\n",e);
		printf("The private key is %d\n",d);
		printf("Orignal plain text: \n%s\n", msg);
		printf("Length of plain text:\t%d\n", l);
	}
	MPI_Gather(c,size,MPI_INT,allencrypt,size,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Gather(m,size,MPI_INT,allmessage,size,MPI_INT,0,MPI_COMM_WORLD);
	
	elapsed_time += MPI_Wtime();
	MPI_Reduce(&elapsed_time, &total_time, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if(!id)
        {
                for(k=0; k<l; k++)
                        printf("%c", allencrypt[k]);
        }
	printf("\n");
	if(!id)
	{
		for(k=0; k<l; k++)
                	printf("%c", allmessage[k]);
		printf("\n");
        	printf("The taken for the program is: %f\nSum of execution time by all processor %f\n", elapsed_time, total_time);
	}
	
	
	MPI_Finalize();
}
