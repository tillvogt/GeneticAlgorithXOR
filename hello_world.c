#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>

const int popSize = 2;
const int netSize = 5;

const int inputs[4][2] = {{1,1},{1,0},{0,1},{0,0}};
const int outputs[4][1] = {{0},{1},{1},{0}};

const int numberOfCases = 4;
const int numberOfInputs = 2;
const int numberOfOutputs = 1;
const float mutationRate = 0.0;


int heaviside(float n){ // >=0 -> 1; <0 -> 0
    if(n>=0) return 1;
    else return 0;
}

int randomInt(){ // generates seed with current time, returns randomly -1;0;1
    
    int num = (rand() % 3);

    if(num == 2){
        num = -1;
    };

    return num;
}

void randomMat(int *matrix){  //needs adress to first entry of the matrix. Then uses randomInt() to fill entries with -1;0;1
    for(int k = 0; k < popSize; k++){
        for(int j = numberOfInputs; j < netSize; j++){
            for(int i = 0; i < netSize-numberOfOutputs; i++){
                if(i == j){
                    *(matrix + i + netSize*j + netSize*netSize*k) = 0;}
                else{
                    *(matrix + i + netSize*j + netSize*netSize*k) = randomInt();}
            }
        }
    }
}

void printMat(int *matrix){     //needs adress to first entry of the matrix. Then prints out matrix
    for(int k = 0; k < popSize; k++){
        printf("\n \n");
        for(int j = 0; j < netSize; j++){
            printf("\n");
            for(int i = 0; i < netSize; i++){
                printf("%2i ", *(matrix + i + netSize*j + netSize*netSize*k));
            }
        }
    }
    printf("\n");
}

void printVec(int *vector){     //needs adress to first entry of the matrix. Then prints out matrix
    for(int k = 0; k < popSize; k++){
        printf("\n");
        for(int j = 0; j < netSize; j++){
            printf("%d ", *(vector + j + netSize*k));
            
        }
    }
    printf("\n");
}

void update(int *matrix, int *vector, int *buffer, int *counter){      //needs adress of matrix, vector, a memoryvector for saving results and a vector for saving the cycles without fixpoint
    for(int k = 0; k < popSize; k++){
        int preCounter = 0;
        for(int j = numberOfInputs; j < netSize; j++){
            int sum = 0;
            for(int i = 0; i < netSize; i++){
                sum += *(matrix + i + netSize*j + netSize*netSize*k) * *(vector + i + k*netSize);
            }

            *(buffer + j + k*netSize) = heaviside(sum);

            if(*(buffer + j + k*netSize) == *(vector + j + k*netSize)){
                preCounter += 1;
            }

            else continue;
        }
        if(preCounter < (netSize-2)){
            *(counter + k) += 1;
        }    
        else continue;
    }
    for(int i=0; i<popSize; i++){
        for(int j=numberOfInputs; j<netSize; j++){
            *(vector + j + i*netSize) = *(buffer + j + i*netSize);
        }
    }
    //memcpy(vector+numberOfInputs, buffer+numberOfInputs, 4*(netSize-numberOfInputs));
}

void setInput(int caseNumber, int *vector){         //needs number of io-Case and pointer to first entry of statevector. Sets the input Values to those above...     
    for(int j = 0; j<popSize; j++){
        for(int i = 0; i<numberOfInputs; i++){
            *(vector + i + j*netSize) = inputs[caseNumber][i]; 
        }
    }    
}

void checkOutput(int caseNumber, int *vector, int *counter){       //needs casenumber, pointer to statevector and pointer to vector for saving results
    for(int i = 0; i < popSize; i++){
        int buffer = 0;
        for(int j = 0; j < numberOfOutputs; j++){
            if(*(vector + (netSize-1-j) + i*popSize) == outputs[caseNumber][0]){
                buffer++;
            }
            else{
                continue;
            }
        }
        if(buffer == numberOfOutputs){
            *(counter + i) += 1;
        }
        else{
            continue;
        }   
    }
}

void enumArray(double *array){      //enumerates the array with the fitness values
    for(int i=0; i<popSize; i++){
        *(array+popSize+i) = i+1;
    }
}

double hiEntry(double *array){      //returns the highest entry of the array
    double temp = *array;
    double index = *(array+popSize);
    for(int i=1; i<popSize; i++){
        if(temp < *(array+i)){
            temp = *(array+i);
            index = *(array+popSize+i);
        }
    }
    return index;
}

void  mutation(int *array){
    int rdm = rand()%((netSize-numberOfOutputs)*(netSize-numberOfInputs)*popSize);
    int z = rdm/((netSize-numberOfInputs)*(netSize-numberOfOutputs));
    int y = (rdm-z*(netSize-numberOfInputs)*(netSize-numberOfOutputs))/(netSize-numberOfOutputs);
    int x = rdm%(netSize-numberOfOutputs);

    *(array+x+(y+numberOfInputs)*netSize+z*netSize*netSize) = randomInt();

}

void generation(int revolutions, int *matrix, double *fit){     //needs number of revolutions, pointer to matrix and pointer to fitness array. Then runs the whole thing.
    int bufferVec[netSize][popSize] = {0};
    int cycleCounter[popSize] = {0};
    int correctOutputs[popSize] = {0};

    for(int i = 0; i<numberOfCases; i++){
        int stateVec[netSize][popSize] = {[0 ... (netSize-1)][0 ... (popSize-1)] = 1};
        setInput(i, &stateVec[0][0]);
        for(int j = 0; j<revolutions; j++){
            update(matrix, &stateVec[0][0], &bufferVec[0][0], &cycleCounter[0]);
        }
        checkOutput(i, &stateVec[0][0], &correctOutputs[0]);
    }

    for(int n=0; n<popSize; n++){
        *(fit+n) = (double) correctOutputs[n] / (cycleCounter[n]+50);
        printf("NETWORK %2i \ncycles: %2i , correct Outputs: %i, fitness: %lf \n", n+1, cycleCounter[n], correctOutputs[n], *(fit+n));
    }

}

int main(){
    struct timespec startTime;
    clock_gettime(CLOCK_REALTIME, &startTime);
    startTime.tv_sec;
    startTime.tv_nsec;
    srand(time(NULL));
    
    &adjmat[0][0][0] = (*int)malloc(4*netSize*netSize*popSize);
    //int adjaMat[netSize][netSize][popSize] = {0};
    
    randomMat(&adjaMat[0][0][0]);
    for(int n=0; n<2; n++){
        double fitVec[popSize][2] = {0};
        enumArray(&fitVec[0][0]);
        
        for(int i=0; i<(int)(((double)netSize*netSize*popSize)*mutationRate); i++){
            mutation(&adjaMat[0][0][0]);
        }

        printMat(&adjaMat[0][0][0]);
        generation(10, &adjaMat[0][0][0], &fitVec[0][0]);

        double hi = hiEntry(&fitVec[0][0]);
        int hint = (int) hi;
        printf("\n\n%i \n", hint);

        for(int k; k<popSize; k++){
            for(int j=0; j<netSize; j++){
                for(int i=0; i<netSize; i++)
                    adjaMat[i][j][k] = adjaMat[i][j][hint];
            }
        }
       
    }

    struct timespec endTime;
    clock_gettime(CLOCK_REALTIME, &endTime);
    endTime.tv_sec;
    endTime.tv_nsec;  
    printf("%.3lf \n", (endTime.tv_sec-startTime.tv_sec)+(endTime.tv_nsec-startTime.tv_nsec)*1e-9);

    return 0;
};