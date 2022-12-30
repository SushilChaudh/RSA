//Sequential program for rsa encryption and decryption

#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>


int cipher =1, plain =1, len;

// euclidian algorithm to find gcd
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
    return d;
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

//rsa algorithm
int main()
{
    //local variable
    int p =151 ;           //first prime
    int q =157 ;           //second prime
    int n ;              //modulus
    int phi_n;           //totient
    int e;               //public key
    int d;               //private key
    char msg[1500];     // plain text
    int c[1500];               //cipher text
    int m[1500];               //decrypted text
        struct timeval start, end;
   

    n = p*q;
    phi_n = (p-1)*(q-1);


    //Lets read plain text
    FILE* ptr;
    ptr = fopen("plain.txt", "r");
    while(fgets(msg, sizeof(msg), ptr)!=NULL);
    fclose(ptr);

    
    // 1 < e < phi_n
    //lets find public key
    
    e = publickey(phi_n);

    // lets find private key
    d = privatekey(e, phi_n);
   
    //lets encrypt
    gettimeofday(&start, NULL);
    encrypt(e, n, msg, c);

    //lets decrypt
    decrypt(d, n, c, m);
    
    gettimeofday(&end, NULL);
    double execution_time = (end.tv_sec -start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
    //lets print the results
    printf("Public key: %d\n", e);
    printf("Private key: %d\n", d);
    printf("Original text: \n\t%s\n", msg);
    printf("Length of plain text is: %d\n",len);
    printf("Encrypted text:\n\t");
    int k;
    for(k=0; k<len; k++)
        printf("%c", c[k]);
    printf("\nDecrypted text:\n\t");
    for(k=0; k<len; k++)
        printf("%c", m[k]);
    printf("\nExecution time: %f\n", execution_time);


}