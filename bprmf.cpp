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
        exit_with_usage();
    }    
    char* train_file = NULL;
    char* model_file = NULL;
    char* submodel_file = NULL;
    Parameter param;
    int train_idx = read_parameter(argc, argv, param);
    train_file = (char*) argv[train_idx];
    if(train_idx!=argc-1){
        model_file = (char*)argv[train_idx+1];
    }
    assert(train_file != NULL);
    fprintf(stderr,"train file = %s\n",train_file);
    if(model_file == NULL){
        model_file = new char[ strlen(train_file) + 10 ];
        sprintf(model_file,"%s.model",train_file);
    }
    submodel_file = new char[ strlen(model_file) + 10 ];
    sprintf(submodel_file,"%s.sub",model_file);

    fprintf(stderr,"model file = %s\n",model_file);
    fprintf(stderr,"submodel file = %s\n",submodel_file);
    Data data;    
    data.load( train_file, param );

    Data subtrain;
    Data validation;
    data.split( subtrain, validation, 1.0 - param.valid_p);

    srand(0);
    Matrix P = Matrix( param.numU, param.K );
    Matrix Q = Matrix( param.K, param.numI );
    

    fprintf(stderr,"Start validation...\n");
    int iter;
    double max_auc = 0.0;
    int max_iter = -1;
    for(iter = 0;;iter++){
        subtrain.sample_pair( param );
        validation.sample_pair( param );

        mf_update( subtrain, P, Q, param );

        subtrain.sample_pair( param );
        validation.sample_pair( param );

        double valid_auc = mf_test(validation, P, Q, param);
        double train_auc = mf_test(subtrain, P, Q, param);
        if( valid_auc > max_auc ){
            max_auc = valid_auc;
            max_iter = iter;
        }
        fprintf(stderr,"#%d, train_auc = %lf, valid_auc = %lf, (%lf, %d)\n",iter,train_auc,valid_auc, max_auc, max_iter);
        if( valid_auc<max_auc && iter-max_iter>20){
            break;
        }
    }
    fprintf(stderr,"Best iter : %d\n", max_iter);
    
    fprintf(stderr,"Start training...\n");
    srand(0);
    P.initialize();
    Q.initialize();
    for(int i=0;i<max_iter+1;i++){
        data.sample_pair(param);
        mf_update( data, P, Q, param );
        data.sample_pair(param);
        double train_auc = mf_test(data, P, Q, param);
        fprintf(stderr,"#%d, train_auc = %lf\n",i,train_auc);
    }
    
    fprintf(stderr,"Write Model...\n");
    write_model(P,Q,param,model_file);


    fprintf(stderr,"Start training...\n");
    srand(0);
    P.initialize();
    Q.initialize();
    for(int i=0;i<max_iter;i++){
        subtrain.sample_pair(param);
        mf_update( subtrain, P, Q, param );
        subtrain.sample_pair(param);
        double train_auc = mf_test(subtrain, P, Q, param);
        fprintf(stderr,"#%d, subtrain_auc = %lf\n",i,train_auc);
    }
    
    fprintf(stderr,"Write Model...\n");
    write_model(P,Q,param,submodel_file);





}
