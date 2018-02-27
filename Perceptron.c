#include <stdlib.h>
#include <stdio.h>
#include <math.h>
/*Display Letter and Retina*/
void display(int array[5][4]){
	int i,j;
	for(i=0;i<5; i++) {
	for(j=0;j<4;j++) {
		printf("%d ",array[i][j]);
	    }
	    printf("\n");
	}
}
void displayretina(float array[20]){
	int i;
	for(i=0;i<20; i++) {
		printf("%f ",array[i]);
	    }
	   printf("\n");
}
/*Decompose the letter to place it on the retina dimensions*/
void retinabuild(int array[5][4],float retina[20]){
	int i,j;
	for(i=0;i<5;i++)
		for(j=0;j<4;j++)
		retina[(4*i)+j]=array[i][j];
}
/*Randomize the weights at the beginning*/
void weightbuild(float weight[20]){
	int i;
	for(i=0;i<20;i++){
		weight[i]=((rand()*1.0)/RAND_MAX)/20.0;
	}
}
/*Learning*/
void learning(float yd,float yi,float mu,float retina[20],float weight[20]){
	int i;
	for(i=0;i<20;i++){
	weight[i]=weight[i]+mu*(yd-yi)*retina[i];
	}
}
/*Information propagation*/
float output(float retina[20],float weight[20],float theta){
	float y;
	float potential=0;
	int i;
		for(i=0;i<20;i++){
			float temp=weight[i]*retina[i];
			potential+=temp;
		}
	if(potential > theta)
		y=1;
	else
		y=0;
	return y;
}
/*Information Propagation Widrow Hoff*/
float outputWH(float retina[20],float weight[20]){
	float potential=0;
	int i;
		for(i=0;i<20;i++){
			float temp=weight[i]*retina[i];
			potential+=temp;
		}
	return potential;
}
/*Noise function that accept double flipping*/
void testing(float retina[20],float tester[20],int error){
	int i;
	int random=0;
	int temp=0;
	for(i=0;i<20;i++){
		tester[i]=retina[i];
	}

	for(i=0;i!=error;i++){
		while(temp==random){
			random=rand()%20;
		}
			if(tester[random]==1){
				tester[random]=0;
			}else{
				tester[random]=1;
			}
		temp=random;
	}
}
/*Noise function that doesn't accept double flipping*/
void testing2(float retina[20],float tester[20],int error){
	int i,n;
	int random[error];
	int temp=0;
	for(i=0;i<20;i++){
		tester[i]=retina[i];
	}
	for(i=0;i!=error;i++){
			temp=rand()%20;
	   for (n=0;n<error; n++){
		  if (random[n]==temp){
			temp=rand()%20;
			n=0;
		  }
	   }
		random[i]=temp;
	 }
	for(i=0;i<error;i++){
		temp=random[i];
			if(tester[temp]==1)
				tester[temp]=0;
			else
				tester[temp]=1;
	}
}
int main(){
	int a[5][4]={{1,1,1,1},{1,0,0,1},{1,0,0,1},{1,1,1,1},{1,0,0,1}};
	int c[5][4]={{1,1,1,1},{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,1,1,1}};
	float retinaa[20];
	float retinac[20];
	float retinat[20];
	float Wij[20];
	FILE *ofp;
	FILE *ofpc;
	FILE *ofpa;
	FILE *testrand;
	FILE *robustness;
	FILE *robustnessc;
	float theta=0.5;
	weightbuild(Wij);
	float ya=0.0;
	float yc=1.0;
	float yad=1.0;
	float ycd=0.0;
	float errora=1.0;
	float errorc=1.0;
	float mu=0.01;
	retinabuild(a,retinaa);
	retinabuild(c,retinac);
	/*Display Letters and Retina
	display(a);
	display(c);
	displayretina(retinaa);
	displayretina(retinac);
	Display Letters and Retina*/
	int iteration=0;
	/*Case Widrow Hoff
	ya=outputWH(retinaa,Wij);
	yc=outputWH(retinac,Wij);
	errora=abs(yad-ya)
	errorc=abs(ycd-yc)
	Case Widrow Hoff*/
	ya=output(retinaa,Wij,theta);
	yc=output(retinac,Wij,theta);
	/*error here*/
	ofp = fopen("errora1.dat", "w");
	ofpc = fopen("errorc1.dat", "w");
	ofpa=fopen("weight1.txt", "w");
	robustness=fopen("robustnessa1.dat", "w");
	robustnessc=fopen("robustnessc1.dat", "w");
int j,e;
/*Initial Weights Saved*/
		for (e=0; e<20;e++){
	fprintf(ofpa, "%f\n",Wij[e]);
	}
			fprintf(ofpa, "\n");
	/*Testing 2*/
	testrand=fopen("mod.txt", "w");
		testing2(retinaa,retinat,20);

	for(j=0;j<20;j++) {
		fprintf(testrand,"%f",retinat[j]);
	    }
	/*Learning (y then error then enter the cycle learn yd error)*/
	while(errora>0.01 || errorc>0.01){
			learning(yad,ya,mu,retinaa,Wij);
			learning(ycd,yc,mu,retinac,Wij);
		ya=output(retinaa,Wij,theta);
		yc=output(retinac,Wij,theta);
		/*Case Widrow Hoff
		ya=outputWH(retinaa,Wij);
		yc=outputWH(retinac,Wij);
		Case Widrow Hoff*/
		errora=(float)fabs(yad-ya);
		errorc=(float)fabs(ycd-yc);
		fprintf(ofp, "%d %f\n",iteration,errora);
		fprintf(ofpc, "%d %f\n",iteration,errorc);
		iteration++;
	}

	/*Final Value Weights*/
	for (e=0; e<20;e++){
	fprintf(ofpa, "%f\n",Wij[e]);
	}
	/*Robustness*/

	for(e=0; e<19;e++){
		int j;
		int sum=0;
		int sumc=0;
		for(j=0; j<100;j++){
			testing2(retinaa,retinat,e);
			ya=output(retinat,Wij,theta);
			/*Case Widrow Hoff
			ya=outputWH(retinat,Wij);
			if(ya>0.99){
			Case Widrow Hoff*/
			if(ya==yad){
				sum++;
			}
			testing2(retinac,retinat,e);
			yc=output(retinat,Wij,theta);
			/*Case Widrow Hoff
			yc=outputWH(retinat,Wij);
			if(yc<0.01){
			Case Widrow Hoff*/
			if(yc==ycd){
				sumc++;
			}
		}
		fprintf(robustnessc,"%d %d\n",e,sumc);
		fprintf(robustness,"%d	%d\n",e,sum);
	}
	fclose(testrand);
	fclose(robustness);
	fclose(robustnessc);
	fclose(ofpc);
	fclose(ofp);
	fclose(ofpa);
	return 0;
}



