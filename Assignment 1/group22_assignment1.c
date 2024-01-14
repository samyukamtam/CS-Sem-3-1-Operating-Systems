#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
int N;
int FLAG;
int cX[]={ 2, 1, -1, -2, -2, -1,  1,  2 };
int cY[]={1, 2, -2,  1, -1,  2, -2,  -1 };
pthread_mutex_t lock;
struct pair{
	int x;
	int y;
};
struct history{
	int i;
	int N;
	//bool visited[50*50];
	struct pair * path;
};

void print_path(struct pair * path,int k){
	pthread_mutex_lock(&lock);
	for(int i=0;i<k;i++)
	{
		printf("%d,%d|",path[i].x,path[i].y);
	}
	fflush(stdout);
	pthread_mutex_unlock(&lock);
}

int lookahead(int X,int Y,struct pair * path,int len){
	int ans=0;
	//printf("Looking for %d,%d \n",X,Y);
	for(int p=0;p<8;p++)
	{
		if(X+cX[p]>-1 && X+cX[p]<N && Y+cY[p]>-1 && Y+cY[p]<N)
		{
			int i=0;
			for(i=0;i<len;i++)
			{
				if(path[i].x==X && path[i].y==Y) break;
			}
			if(i==len) ans++;
		}
	}
	return ans;
}
void  * solver(void * args)
{
	struct history * curr = (struct history *) args;
	//print_path(curr->path,curr->i);
	int currX=curr->path[curr->i-1].x;
	int currY=curr->path[curr->i-1].y;
	pthread_mutex_lock(&lock);
	//printf("CURRENTLY AT %d,%d and step %d \n",currX,currY,curr->i);
	pthread_mutex_unlock(&lock);
	fflush(stdout);
	if(curr->i==(N*N)){
		FLAG=1;
		print_path(curr->path,N*N);
		fflush(stdout);
		free(curr->path);
		free(curr);
		pthread_exit(NULL);
	}
	if(FLAG)
	{
		free(curr->path);
		free(curr);
		pthread_exit(NULL);
	}
	bool breaker;
	int lowest[8];
	int num_threads=0;
	int min_lookahead=9;
	for(int p=0;p<8;p++)
	{	
		breaker=false;
		int X=currX+cX[p];
		int Y=currY+cY[p];
		if(X<N && X>-1 && Y>-1 && Y<N){
			for(int k = 0; k<=curr->i;k++ ){
				if(curr->path[k].x == X&& curr->path[k].y ==Y){
					breaker=true;
					break;
				}
			}
			if(breaker) continue;
			int temp=lookahead(X,Y,curr->path,curr->i);
			if(temp < min_lookahead) 
			{
				min_lookahead=temp;
				num_threads=0;
				lowest[num_threads]=p;
				num_threads++;
			}
			else if(temp == min_lookahead)
			{
				lowest[num_threads]=p;
				num_threads++;
			}
		}
	}
	////printf("Number of thread %d \n",num_threads);
	if(num_threads==1)
	{
		pthread_mutex_lock(&lock);
		//printf("SINGLE MODE \n");
		pthread_mutex_unlock(&lock);
		fflush(stdout);
		curr->i++;
		curr->path[curr->i-1].x=currX+cX[lowest[0]];
		curr->path[curr->i-1].y=currY+cY[lowest[0]];
		solver((void *)curr);
	}
	if(num_threads>1){
		pthread_mutex_lock(&lock);
		//printf("MULTI MODE\n");
		pthread_mutex_unlock(&lock);
		fflush(stdout);
		pthread_t ids[num_threads];
		for(int t=0;t<num_threads;t++)
		{
			struct history * temp =(struct history *)malloc(sizeof(struct history));
			temp->path=malloc(N*N*sizeof(struct pair));
			temp->i=(curr->i)+1;
			for(int i=0;i<N*N;i++)
			{
				temp->path[i]=curr->path[i];
			}
			temp->N=N;
			temp->path[temp->i-1].x=currX+cX[lowest[t]];
			temp->path[temp->i-1].y=currY+cY[lowest[t]];
			
			//printf("new state %d,%d %d \n",temp->path[temp->i-1].x,temp->path[temp->i-1].y,temp->i);
			fflush(stdout);
			pthread_create(&ids[t],NULL,solver,(void *)temp);
			while(pthread_join(ids[t],NULL)!=0){
			}
			//printf("%d",ids[t]);
		}
	}
	free(curr->path);
	free(curr);
	pthread_exit(NULL);
}
int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Usage: ./Knight.out grid_size StartX StartY");
		exit(-1);
	}
	N = atoi(argv[1]);
	int startX=atoi(argv[2]);
	int startY=atoi(argv[3]);
	
	pthread_mutex_init(&lock,NULL);
	
	/* Do your thing here */
	struct history *t=(struct history *)malloc(sizeof(struct history));
	t->path=malloc(N*N*sizeof(struct pair));
	t->path[0].x=startX;
	t->path[0].y=startY;
	t->i=1;
	for(int i=1;i<N*N;i++)
	{
		t->path[i].x=-1;
		t->path[i].y=-1;
	}
	solver((void *)t);
	if(FLAG==0)
	{
		printf("No Possible Tour");
	}
	pthread_mutex_destroy(&lock);
	return 0;
}
