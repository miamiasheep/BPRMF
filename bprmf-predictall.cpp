#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<cassert>
#include<vector>
#include<algorithm>
#include"matrix.h"
#include"usage.h"
#include"data.h"
#include"mf.h"
using namespace std;
int main(int argc,const char* argv[]){
    if( argc < 1 + 1){
        exit_with_predictall_usage();
    }    
    char* model_file = (char*) argv[1];
    char* predict_file = NULL;
    if(argc > 1+1){
        predict_file = (char*) argv[2];
    }else{
        predict_file = new char[ strlen(model_file) + 10 ];
        sprintf(predict_file,"%s.pred",model_file);
    }
    fprintf(stderr,"model file = %s\n",model_file);
    fprintf(stderr,"predict file = %s\n",predict_file);

    Matrix P;
    Matrix Q;
    Parameter param;
    load_model(P, Q, param, model_file);
    FILE* wp = fopen(predict_file,"w");
    double val;
    for(int i=0;i<param.numU;i++){
        for(int j=0;j<param.numI;j++){
            val = 0.0;
            for(int k=0;k<param.K;k++)
                val += P.value[i][k] * Q.value[k][j];
            fprintf(wp,"%lf\t", val);
        }           
        fprintf(wp,"\n");
    }
    fclose(wp);



    return 0;
}
