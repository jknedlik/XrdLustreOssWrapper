long uid;
#define typeof __typeof__
#include "lfs.c"

int main(int argc, char **argv){
struct qsStruct qs =getQuotaSpace();

printf("Max/Total:%lli \nUsed:%lli \nFree:%lli \n",qs.Total,qs.Curr,qs.Total-qs.Curr);

}
