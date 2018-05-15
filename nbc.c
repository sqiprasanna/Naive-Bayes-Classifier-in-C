#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <string.h>

//********************************************
typedef float probability;
float Train_cnt(int train[][192], int label[], int class, int feature, int value, float prob_class[]);
int Test_cnt(int test[],float prob[][192][5], float prob_class[]);


//*********************************************
int main(){
	
	FILE *fp1 = fopen("pp_tra.dat", "r"); 
    
	int train[6670][192];
	int label[6670];
	float prob_class[10];

	int i,j,k,c;

	for(i=0;i<6670;i++){
		for(j=0;j<192;j++){
			fscanf(fp1,"%d",&train[i][j]);
		}
        fscanf(fp1,"%d",&label[i]);
	}

	//probability *prob = (probability*)malloc(10*192*5*sizeof(probability));
	//We have done like this to avoid multiple 2D matrices.
	float prob[10][192][5];

	for(i=0;i<10;i++){
		for(j=0;j<192;j++){
			for(k=0;k<5;k++){//*(prob+i*192*5+j*5+k)=Train_cnt(train,label,i,j,k,prob_class);
				prob[i][j][k]=Train_cnt(train,label,i,j,k,prob_class);
			}
		}
	}

	for(i=0;i<10;i++){
		printf("%d\n\n ",i);
		for(j=0;j<192;j++){
			printf("%d ",j);
			for(k=0;k<5;k++){
				printf("%f ",prob[i][j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}



	FILE *fp2 = fopen("pp_tes.dat", "r"); 
    
	int test[3333][192];
	int label_pred[3333];
	int label_test[3333];


	for(i=0;i<3333;i++){
		for(j=0;j<192;j++){
			fscanf(fp2,"%d",&test[i][j]);
		}
        fscanf(fp2,"%d",&label_test[i]);
	}

	for(i=0;i<3333;i++)
		label_pred[i] = Test_cnt(test[i],prob,prob_class);
	

    int cnt=0;
	for(i=0;i<3333;i++)
		if(label_test[i]==label_pred[i])
			cnt++;


	for(i=0;i<3333;i++)
		printf("Compare :- %d %d %d\n",i,label_pred[i],label_test[i]);
	

	printf("Accuracy :-  %lf\n",(cnt/3333.0)*100);

	
	return 0;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++
float Train_cnt(int train[][192], int label[], int class, int feature, int value, float prob_class[]){
	int i,j;
	float cnt=0.0,cnt1=0.0;
	for(i=0;i<6670;i++){
		if(label[i]==class){ 
			if(train[i][feature] == value){
				cnt++;
			}
			cnt1++;
		}
	}
	prob_class[class]=cnt1/6670.0;
	return cnt/cnt1;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++
int Test_cnt(int test[],float prob[][192][5], float prob_class[]){
	
	float max = -9999.0;
	float sum;
	int i,j,index=-1;


	for(i=0;i<10;i++){
		sum=0.0;
		for(j=0;j<192;j++){		//printf("%f \t",log(*(prob+i*192+j*5+test[j])));//printf("%d ",test[j]);
			sum = sum + log(prob[i][j][test[j]]);
		}
		//printf("\n\n");
		sum = sum + log(prob_class[i]);
		if(sum > max){
			max = sum;
			index = i;
		}
	}
    return index;
}