#include <time.h>
#include <stdlib.h>


#define SIZE 8192
#define bsize 16
//double size is 8
// L1 cache size is 32K
// the largest block to accomodate in L1 caceh

double a[SIZE][SIZE];
double b[SIZE][SIZE];
double c[SIZE][SIZE];


int tmm(){
  double sum=0; int ii, jj, kk, i, j, k;

  #pragma clang loop recompute (c ii)
  for (kk=0; kk<SIZE; kk=kk+bsize){
    for ( ii =0; ii <SIZE; ii=ii+bsize){
      for ( jj =0; jj <SIZE; jj=jj+bsize){
        for ( i =ii ; i <(ii+bsize); i=i+1){
          for ( j=jj ; j<(jj+bsize); j=j+1)
            {
              sum = c[i ][ j ];
              for (k=kk; k<(kk+bsize); k=k+1){
                sum += a[i][k]*b[k ][ j ];
              }
              c[ i ][ j ] = sum;
            } //end of j loop
        }//end of i loop
      }//end of jj loop
      //HERE: insertion point
    }//end of ii loop
  }//end of kk loop

  return 0;
}




int main(){

  srand(time(NULL));

  for(int l=0; l<SIZE; l++){
    for(int ll=0; ll<SIZE; ll++){
      a[l][ll]= rand() / (RAND_MAX + 1.);
      b[l][ll]= rand() / (RAND_MAX + 1.);
      ll++;
    }l++;
  }
  tmm();
}
