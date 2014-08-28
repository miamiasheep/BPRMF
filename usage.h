#ifndef _USAGE_H_
#define _USAGE_H_
#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<utility>
#include<vector>
using namespace std;
class Parameter{
    public:
        int K, numU, numI, nSample;
        double learning_rate, reg_user, reg_item, valid_p;
        Parameter(){
            learning_rate = 0.05;
            numU = numI = -1;
            K = 10;
            nSample = 1;
            reg_user = 0.001;
            reg_item = 0.001;            
            valid_p = 0.1;
        };
};

void exit_with_predictall_usage(){
	printf(
	"Usage: bprmf-predictall model_file [predict_file]\n"
	);
    exit(1);
}

void exit_with_predict_usage(){
	printf(
	"Usage: bprmf-predict testing_set_file model_file [predict_file]\n"
    "file_format:\n"
    "  uid iid\n"
	);
    exit(1);
}


void exit_with_usage(){
	printf(
	"Usage: bprmf [options] training_set_file [model_file]\n"
	"options:\n"
    "  -K hidden factor (default 10)\n"
	"  -L learning rate (default 0.05)\n"
	"  -U #(User)\n"
	"  -I #(Item)\n"
	"  -u user regularization (defaut 0.001)\n"
	"  -i item regularization (defaut 0.001)\n"
	"  -p percent of validation (defaut 0.1)\n"
	"  -s number of negative samples for each postive sample (defaut 1)\n"
    "file_format:\n"
    "  #User\n"
    "  #Item\n"
    "  uid iid\n"
	);
    exit(1);
}

int read_parameter(int argc, const char* argv[], Parameter &param){
    int i = 1;
    for(;i<argc;i++){
        if(argv[i][0] != '-') break;
        if(++i >= argc) exit_with_usage();
		switch(argv[i-1][1])
		{
			case 'K':
				param.K = atoi(argv[i]);
				break;
            case 'L':
                param.learning_rate = atof(argv[i]);
                break;
            case 'U':
                param.numU = atoi(argv[i]);
                break;
            case 'I':
                param.numI = atoi(argv[i]);
                break;
            case 's':
                param.nSample = atoi(argv[i]);
                break;
            case 'u':
                param.reg_user = atof(argv[i]);
                break;
            case 'i':
                param.reg_item = atof(argv[i]);
                break;
            case 'p':
                param.valid_p = atof(argv[i]);
                break;
			default:
				fprintf(stderr,"Unknown option: -%c\n", argv[i-1][1]);
				exit_with_usage();
        }
    }

    return i;
}


double** create_2d_double_array(int M, int N){
    double** val;
    val = new double*[M];
    for(int i=0;i<M;i++){
        val[i] = new double[N];
    }
    for(int i=0;i<M;i++) for(int j=0;j<N;j++) val[i][j] = 0.0;
    return val;
}
void delete_2d_double_array(double** val, int M, int N){
    if(M > 0){
        for(int i=0;i<M;i++){
            delete [] val[i];
        }
        delete [] val;
    }
}

int** create_2d_int_array(int M, int N){
    int** val;
    val = new int*[M];
    for(int i=0;i<M;i++){
        val[i] = new int[N];
    }
    for(int i=0;i<M;i++) for(int j=0;j<N;j++) val[i][j] = 0;
    return val;
}
void delete_2d_int_array(int** val, int M, int N){
    if(M > 0){
        for(int i=0;i<M;i++){
            delete [] val[i];
        }
        delete [] val;
    }
}

#endif
