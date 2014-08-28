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
    if( argc < 1 + 2){
        exit_with_predict_usage();
    }    
    char* test_file = (char*) argv[1];
    char* model_file = (char*) argv[2];
    char* predict_file = NULL;
    if(argc > 1+2){
        predict_file = (char*) argv[2];
    }else{
        predict_file = new char[ strlen(test_file) + 10 ];
        sprintf(predict_file,"%s.pred",test_file);
    }
    fprintf(stderr,"model file = %s\n",model_file);
    fprintf(stderr,"predict file = %s\n",predict_file);

    Matrix P;
    Matrix Q;
    Parameter param;
    load_model(P, Q, param, model_file);

    Data data;
    data.load( test_file, param );
    data.sample_pair(param);
    double test_auc = mf_test(data, P, Q, param);
    fprintf(stderr,"test_auc = %lf\n",test_auc);



    FILE* fp = fopen(test_file, "r");

    FILE* wp = fopen(predict_file,"w");

    int uu, ii;
    double val;
    while(fscanf(fp,"%d%d",&uu,&ii)!=EOF){
        val = 0.0;
        for(int k=0;k<param.K;k++)
            val += P.value[uu][k] * Q.value[k][ii];
        fprintf(wp,"%lf\n", val);
    }

    fclose(fp);
    fclose(wp);




    return 0;
}
