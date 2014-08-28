#ifndef _DATA_H_
#define _DATA_H_
#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<cassert>
#include<utility>
#include<vector>
#include<algorithm>
#include<set>
#include"usage.h"
using namespace std;




class Data{
    public:
        int nData, data_length;
        int *uid, *iidi, *iidj;
        vector< pair<int, int>  > user_item;
        Data(){
            nData = 0;
            data_length = 1024; 
            uid = (int*) malloc( data_length * sizeof(int) );
            iidi = (int*) malloc( data_length * sizeof(int) );
            iidj = (int*) malloc( data_length * sizeof(int) );
        };
        ~Data(){ if(data_length){ free(uid); free(iidi); free(iidj); } };
        void insert(int uu, int iii, int iij);
        void load(char *filename, Parameter &param);
        void split(Data &data_a, Data &data_b, double percent);
        void sample_pair(Parameter &param);
};

void Data::insert(int uu, int iii, int iij){
    if(data_length == nData){
        data_length *= 2;
        int*    new_uid =    (int*) malloc( data_length * sizeof(int) );
        int*    new_iidi =    (int*) malloc( data_length * sizeof(int) );
        int*    new_iidj =    (int*) malloc( data_length * sizeof(int) );
        for(int i=0;i<nData;i++){
            new_uid[i] = uid[i];
            new_iidi[i] = iidi[i];
            new_iidj[i] = iidj[i];
        }
        free(uid);
        free(iidi);
        free(iidj);
        uid = new_uid;
        iidi = new_iidi;
        iidj = new_iidj;
    }
    uid[nData] = uu;
    iidi[nData] = iii;
    iidj[nData] = iij;
    nData++;
}

void Data::load(char *filename, Parameter &param){
    FILE* fp = fopen(filename, "r");
    int uu, ii;
    double vv;
    int nU = 0, nI = 0;
    while(fscanf(fp,"%d %d",&uu,&ii)!=EOF){
        user_item.push_back( make_pair(uu,ii) );
        if(uu > nU) nU = uu;
        if(ii > nI) nI = ii;
    }
    fclose(fp);
    if( param.numU == -1 ) param.numU = nU + 1;
    if( param.numI == -1 ) param.numI = nI + 1;

    fprintf(stderr,"numU = %d, numI = %d\n",param.numU, param.numI);

    sort(user_item.begin(), user_item.end());


    // random shuffle
    int*    new_uid =    (int*) malloc( data_length * sizeof(int) );
    int*    new_iidi =    (int*) malloc( data_length * sizeof(int) );
    int*    new_iidj =    (int*) malloc( data_length * sizeof(int) );
    int*    new_idx =    (int*) malloc( nData * sizeof(int) );
    for(int i=0;i<nData;i++) new_idx[i] = i;
    random_shuffle(new_idx, new_idx + nData);
    for(int i=0;i<nData;i++){
        new_uid[i] = uid[ new_idx[i] ];
        new_iidi[i] = iidi[ new_idx[i] ];
        new_iidj[i] = iidj[ new_idx[i] ];
    }
    free(new_idx);
    free(uid);
    free(iidi);
    free(iidj);
    uid = new_uid;
    iidi = new_iidi;    
    iidj = new_iidj;
}



void Data::sample_pair(Parameter &param){

    int Size = user_item.size(), Nxt = 0;
    nData = 0;
    for(int i = 0;i < Size;i = Nxt){
        Nxt = i;
        while(Nxt<Size && user_item[Nxt].first ==user_item[i].first ) Nxt++;
        set<int> pos;
        int user = user_item[i].first;
        for(int j=i;j<Nxt;j++) pos.insert( user_item[j].second );
        for(int j=i;j<Nxt;j++){

            int now_cnt = param.nSample;
            while(now_cnt){
                int ng = rand()%param.numI;
                if( pos.find( ng ) == pos.end() ){
                    insert(user, user_item[j].second, ng );
                    now_cnt--;
                }
            }
        }
    }

}




void Data::split(Data &data_a, Data &data_b, double percent){
    assert(percent<=1.0);
    random_shuffle(user_item.begin(), user_item.end());
    int Size = user_item.size();
    int L = (int)( ((double) Size) * percent );
    int w = 0;
    data_a.user_item.clear();
    data_b.user_item.clear();
    for(;w<L;w++){
        data_a.user_item.push_back( user_item[w] );
    }
    for(;w<Size;w++){
        data_b.user_item.push_back( user_item[w] );
    }
    sort(user_item.begin(), user_item.end());
    sort(data_a.user_item.begin(), data_a.user_item.end());
    sort(data_b.user_item.begin(), data_b.user_item.end());

}

/*
void Data::split(Data &data_a, Data &data_b, double percent){
    assert(percent<=1.0);
    
    int L = (int)( ((double)nData) * percent );
    int w = 0;
    for(;w<L;w++){
        data_a.insert( uid[w], iidi[w], iidj[w]);
    }
    for(;w<nData;w++){
        data_b.insert( uid[w], iidi[w], iidj[w]);
    }

}
*/
#endif


