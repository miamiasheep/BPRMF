#ifndef _MF_H_
#define _MF_H_
#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<cerrno>
#include<utility>
#include<vector>
#include"usage.h"
#include"matrix.h"
#include"data.h"
using namespace std;

void write_model(Matrix &P, Matrix &Q, Parameter &param, char* filename){
    FILE* wp = fopen(filename, "w");
    fprintf(wp,"%d %d %d\n",param.K,param.numU,param.numI);
    fprintf(wp,"%d %d\n",P.row,P.col);
    for(int i=0;i<P.row;i++){
        for(int j=0;j<P.col;j++)
            fprintf(wp,"%lf ", P.value[i][j]);
        fprintf(wp,"\n");
    }

    fprintf(wp,"%d %d\n",Q.row,Q.col);
    for(int i=0;i<Q.row;i++){
        for(int j=0;j<Q.col;j++)
            fprintf(wp,"%lf ", Q.value[i][j]);
        fprintf(wp,"\n");
    }
    fclose(wp);
}


void load_model(Matrix &P, Matrix &Q, Parameter &param, char* filename){
    FILE* fp = fopen(filename, "r");
    fscanf(fp,"%d%d%d",&param.K,&param.numU,&param.numI);
    fscanf(fp,"%d%d",&P.row,&P.col);
    P.create_matrix(P.row, P.col);
    for(int i=0;i<P.row;i++)
        for(int j=0;j<P.col;j++)
            fscanf(fp,"%lf",&P.value[i][j]);

    fscanf(fp,"%d%d",&Q.row,&Q.col);
    Q.create_matrix(Q.row, Q.col);
    for(int i=0;i<Q.row;i++)
        for(int j=0;j<Q.col;j++)
            fscanf(fp,"%lf",&Q.value[i][j]);
    fclose(fp);
}


void mf_update(Data &data_train, Matrix &P, Matrix &Q, Parameter &param){
    double *new_p = new double[param.K];
    double *new_qi = new double[param.K];
    double *new_qj = new double[param.K];
    double now_i, now_j, err, sig;
    for(int i=0;i<data_train.nData;i++){
        int uu = data_train.uid[i];
        int iii = data_train.iidi[i];
        int iij = data_train.iidj[i];

        now_i = 0.0;
        now_j = 0.0;
        for(int j=0;j<param.K;j++){
            now_i += P.value[uu][j] * Q.value[j][iii];
            now_j += P.value[uu][j] * Q.value[j][iij];
        }

        err = now_i - now_j;
        sig = exp(-err) / (1.0 + exp(-err));
        if(errno == ERANGE){
            sig = 1.0;
        }

        for(int j=0;j<param.K;j++){
            new_p[j] = P.value[uu][j] - param.learning_rate*(-sig*(Q.value[j][iii]-Q.value[j][iij]) + param.reg_user*P.value[uu][j] );
            new_qi[j] = Q.value[j][iii] - param.learning_rate*( -sig*P.value[uu][j] + param.reg_item*Q.value[j][iii] );
            new_qj[j] = Q.value[j][iij] - param.learning_rate*( -sig*(-P.value[uu][j]) + param.reg_item*Q.value[j][iij] );
        }
        for(int j=0;j<param.K;j++){
            P.value[uu][j] = new_p[j];
            Q.value[j][iii] = new_qi[j];
            Q.value[j][iij] = new_qj[j];
        }
    }
    delete new_p;
    delete new_qi;
    delete new_qj;
}

double mf_test(Data &data_test, Matrix &P, Matrix &Q, Parameter &param){
    double auc = 0.0;
    for(int i=0;i<data_test.nData;i++){
        int uu = data_test.uid[i];
        int iii = data_test.iidi[i];
        int iij = data_test.iidj[i];
        
        double nowi = 0.0;
        double nowj = 0.0;
        for(int j=0;j<param.K;j++){
            nowi += P.value[uu][j] * Q.value[j][iii];
            nowj += P.value[uu][j] * Q.value[j][iij];            
        }
        if( nowi > nowj ){
            auc+=1.0;
        }
    }
    auc = auc / (double)data_test.nData;
    return auc;
}

#endif
