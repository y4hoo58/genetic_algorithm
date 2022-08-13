#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;


class Helpers{
    public:
    float randGenerator(float _scaler = 1){
        float rNum = ((float) rand() / RAND_MAX);
        rNum = rNum *_scaler;
        return rNum;
    }
};

class Parent{
    public:
    float fitnessScore;
    float *genes;

    void initGenes(int _geneNum){
        genes = new float[_geneNum];
    }
};

class GeneticAlgorithm{
    public:
    
    int parentNum,epochNum,geneNum,eliteNum;
    
    float maxVal,mut_percent;

    bool eliteMutation;

    int *elites;
    int *normies;

    Helpers cfuncs;
    Parent *parents;

    GeneticAlgorithm(int _parentNum,int _geneNum,int _epochNum,int _eliteNum,float _maxVal,float _mut_percent,bool _eliteMutation){
        parentNum = _parentNum;
        geneNum = _geneNum;
        epochNum = _epochNum;
        eliteNum = _eliteNum;
        maxVal = _maxVal;
        mut_percent = _mut_percent;
        eliteMutation = _eliteMutation;
    }

    void initialization(){
        // Heappp
        parents = new Parent[parentNum];
        elites=new int[eliteNum];
        normies = new int[parentNum - eliteNum];

        // Initiates parents and genes
        initParents();
    }

    void initParents(){

        for(int i=0;i<parentNum;i++){
            Parent parr;
            parr.initGenes(geneNum);

            // Assign parent
            parents[i] = parr;

            // Assign random genes
            for(int j=0;j<geneNum;j++){
                float _random_gene = cfuncs.randGenerator(maxVal);
                parents[i].genes[j] = _random_gene;
            }
        }
    }


    // Todo : Change fitness score according to the problem
    float fitnessScore(Parent parent){
        // Personal fitness score
        float sum = 0;
        for(int i=0;i<geneNum;i++){
            sum = sum + parent.genes[i];
        }
        return sum;
    }
    void calcFitness(){
        // Calculates the fitness of each parents
        for(int i=0;i<parentNum;i++){
            float _fscore = fitnessScore(parents[i]);
            parents[i].fitnessScore = _fscore;
        }
    }

    int findFittest(){
        int _max  =0;
        int _maxIndex = 0;
        
        for(int i = 0;i<parentNum;i++){
            
            if(i==0){
                _max = parents[i].fitnessScore;
            }   

            if(parents[i].fitnessScore>_max){
                _max = parents[i].fitnessScore;
                _maxIndex = i;
            }
        }
        return _maxIndex;
    }

    void sortParents(){
        // Ranks elites and normies based on their fitness scores
        for(int i =0; i< parentNum;i++){
            
            int _maxIndex=findFittest();


            if(i < eliteNum-1){
                parents[_maxIndex].fitnessScore = 0;
                elites[i] = _maxIndex;
            }else{
                parents[_maxIndex].fitnessScore =0;
                normies[i-eliteNum]=_maxIndex;
            }
            
        }
    }


    void crossover(){
        // Injects elite genes to the normies.
        int counter = 0;
        float injection_threshold = 0.5;

        for(int i = 0 ; i< eliteNum;i++){
            int _index1 = elites[i]; 
            int _index2 = normies[i];
            
            for(int j = 0 ; j< geneNum;j++){
                float _rand = cfuncs.randGenerator();
                if(_rand>injection_threshold){
                    int _gene1 = parents[_index1].genes[j];

                    parents[_index2].genes[j] = _gene1;
                }
            }
        }
    }

    void mutate(int *pindexes,int _size){
        for(int i = 0; i< _size;i++){
            for(int j = 0; j< geneNum; j++){
                float _rand = cfuncs.randGenerator();
                if(_rand>mut_percent){
                    float _mut_gene = cfuncs.randGenerator(maxVal);
                    parents[pindexes[i]].genes[j] = _mut_gene;
                }
            }
        }
    }

    void mutation(){
        // Mutate the normies
        mutate(normies,parentNum-eliteNum);

        // Mutate the elites
        if(eliteMutation ==  true){
            mutate(elites,eliteNum);
        }
    }
    
    
    void test(){
        calcFitness();
        int _maxIndex = findFittest();
        float _fscore = fitnessScore(parents[_maxIndex]);
        cout<<_fscore<<endl;
    }

    void mainLoop(){
        initialization();
        for(int i = 0; i< epochNum;i++){
            calcFitness();
            sortParents();
            crossover();
            mutation();
            test();
        }
    }
};


int main(){

    int parent_num = 100;
    int gene_num = 3;
    int epoch_num =100;
    int elite_num = parent_num*0.5;
    int max_val = 10;
    float mut_percent = 0.5;
    
    bool elite_mutation = false;

    GeneticAlgorithm gA(parent_num,gene_num,epoch_num,elite_num,max_val,mut_percent, elite_mutation);
    gA.mainLoop();

    return 0;
}
