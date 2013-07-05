#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <signal.h>
#include <math.h>


#include "cs402.h"

#include "my402list.h"
pthread_mutex_t lock;
pthread_t tid1111111111,tid1111111112,tid1111111113;

//Copyright from William Ching Warmup2 slides////
struct sigaction act11;
void *handler11(), interrupt();
//Copyright from William Ching Warmup2 slides////

int numberoftokens=1;
double r1=1.5,r2=0;
long b1=10,b2=0;
int droppedTokens=0,droppedPackets=0;
int totalCountTokens=0;
int totalCountPackets=0;
int totalCountPackets1=0;
int condition=0;
int droppedTokens111=0,droppedPackets111=0;
int totalCountTokens111=0;
int totalCountPackets111=0;
double totalInterarrivalTime=0;
double totalServiceTime=0;
double totalQ1time=0;
double totalQ2time=0;
double totalstime=0;
double totalEmulationTime=0;
double systemtime=0;





typedef struct warmup2 {
   int currentPacketNumber;
   int totalPackets;
   long serviceTime;
   int noTokens;
   long interarrivalTime;
} warmup2;


double notFileServiceTime=0;
double startTime=0;
double endTime=0,difference=0,stored_value=0,previous_value=0;
struct timeval start;
int dig=0;


typedef struct packet
{
int currentPacketNumber;
float arrival_Time_Q1;
float ending_Time_Q1;
int no_tokens;
float NotFileservicetime;
float Fileservicetime;
float arrival_Time_BeforeQ1;
}pack;

typedef struct queue2
{
int currentPacketNumber;
float arrival_Time_Q1;
float ending_Time_Q1;
float arrival_Time_Q2;
float ending_Time_Q2;
int no_tokens;
float serviceTime;
float arrival_Time_BeforeQ1;
}queue111;

float packet_stay_q1=0;

int currentPacketNumber11=0;
float arrival_Time_Q111=0;
float ending_Time_Q111=0;
float arrival_Time_Q211=0;
float ending_Time_Q211=0;
int no_tokens11=0;
float servicetime11=0;

float rtoken=0;
pack packet11111;
pack *warm23;
My402List list,packetlist,queue_secondlist;
double lamda1=0.5,lamda2=0;
long p1=3,p2=0;
long num1=20,num2=0;
int file11111111=-1;
pthread_cond_t cond_var111;
float values1111=0;
void packetcheck()
{
int no_digit=0;
int no_34=0;
if(My402ListEmpty(&packetlist)==0)
	{
		
	
	//printf("\nPacket list not Empty:-%d",My402ListLength(&packetlist));
	My402ListElem *first=My402ListFirst(&packetlist);
	pack *packing=(pack*)(first->obj);
	if((packing->no_tokens)<numberoftokens)
		{
	//printf("packing Number of Tokens %d",(packing->no_tokens));
	//printf("numberoftokens %d",(numberoftokens));
	no_34=numberoftokens-1;
	
	no_34=no_34-(packing->no_tokens);
	int k=no_34;
	//printf("Currentnumberoftokens After Subtraction %d",(no_34));
	packet_stay_q1=packing->arrival_Time_Q1;
	currentPacketNumber11=packing->currentPacketNumber;
	arrival_Time_Q111=packing->arrival_Time_Q1;
	no_tokens11=(packing->no_tokens);
	
	
	
	numberoftokens=no_34+1;
	//printf("FinalTokens %d",(numberoftokens));
	
	 if((packing->Fileservicetime)!=0)
	   {
	   servicetime11=(packing->Fileservicetime);
	   }
	   if((packing->NotFileservicetime)!=0)
	   {
	   servicetime11=(packing->NotFileservicetime);
	   }
	   
	   My402ListUnlink(&packetlist,first);
	   gettimeofday(&start, NULL);  
	   endTime=start.tv_sec+(start.tv_usec/1000000.0);
	   
	   
	   
	   
       difference=(endTime-startTime)*1000;
	   ending_Time_Q111=difference;
	   
	   packet_stay_q1=(difference)-packet_stay_q1;
	   
	   dig = (int) difference;
	   no_digit=count_digitnumber(dig);


//printf("\n values in Token %d %f %f %f %f %d %f",currentPacketNumber11,arrival_Time_Q111,ending_Time_Q111,arrival_Time_Q211,ending_Time_Q211,no_tokens11,servicetime11);	   
	   //numberoftokens=numberoftokens-(packing->no_tokens);
        
        

	   totalQ1time+=packet_stay_q1;
	   if(no_digit==0)
	   printf("\n00000000%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==1)
	   printf("\n0000000%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==2)
	   printf("\n000000%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==3)
	   printf("\n00000%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==4)
	   printf("\n0000%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==5)
	   printf("\n000%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==6)
	   printf("\n00%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==7)
	   printf("\n0%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==8)
	   printf("\n%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   printf("\n                  token bucket now has %d token",k);
	   
	  // printf("\n values in Token %d %f %f %f %f %d %f",currentPacketNumber11,arrival_Time_Q111,ending_Time_Q111,arrival_Time_Q211,ending_Time_Q211,no_tokens11,servicetime11);	   
       //printf("packing->arrival_Time_BeforeQ1%f",(packing->arrival_Time_BeforeQ1));
	 

	   values1111=(packing->arrival_Time_BeforeQ1);
	    
	   addingQueue2(currentPacketNumber11,arrival_Time_Q111,ending_Time_Q111,arrival_Time_Q211,ending_Time_Q211,no_tokens11,servicetime11,(packing->arrival_Time_BeforeQ1));       
         
	   
	   
	   
	   
	   
	   }
    }
	   
	 
}

void addingQueue2(int cp,float atq1,float etq1,float atq2,float etq2,int nt,float st,float arrival_Time_BeforeQ)
{
int no_digit=0;
queue111 *war=(queue111*)malloc(sizeof(queue111));
if(war!=NULL)
{

war->currentPacketNumber=cp;
war->arrival_Time_Q1=atq1;
war->ending_Time_Q1=etq1;

war->ending_Time_Q2=etq2;
war->no_tokens=nt;
war->serviceTime=servicetime11;
//printf("arrival_Time_BeforeQ%f",arrival_Time_BeforeQ);
war->arrival_Time_BeforeQ1=values1111;
//printf("Values %f %d",war->arrival_Time_BeforeQ1,war->currentPacketNumber);

//printf("ServiceTimeEnteringQ2%f",servicetime11);
gettimeofday(&start, NULL);  
endTime=start.tv_sec+(start.tv_usec/1000000.0);
difference=endTime-startTime;
difference=difference*1000;
dig = (int) difference;
no_digit=count_digitnumber(dig);
war->arrival_Time_Q2=difference;
int empty111=My402ListEmpty(&queue_secondlist);
(void)My402ListAppend(&queue_secondlist, (void*)war);
//printf("p%d enters Q2 %f",difference);
      if(no_digit==0)
		 { 
		 printf("\n00000000%.3fms: p%d enters Q2",(difference),(war->currentPacketNumber));
		 }
	  if(no_digit==1)
		 { 
		 printf("\n0000000%.3fms: p%d enters Q2",(difference),(war->currentPacketNumber));
		 }
	  if(no_digit==2)
		 { 
		 printf("\n000000%.3fms: p%d enters Q2",(difference),(war->currentPacketNumber));
		 }
	  if(no_digit==3)
		 { 
		 printf("\n00000%.3fms: p%d enters Q2",(difference),(war->currentPacketNumber));
		 }
	  if(no_digit==4)
		 { 
		 printf("\n0000%.3fms: p%d enters Q2",(difference),(war->currentPacketNumber));
		 }
	  if(no_digit==5)
		 { 
		 printf("\n000%.3fms: p%d enters Q2",(difference),(war->currentPacketNumber));
		 }
	  if(no_digit==6)
		 { 
		 printf("\n00%.3fms:p%d enters Q2",(difference),(war->currentPacketNumber));
		 }
	  if(no_digit==7)
		 { 
		 printf("\n0%.3fms:p%d enters Q2",(difference),(war->currentPacketNumber));
		 }
	  if(no_digit==8)
		 { 
		 printf("\n%.3fms:p%d enters Q2",(difference),(war->currentPacketNumber));
		 }
//printf("\n 00000752.716ms: p1 enters Q2");`

     if(empty111==1)
	 {
     pthread_cond_broadcast(&cond_var111);
	 }
     //pthread_mutex_unlock(&mutex);
     

}


if(My402ListEmpty(&queue_secondlist)==1)// List is empty
 {
 
 }
if(My402ListEmpty(&queue_secondlist)==0)// list is not empty
 {
 
 }
//warm23=(pack*)malloc(sizeof(pack));
}

void display(My402List* list)
{
warmup2 *obj11111;
My402ListElem *check1,*start1;
struct timeval tim;  
double t1=0,t2=0,t3=0,t4=0;
float k=0;
long Store=0;
int store1=0;
int no_digit=0;
float prev=0,diff=0,diff1=0,diff2=0;
memset(&packetlist, 0, sizeof(My402List));
My402ListInit(&packetlist);
warm23=(pack*)malloc(sizeof(pack));
if( (list->anchor.next)!=NULL)
	{
check1=list->anchor.next;
start1=check1;
int count=0;
while (check1->next  != list)
        {
		warm23=(pack*)malloc(sizeof(pack));
		obj11111=(warmup2 *)(check1->obj);
		//printf("\n No of token it needs %d",(obj11111->noTokens));
        //printf("\n Bucket Depth %d",b2);
		
		
        Store=(obj11111->interarrivalTime)*1000;
		gettimeofday(&tim, NULL);  
		prev=k;
        t1=tim.tv_sec+(tim.tv_usec/1000000.0); 
		usleep(Store);
		gettimeofday(&tim, NULL);
        t2=tim.tv_sec+(tim.tv_usec/1000000.0); 
		k+=(t2-t1)*1000;
		if(count!=0)
		prev=(t2-t1)*1000;
		diff=k-prev;
		store1=(int)(k);
	 gettimeofday(&tim, NULL);
	 t3=tim.tv_sec+(tim.tv_usec/1000000.0); 
	 gettimeofday(&start, NULL);  
     endTime=start.tv_sec+(start.tv_usec/1000000.0);
	 difference=endTime-startTime;
	 difference=difference*1000;
	 dig = (int) difference;
	 no_digit=count_digitnumber(dig);
	 if((obj11111->noTokens)>b2)
		{
		
	 pthread_mutex_lock(&lock);
	 if(no_digit==0)
	  {
	 printf("\n00000000%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	 if(no_digit==1)
	  {
	 printf("\n0000000%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  if(no_digit==2)
	  {
	 printf("\n000000%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  if(no_digit==3)
	  {
	 printf("\n00000%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  if(no_digit==4)
	  {
	 printf("\n0000%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  if(no_digit==5)
	  {
	 printf("\n000%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  if(no_digit==6)
	  {
	 printf("\n00%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  if(no_digit==7)
	  {
	 printf("\n0%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  if(no_digit==8)
	  {
	 printf("\n%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  droppedPackets111++;
	  check1=check1->next;
	 pthread_mutex_unlock(&lock);
	 //h=h+1;
	 continue;
	 
	 
		}
		totalInterarrivalTime+=(difference-previous_value);
	 if(no_digit==0)
		 { pthread_mutex_lock(&lock);
		 printf("\n00000000%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==1)
		 { pthread_mutex_lock(&lock);
		 printf("\n0000000%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==2)
		 { pthread_mutex_lock(&lock);
		 printf("\n000000%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==3)
		 { pthread_mutex_lock(&lock);
		 printf("\n00000%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==4)
		 { pthread_mutex_lock(&lock);
		 printf("\n0000%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==5)
		 { pthread_mutex_lock(&lock);
		 printf("\n000%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==6)
		 { pthread_mutex_lock(&lock);
		 printf("\n00%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==7)
		 { pthread_mutex_lock(&lock);
		 printf("\n0%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==8)
		 {  pthread_mutex_lock(&lock);
		 printf("\n%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
	     pthread_mutex_unlock(&lock);
		 }
		 totalCountPackets111++;
		 
		 previous_value=difference;
	
	
	gettimeofday(&tim, NULL);
	t4=tim.tv_sec+(tim.tv_usec/1000000.0);
    diff1+=(t4-t3)*1000;
    diff2=diff1+k;
	
	
	
	
	if(warm23!=NULL)
	{
	warm23->arrival_Time_BeforeQ1=difference;
	warm23->currentPacketNumber=(obj11111->currentPacketNumber)-1;
	gettimeofday(&start, NULL);  
    endTime=start.tv_sec+(start.tv_usec/1000000.0);
    difference=(endTime-startTime)*1000;
	warm23->arrival_Time_Q1=(float)difference;
	warm23->ending_Time_Q1=0;
	warm23->no_tokens=(obj11111->noTokens);
	warm23->NotFileservicetime=0;
	warm23->Fileservicetime=(float)(obj11111->serviceTime);
	//printf("\n values %d %f %f %d",warm23->currentPacketNumber,warm23->arrival_Time_Q1,warm23->ending_Time_Q1,warm23->no_tokens);
	(void)My402ListAppend(&packetlist, (void*)warm23);
    }
	store1=(int)(diff2);
	dig = (int) difference;
	no_digit=count_digitnumber(dig);
	pthread_mutex_lock(&lock);
		 if(no_digit==0)
		 {//pthread_mutex_lock(&lock);
		 printf("\n00000000%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==1)
		 {//pthread_mutex_lock(&lock);
		 printf("\n0000000%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==2)
		 {//pthread_mutex_lock(&lock);
		 printf("\n000000%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==3)
		 {//pthread_mutex_lock(&lock);
		 printf("\n00000%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==4)
		 {//pthread_mutex_lock(&lock);
		 printf("\n0000%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==5)
		 {//pthread_mutex_lock(&lock);
		 printf("\n000%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==6)
		 {//pthread_mutex_lock(&lock);
		 printf("\n00%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==7)
		 {//pthread_mutex_lock(&lock);
		 printf("\n0%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==8)
		 {//pthread_mutex_lock(&lock);
		 printf("\n%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 
		
		packetcheck();
		pthread_mutex_unlock(&lock);
        check1 = check1->next;
		}
		
		obj11111=(warmup2 *)(check1->obj);
		//printf("\n No of token it needs %d",(obj11111->noTokens));
        //printf("\n Bucket Depth %d",b2);
		 Store=(obj11111->interarrivalTime)*1000;
		 gettimeofday(&tim, NULL); 
         prev=k;		 
         t1=tim.tv_sec+(tim.tv_usec/1000000.0); 
		 usleep(Store);
		 gettimeofday(&tim, NULL);
         t2=tim.tv_sec+(tim.tv_usec/1000000.0); 
		 k+=(t2-t1)*1000;
		 diff=k-prev;
	     store1=(int)(k);
	 gettimeofday(&tim, NULL);
	 t3=tim.tv_sec+(tim.tv_usec/1000000.0); 
	 gettimeofday(&start, NULL);  
     endTime=start.tv_sec+(start.tv_usec/1000000.0);
     difference=(endTime-startTime)*1000;
	 dig = (int) difference;
	 no_digit=count_digitnumber(dig);
	 
	 if((obj11111->noTokens)>b2)
		{
		
	 pthread_mutex_lock(&lock);
	 if(no_digit==0)
	  {
	 printf("\n00000000%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	 if(no_digit==1)
	  {
	 printf("\n0000000%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  if(no_digit==2)
	  {
	 printf("\n000000%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  if(no_digit==3)
	  {
	 printf("\n00000%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  if(no_digit==4)
	  {
	 printf("\n0000%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  if(no_digit==5)
	  {
	 printf("\n000%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  if(no_digit==6)
	  {
	 printf("\n00%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  if(no_digit==7)
	  {
	 printf("\n0%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  if(no_digit==8)
	  {
	 printf("\n%.3fms: packet p%d arrives, needs %d tokens, dropped",difference,(obj11111->currentPacketNumber)-1,(obj11111->noTokens));
	  }
	  //check1=check1->next;
	 pthread_mutex_unlock(&lock);
	 //h=h+1;
	droppedPackets111++;
	 
	 
		}
		
		
		else if ((obj11111->noTokens)<=b2)
		{
	 
	 
	 totalInterarrivalTime+=(difference-previous_value);
	 if(no_digit==0)
		  {
		 pthread_mutex_lock(&lock);
		 printf("\n00000000%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		  }
		 
		 if(no_digit==1)
		 {
		 pthread_mutex_lock(&lock);
		 printf("\n0000000%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==2)
		 {
		 pthread_mutex_lock(&lock);
		 printf("\n000000%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==3)
		 {
		 pthread_mutex_lock(&lock);
		 printf("\n00000%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==4)
		 {
		 pthread_mutex_lock(&lock);
		 printf("\n0000%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==5)
		 {
		 pthread_mutex_lock(&lock);
		 printf("\n000%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==6)
		 {
		 pthread_mutex_lock(&lock);
		 printf("\n00%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==7)
		 {
		 pthread_mutex_lock(&lock);
		 printf("\n0%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==8)
		 {
		 pthread_mutex_lock(&lock);
		 printf("\n%.3fms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms",(difference),(obj11111->currentPacketNumber)-1,(obj11111->noTokens),(difference-previous_value));
	     pthread_mutex_unlock(&lock);
		 
		
		 }
		 totalCountPackets111++;
	previous_value=difference;
	gettimeofday(&tim, NULL);
	t4=tim.tv_sec+(tim.tv_usec/1000000.0);
    diff1+=(k+(t4-t3)*1000);
	warm23=(pack*)malloc(sizeof(pack));
	if(warm23!=NULL)
	{
	warm23->arrival_Time_BeforeQ1=difference;
	
	warm23->currentPacketNumber=(obj11111->currentPacketNumber)-1;
	gettimeofday(&start, NULL);  
    endTime=start.tv_sec+(start.tv_usec/1000000.0);
    difference=(endTime-startTime)*1000;
	warm23->arrival_Time_Q1=(float)difference;
	warm23->ending_Time_Q1=0;
	warm23->no_tokens=(obj11111->noTokens);
	warm23->NotFileservicetime=0;
	warm23->Fileservicetime=(float)(obj11111->serviceTime);
	//printf("\n values %d %f %f %d",warm23->currentPacketNumber,warm23->arrival_Time_Q1,warm23->ending_Time_Q1,warm23->no_tokens);
	(void)My402ListAppend(&packetlist, (void*)warm23);
    }
	//display111111111111(&packetlist);
	store1=(int)(diff1);
	dig = (int) difference;
	//no_digit=count_digitnumber(store1);
	no_digit=count_digitnumber(dig);
	 pthread_mutex_lock(&lock);
		 if(no_digit==0)
		 {//pthread_mutex_lock(&lock);
		 printf("\n00000000%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		// pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==1)
		 {//pthread_mutex_lock(&lock);
		 printf("\n0000000%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==2)
		 {//pthread_mutex_lock(&lock);
		 printf("\n000000%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==3)
		 {//pthread_mutex_lock(&lock);
		 printf("\n00000%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==4)
		 {//pthread_mutex_lock(&lock);
		 printf("\n0000%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==5)
		 {//pthread_mutex_lock(&lock);
		 printf("\n000%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==6)
		 {//pthread_mutex_lock(&lock);
		 printf("\n00%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==7)
		 {//pthread_mutex_lock(&lock);
		 printf("\n0%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		 
		 if(no_digit==8)
		 {//pthread_mutex_lock(&lock);
		 printf("\n%.3fms: p%d enters Q1",(difference),(obj11111->currentPacketNumber)-1);
		 //pthread_mutex_unlock(&lock);
		 }
		
		 packetcheck();
		 
		 pthread_mutex_unlock(&lock);
	     
         
	}
		 
    }
	//////////////New logic added///////////////
	
	int totalpackets=totalCountPackets111+droppedPackets111;
	//printf("Arpit %d",totalpackets);
	if(totalpackets==num2)
	{
	pthread_exit(NULL);
	}
	
	//////////////New logic added///////////////
}






void display111111111111(My402List *packetlist1)
{
//My402List *packetlist1=&packetlist;
//printf("Displaybjknkndkncknckn");
My402ListElem *check2,*start2;
queue111 *obj222=(queue111 *)malloc(sizeof(queue111));
if( (packetlist1->anchor.next)!=NULL)
	{
check2=packetlist1->anchor.next;
start2=check2;
//int count1=0;


while (check2->next  != packetlist1)
      {
obj222=(queue111 *)(check2->obj);
//printf("\n %d %f %f %f %f %d %f",(obj222->currentPacketNumber),(obj222->arrival_Time_Q1),(obj222->ending_Time_Q1),(obj222->arrival_Time_Q2),(obj222->ending_Time_Q2),(obj222->no_tokens),(obj222->serviceTime));

check2 = check2->next;
      }
obj222=(queue111 *)(check2->obj);
//printf("\n %d %f %f %f %f %d %f",(obj222->currentPacketNumber),(obj222->arrival_Time_Q1),(obj222->ending_Time_Q1),(obj222->arrival_Time_Q2),(obj222->ending_Time_Q2),(obj222->no_tokens),(obj222->serviceTime));

    }
}



int count_digitnumber (int num111111) {

	 int count111 =0;
	 if (num111111 == 0) count111++;
	   while (num111111 !=0) {
	                   count111++;
	                   num111111/=10;
	}
	return count111;
	 
	}
	
	int dfggjkdwjkd=0;
	
void* doSomeThing1(void *arg)
{

double x=1/lamda2;

x=x*1000000;

struct timeval tim;  
double t1=0,t2=0,t3=0,t4=0;
float k=0;


int store1=0;
int no_digit=0;
float prev=0,diff=0,diff1=0,diff2=0;
int count=0;
int h=1;
if(file11111111==1)
 {
    memset(&packetlist, 0, sizeof(My402List));
    My402ListInit(&packetlist);
    warm23=(pack*)malloc(sizeof(pack)); 
	
	
	
 while(h<=num2)
 {
 
        
 
    warm23=(pack*)malloc(sizeof(pack));
    gettimeofday(&tim, NULL);  
		 prev=k;
         t1=tim.tv_sec+(tim.tv_usec/1000000.0); 
		 usleep(x);
		 gettimeofday(&tim, NULL);
         t2=tim.tv_sec+(tim.tv_usec/1000000.0); 
		 
		 k+=(t2-t1)*1000;
		 
		 if(count!=0)
		 prev=(t2-t1)*1000;
		 
		 diff=k-prev;
		 
		 
		 
	 store1=(int)(k);
	 //no_digit=count_digitnumber(store1);
	 
	 gettimeofday(&tim, NULL);
	 t3=tim.tv_sec+(tim.tv_usec/1000000.0);
	 
	 gettimeofday(&start, NULL);  
     endTime=start.tv_sec+(start.tv_usec/1000000.0);
     difference=(endTime-startTime)*1000;
	 dig = (int) difference;
	 no_digit=count_digitnumber(dig);
	 
	 float arpit=difference;
	/* printf("\n No of token it needs %d",p2);
     printf("\n Bucket Depth %d",b2);*/
	 
	 if(p2>b2)
	 {
	 pthread_mutex_lock(&lock);
	 if(dfggjkdwjkd==0)
	 {
	 previous_value=0;
	 dfggjkdwjkd++;
	 }
	 totalInterarrivalTime+=(difference-previous_value);
	 previous_value=difference;
	 if(no_digit==0)
	  {
	 printf("\n00000000%.3fms: packet p%d arrives, needs %ld tokens, dropped",difference,h,p2);
	  }
	 if(no_digit==1)
	  {
	 printf("\n0000000%.3fms: packet p%d arrives, needs %ld tokens, dropped",difference,h,p2);
	  }
	  if(no_digit==2)
	  {
	 printf("\n000000%.3fms: packet p%d arrives, needs %ld tokens, dropped",difference,h,p2);
	  }
	  if(no_digit==3)
	  {
	 printf("\n00000%.3fms: packet p%d arrives, needs %ld tokens, dropped",difference,h,p2);
	  }
	  if(no_digit==4)
	  {
	 printf("\n0000%.3fms: packet p%d arrives, needs %ld tokens, dropped",difference,h,p2);
	  }
	  if(no_digit==5)
	  {
	 printf("\n000%.3fms: packet p%d arrives, needs %ld tokens, dropped",difference,h,p2);
	  }
	  if(no_digit==6)
	  {
	 printf("\n00%.3fms: packet p%d arrives, needs %ld tokens, dropped",difference,h,p2);
	  }
	  if(no_digit==7)
	  {
	 printf("\n0%.3fms: packet p%d arrives, needs %ld tokens, dropped",difference,h,p2);
	  }
	  if(no_digit==8)
	  {
	 printf("\n%.3fms: packet p%d arrives, needs %ld tokens, dropped",difference,h,p2);
	  }
	 pthread_mutex_unlock(&lock);
	 droppedPackets111++;
	 h=h+1;
	 continue;
	 }
	 //pthread_mutex_lock(&lock);
	 totalInterarrivalTime+=(difference-previous_value);
		 if(no_digit==0)
		 {pthread_mutex_lock(&lock);
		 printf("\n00000000%.3fms: p%d arrives, needs %ld tokens, inter-arrival time = %.3fms",(difference),h,p2,(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==1)
		 {pthread_mutex_lock(&lock);
		 printf("\n0000000%.3fms: p%d arrives, needs %ld tokens, inter-arrival time = %.3fms",(difference),h,p2,(difference-previous_value));
		pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==2)
		 {pthread_mutex_lock(&lock);
		 printf("\n000000%.3fms: p%d arrives, needs %ld tokens, inter-arrival time = %.3fms",(difference),h,p2,(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==3)
		 {pthread_mutex_lock(&lock);
		 printf("\n00000%.3fms: p%d arrives, needs %ld tokens, inter-arrival time = %.3fms",(difference),h,p2,(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==4)
		 {pthread_mutex_lock(&lock);
		 printf("\n0000%.3fms: p%d arrives, needs %ld tokens, inter-arrival time = %.3fms",(difference),h,p2,(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==5)
		 {pthread_mutex_lock(&lock);
		 printf("\n000%.3fms: p%d arrives, needs %ld tokens, inter-arrival time = %.3fms",(difference),h,p2,(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==6)
		 {pthread_mutex_lock(&lock);
		 printf("\n00%.3fms: p%d arrives, needs %ld tokens, inter-arrival time = %.3fms",(difference),h,p2,(difference-previous_value));
		pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==7)
		 {pthread_mutex_lock(&lock);
		 printf("\n0%.3fms: p%d arrives, needs %ld tokens, inter-arrival time = %.3fms",(difference),h,p2,(difference-previous_value));
		 pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==8)
		 {pthread_mutex_lock(&lock);
		 printf("\n%.3fms: p%d arrives, needs %ld tokens, inter-arrival time = %.3fms",(difference),h,p2,(difference-previous_value));
	     pthread_mutex_unlock(&lock);
		 }
		 totalCountPackets111++;
	previous_value=arpit;
    
   gettimeofday(&tim, NULL);
   t4=tim.tv_sec+(tim.tv_usec/1000000.0);
   diff1+=(t4-t3)*1000;
   diff2=diff1+k;
	
	
	
	if(warm23!=NULL)
	{
	warm23->arrival_Time_BeforeQ1=arpit;

	//warm23->arrival_Time_BeforeQ1=difference;
	
	//printf("11111111111%f",warm23->arrival_Time_BeforeQ1);
	
	warm23->currentPacketNumber=h;
	gettimeofday(&start, NULL);  
    endTime=start.tv_sec+(start.tv_usec/1000000.0);
    difference=(endTime-startTime)*1000;
	
	warm23->arrival_Time_Q1=(float)difference;
	
	
	warm23->ending_Time_Q1=0;
	warm23->no_tokens=(p2);
	
	warm23->NotFileservicetime=(1/notFileServiceTime)*1000;
	warm23->Fileservicetime=0;
	
	
	//printf("\n values %d %f %f %d",warm23->currentPacketNumber,warm23->arrival_Time_Q1,warm23->ending_Time_Q1,warm23->no_tokens);
	
	
   
	
	

//(void)My402ListAppend(&packetlist, (void*)warm23);
    store1=(int)(diff2);
	//no_digit=count_digitnumber(store1);
	dig = (int) difference;
	no_digit=count_digitnumber(dig);
	pthread_mutex_lock(&lock);
		 if(no_digit==0)
		 {
		 //pthread_mutex_lock(&lock);
		 printf("\n00000000%.3fms: p%d enters Q1",(difference),h);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==1)
		 {
		 //pthread_mutex_lock(&lock);
		 printf("\n0000000%.3fms: p%d enters Q1",(difference),h);
		 //pthread_mutex_unlock(&lock);
		 }
		 if(no_digit==2)
		 {
		 //pthread_mutex_lock(&lock);
		 printf("\n000000%.3fms: p%d enters Q1",(difference),h);
		 //pthread_mutex_unlock(&lock);
		 }
		
		 if(no_digit==3)
		 {
		 //pthread_mutex_lock(&lock);
		 printf("\n00000%.3fms: p%d enters Q1",(difference),h);
		 //pthread_mutex_unlock(&lock);
		 }
		
		 if(no_digit==4)
		 {
		 //pthread_mutex_lock(&lock);
		 printf("\n0000%.3fms: p%d enters Q1",(difference),h);
		 //pthread_mutex_unlock(&lock);
		 }
		
		 if(no_digit==5)
		 {
		 //pthread_mutex_lock(&lock);
		 printf("\n000%.3fms: p%d enters Q1",(difference),h);
		 //pthread_mutex_unlock(&lock);
		 }
		
		 if(no_digit==6)
		 {
		 //pthread_mutex_lock(&lock);
		 printf("\n00%.3fms: p%d enters Q1",(difference),h);
		 //pthread_mutex_unlock(&lock);
		 }
		
		 if(no_digit==7)
		 {
		// pthread_mutex_lock(&lock);
		 printf("\n0%.3fms: p%d enters Q1",(difference),h);
		 //pthread_mutex_unlock(&lock);
		 }
		
		 
		 if(no_digit==8)
		 {
		 //pthread_mutex_lock(&lock);
		 printf("\n%.3fms: p%d enters Q1",(difference),h);
		 //pthread_mutex_unlock(&lock);
		 }
		 (void)My402ListAppend(&packetlist, (void*)warm23);
		 
		 //pthread_mutex_lock(&lock);
		 packetcheck();
		 pthread_mutex_unlock(&lock);
		 
		
		
	}	 
	
	
		
		 h++;
		 if(h>num2)
		 {
		 //printf("\n Killed Parent thread Before");
		 //pthread_exit(3);
		 //int x=pthread_kill(tid1111111112,0);
		 //printf("\n Killed Parent thread After%d",x);
		 }
  } 
  //display111111111111(&packetlist);
  
  
	
	
 }
 
 //////////////New logic added///////////////
	
 int totalpackets=totalCountPackets111+droppedPackets111;
	//printf("Arpit %d",totalpackets);
	if(totalpackets==num2)
	{
	pthread_exit(NULL);
	}
	
//////////////New logic added///////////////
	
 
 else
 {
 display(&list);
 
 
 }




}
/////////////////////// Server
void* server(void *arg)
{
double standarddeviation=0;
int no_digit=0;
double power1=1;
int cp=0;
float atq1=0;
float etq1=0;
float atq2=0;
float etq2=0;
int nt=0;
float st=0;
int yui222 = 0;
totalServiceTime=0;

pthread_mutex_lock(&lock);
pthread_cond_wait(&cond_var111,&lock);
pthread_mutex_unlock(&lock);

while (yui222 <= num2)
{

float dif2=0;
float arrivaltime=0;

if(My402ListLength(&queue_secondlist) != 0)
   {
My402ListElem *first=My402ListFirst(&queue_secondlist);
 
	 queue111 *queu=(queue111*)(first->obj);
	 
	 cp=queu->currentPacketNumber;
	 atq1=queu->arrival_Time_Q1 ;
	 etq1=queu->ending_Time_Q1;
	 atq2=queu->arrival_Time_Q2;
	 etq2=queu->ending_Time_Q2;
	 arrivaltime=queu->arrival_Time_BeforeQ1;
	 nt=queu->no_tokens;
	 st=queu->serviceTime;
	 //printf("Service Time%f",st);
	 My402ListUnlink(&queue_secondlist,first);
	 condition++;
	 gettimeofday(&start, NULL);  
	 endTime=start.tv_sec+(start.tv_usec/1000000.0);
     difference=(endTime-startTime)*1000;
	 dig = (int) difference;
	 no_digit=count_digitnumber(dig);
	 dif2=difference-atq2;
	 float ser=difference;
	 totalQ2time+=dif2;
		 if(no_digit==0)
		 {
		 printf("\n00000000%.3fms: p%d begin service as S, time in Q2 = %.3f",(difference),cp,dif2);
		 }
		 if(no_digit==1)
		 {
		 printf("\n0000000%.3fms: p%d begin service as S, time in Q2 = %.3f",(difference),cp,dif2);
		 }
		 if(no_digit==2)
		 {
		 printf("\n000000%.3fms: p%d begin service as S, time in Q2 = %.3f",(difference),cp,dif2);
		 }
		 if(no_digit==3)
		 {
		 printf("\n00000%.3fms: p%d begin service as S, time in Q2 = %.3f",(difference),cp,dif2);
		 }
		 if(no_digit==4)
		 {
		 printf("\n0000%.3fms: p%d begin service as S, time in Q2 = %.3f",(difference),cp,dif2);
		 
		 }
		 if(no_digit==5)
		 {
		 printf("\n000%.3fms: p%d begin service as S, time in Q2 = %.3f",(difference),cp,dif2);
		  
		 }
		 if(no_digit==6)
		 {
		 printf("\n00%.3fms: p%d begin service as S, time in Q2 = %.3f",(difference),cp,dif2);
		  
		 }
		 if(no_digit==7)
		 {
		 printf("\n0%.3fms: p%d begin service as S, time in Q2 = %.3f",(difference),cp,dif2);
		 
		 }
		 if(no_digit==8)
		 {
		 printf("\n%.3fms: p%d begin service as S, time in Q2 = %.3f",(difference),cp,dif2);
		 
		 }
//printf("\n 00003612.843ms: p1 departs from S, service time = 2860.127ms time in system = 3109.731ms");
         
		 
		 
		 pthread_mutex_unlock(&lock);
		 //printf("stbhbjndnj  %f",st);
         int x=(int)(st*1000);	
         //printf("Service1 Time %d %f",x,st);		 
		 usleep(x);
		 gettimeofday(&start, NULL);
		 endTime=start.tv_sec+(start.tv_usec/1000000.0);
         difference=(endTime-startTime)*1000;
		 //printf("Service2 Time %f",difference);
	     dig = (int) difference;
	     no_digit=count_digitnumber(dig);
		 
		 pthread_mutex_lock(&lock);
		 
		 //float st=1/st;
		 totalServiceTime+=(difference-ser);
		 //printf("%lf %f",totalServiceTime,(difference-ser));
		 double valbss=(difference-arrivaltime);
		 totalstime+=(valbss);
		 //printf("Square value%.3f;",(valbss)*(valbss));
		 systemtime+=((valbss)*(valbss));
		 //printf("SystemTime%.3f",systemtime);
		 
		 if(no_digit==0)
		 {
		 printf("\n00000000%.3fms: p%d departs from S, service time = %.3fms",(difference),cp,(difference-ser));
		 printf("\n                time in system = %f",(difference-arrivaltime));
		
		 }
		 if(no_digit==1)
		 {
		 printf("\n0000000%.3fms: p%d departs from S, service time = %.3fms",(difference),cp,(difference-ser));
		 printf("\n               time in system = %f",(difference-arrivaltime)); 
		 }
		 if(no_digit==2)
		 {
		 printf("\n000000%.3fms: p%d departs from S, service time = %.3fms",(difference),cp,(difference-ser));
		 printf("\n              time in system = %f",(difference-arrivaltime)); 
		 }
		 if(no_digit==3)
		 {
		printf("\n00000%.3fms: p%d departs from S, service time = %.3fms",(difference),cp,(difference-ser));
		 printf("\n            time in system = %f",(difference-arrivaltime)); 
		 }
		 if(no_digit==4)
		 {
		printf("\n0000%.3fms: p%d departs from S, service time = %.3fms",(difference),cp,(difference-ser));
		 printf("\n           time in system = %f",(difference-arrivaltime)); 
		 }
		 if(no_digit==5)
		 {
		 printf("\n000%.3fms: p%d departs from S, service time = %.3fms",(difference),cp,(difference-ser));
		 printf("\n           time in system = %f",(difference-arrivaltime)); 
		 }
		 if(no_digit==6)
		 {
		 printf("\n00%.3fms: p%d departs from S, service time = %.3fms",(difference),cp,(difference-ser));
		 printf("\n          time in system = %f",(difference-arrivaltime));
		  
		 }
		 if(no_digit==7)
		 {
		 printf("\n0%.3fms: p%d departs from S, service time = %.3fms",(difference),cp,(difference-ser));
		 printf("\n         time in system = %f",(difference-arrivaltime));
		  
		 }
		 if(no_digit==8)
		 {
		 printf("\n%.3fms: p%d departs from S, service time = %.3fms",(difference),cp,(difference-ser));
		 printf("\n        time in system = %f",(difference-arrivaltime));
		  
		 }
		 totalEmulationTime=difference;
         pthread_mutex_unlock(&lock);

	}
	else 
	{
pthread_mutex_lock(&lock);
//printf("Condition %d num2 %d",condition,num2);
if( ((condition==num2))||((condition+droppedPackets111)==num2))
	 {
 //printf("server11");
 printf("\n \nStatistics:");
 int totalpackets=totalCountPackets111+droppedPackets111;
 double packet_arrivalTime=(totalInterarrivalTime/totalpackets);
 packet_arrivalTime=(packet_arrivalTime/1000);
 
 double serviceTime=(totalServiceTime/totalpackets);
 serviceTime=(serviceTime/1000);
 
 double totalq1=(totalQ1time/totalEmulationTime);
 double totalq2=(totalQ2time/totalEmulationTime);
 double totals=(totalServiceTime/totalEmulationTime);
 double total=(totalstime/totalpackets);
 total=(total/1000);
 
 int totaltok=totalCountTokens111+droppedTokens111;
 
 //printf("%d %d",droppedTokens111,totaltok);
 float tokendropprobability=0;
 tokendropprobability=(float)(droppedTokens111/totaltok);
 
 
 int packtot=totalCountPackets111+droppedPackets111;
 float dp=droppedPackets111;
 float packet_drop=(float)(dp/packtot);
 
 
 standarddeviation=((systemtime/totalpackets)-((totalstime*totalstime)/(totalpackets*totalpackets)));
 //printf("%f",standarddeviation);
 
 printf("\n average packet inter-arrival time = %.6f",packet_arrivalTime);
 printf("\n average packet service time = %.6f",serviceTime);
 printf("\n \n");
 printf("\n average number of packets in Q1 = %.6f",totalq1);
 printf("\n average number of packets in Q2 = %.6f",totalq2);	
 printf("\n average number of packets at S = %.6f",totals);
 printf("\n \n");
 printf("\n average time a packet spent in system = %.6f",total);
standarddeviation = pow((double)(standarddeviation/1000000),(double) 0.5);
 
 printf("\n standard deviation for time spent in system = %.6f",standarddeviation);
 float droppedTokens1=droppedTokens111;
 
 printf("\n \n");
 printf("\n token drop probability = %.6f",(droppedTokens1/totaltok));// DONE
 printf("\n packet drop probability = %.6f",packet_drop);// DONE
 
 /*printf("\n \n");
 printf("\n totalInterarrivalTime %lf",totalInterarrivalTime);
 printf("\n totalServiceTime %lf",totalServiceTime);
 printf("\n totalQ1time %lf",totalQ1time);
 printf("\n totalQ2time %lf",totalQ2time);
 printf("\n totalstime %lf",totalstime);
 printf("\n totalEmulationTime %lf",totalEmulationTime);
 printf("\n Square of System Time=%.3f",systemtime);
 
 
 printf("\n Total No of packets %d %d",totalCountPackets111,droppedPackets111);
 printf("\n Total No of tokens %d %d",totalCountTokens111,droppedTokens111);*/
 
 
 exit(0);
     }   
          pthread_cond_wait(&cond_var111,&lock);
		  pthread_mutex_unlock(&lock);
     }
	 
	}
	
	

   }
 
int noofdjwjd=0;
void* doSomeThing(void *arg)
{

//int no_to=0,no_to1=0;    
struct timeval tim;  

	int h=1;
	
	double t1=0,t2=0;
	int store1=0;
	
	
	double r3=1/r2;
 	
// printf("\n\n00000000.000ms: emulation begins");
 r3=r3*1000000;
 int no_digit=0;
 //int numberoftokens=1;
 float k=0;
	while(1)
	{
	
	
	
	
  
  gettimeofday(&tim, NULL);  
  t1=tim.tv_sec+(tim.tv_usec/1000000.0); 
  
	usleep(r3);
	gettimeofday(&tim, NULL);  
 
    t2=tim.tv_sec+(tim.tv_usec/1000000.0); 
	
	 
	 
	 k+=(t2-t1)*1000;
	 store1=(int)(k);
	 gettimeofday(&start, NULL);  
     endTime=start.tv_sec+(start.tv_usec/1000000.0);
     difference=(endTime-startTime)*1000;
	 dig = (int) difference;
	 no_digit=count_digitnumber(dig);
	 
	 

if(numberoftokens<=b2)
{

//printf("token t%d %f",difference);

if(no_digit==0)
{
pthread_mutex_lock(&lock);	
printf("\n00000000%.3lfms: token t%d arrives, token bucket now has %d token",(difference),h,numberoftokens);
pthread_mutex_unlock(&lock);
}	
if(no_digit==1)
{
pthread_mutex_lock(&lock);	
printf("\n0000000%.3lfms: token t%d arrives, token bucket now has %d token",(difference),h,numberoftokens);	
pthread_mutex_unlock(&lock);
}

if(no_digit==2)
{
pthread_mutex_lock(&lock);	
printf("\n000000%.3lfms: token t%d arrives, token bucket now has %d token",(difference),h,numberoftokens);
pthread_mutex_unlock(&lock);	
}

if(no_digit==3)
{
pthread_mutex_lock(&lock);	
printf("\n00000%.3lfms: token t%d arrives, token bucket now has %d token",(difference),h,numberoftokens);	
pthread_mutex_unlock(&lock);
}

if(no_digit==4)
{
pthread_mutex_lock(&lock);	
printf("\n0000%.3lfms: token t%d arrives, token bucket now has %d token",(difference),h,numberoftokens);
pthread_mutex_unlock(&lock);	
}

if(no_digit==5)
{
pthread_mutex_lock(&lock);	
printf("\n000%.3lfms: token t%d arrives, token bucket now has %d token",(difference),h,numberoftokens);
pthread_mutex_unlock(&lock);
}	

if(no_digit==6)
{
pthread_mutex_lock(&lock);	
printf("\n00%.3lfms: token t%d arrives, token bucket now has %d token",(difference),h,numberoftokens);
pthread_mutex_unlock(&lock);
}	

if(no_digit==7)
{
pthread_mutex_lock(&lock);	
printf("\n0%.3lfms: token t%d arrives, token bucket now has %d token",(difference),h,numberoftokens);	
pthread_mutex_unlock(&lock);
}

if(no_digit==8)
{
pthread_mutex_lock(&lock);	
printf("\n%.3lfms: token t%d arrives, token bucket now has %d token",(difference),h,numberoftokens);
pthread_mutex_unlock(&lock);
}


totalCountTokens111++;
	
}
else
{

if(no_digit==0)
{
pthread_mutex_lock(&lock);
printf("\n00000000%.3lfms: token t%d arrives, dropped",(difference),h);	
pthread_mutex_unlock(&lock);
}
if(no_digit==1)
{pthread_mutex_lock(&lock);
printf("\n0000000%.3lfms: token t%d arrives, dropped",(difference),h);
pthread_mutex_unlock(&lock);
}

if(no_digit==2)
{pthread_mutex_lock(&lock);
printf("\n000000%.3lfms: token t%d arrives, dropped",(difference),h);
pthread_mutex_unlock(&lock);
}	

if(no_digit==3)
{pthread_mutex_lock(&lock);
printf("\n00000%.3lfms: token t%d arrives, dropped",(difference),h);
pthread_mutex_unlock(&lock);
}

if(no_digit==4)
{pthread_mutex_lock(&lock);
printf("\n0000%.3lfms: token t%d arrives, dropped",(difference),h);	
pthread_mutex_unlock(&lock);
}

if(no_digit==5)
{pthread_mutex_lock(&lock);
printf("\n000%.3lfms: token t%d arrives, dropped",(difference),h);
pthread_mutex_unlock(&lock);
}

if(no_digit==6)
{pthread_mutex_lock(&lock);
printf("\n00%.3lfms: token t%d arrives, dropped",(difference),h);
pthread_mutex_unlock(&lock);
}	

if(no_digit==7)
{pthread_mutex_lock(&lock);
printf("\n0%.3lfms: token t%d arrives, dropped",(difference),h);
pthread_mutex_unlock(&lock);
}

if(no_digit==8)
{pthread_mutex_lock(&lock);
printf("\n%.3lfms: token t%d arrives, dropped",(difference),h);
pthread_mutex_unlock(&lock);
}
totalEmulationTime=difference;

droppedTokens111++;
pthread_mutex_lock(&lock);
numberoftokens=numberoftokens-1;
pthread_mutex_unlock(&lock);

if( (droppedPackets111==num2))
	 {
	 pthread_mutex_lock(&lock);
	 
	 
 double standarddeviation=0;	 
 printf("\n \nStatistics:");
 int totalpackets=totalCountPackets111+droppedPackets111;
 double packet_arrivalTime=(totalInterarrivalTime/totalpackets);
 packet_arrivalTime=(packet_arrivalTime/1000);
 
 double serviceTime=(totalServiceTime/totalpackets);
 serviceTime=(serviceTime/1000);
 
 double totalq1=(totalQ1time/totalEmulationTime);
 double totalq2=(totalQ2time/totalEmulationTime);
 double totals=(totalServiceTime/totalEmulationTime);
 double total=(totalstime/totalpackets);
 total=(total/1000);
 
 int totaltok=totalCountTokens111+droppedTokens111;
 
 //printf("%d %d",droppedTokens111,totaltok);
 float tokendropprobability=0;
 tokendropprobability=(float)(droppedTokens111/totaltok);
 
 
 int packtot=totalCountPackets111+droppedPackets111;
 float dp=droppedPackets111;
 float packet_drop=(float)(dp/packtot);
 
 
 standarddeviation=((systemtime/totalpackets)-((totalstime*totalstime)/(totalpackets*totalpackets)));
 packet_arrivalTime=0;
 printf("\n average packet inter-arrival time = %.6f",packet_arrivalTime);
 printf("\n average packet service time = %.6f",serviceTime);
 printf("\n \n");
 printf("\n average number of packets in Q1 = %.6f",totalq1);
 printf("\n average number of packets in Q2 = %.6f",totalq2);	
 printf("\n average number of packets at S = %.6f",totals);
 printf("\n \n");
 printf("\n average time a packet spent in system = %.6f",total);
 standarddeviation = pow((double)(standarddeviation/1000000),(double) 0.5);
 
 printf("\n standard deviation for time spent in system = %.6f",standarddeviation);
 float droppedTokens1=droppedTokens111;
 
 printf("\n \n");
 printf("\n token drop probability = %.6f",(droppedTokens1/totaltok));// DONE
 printf("\n packet drop probability = %.6f",packet_drop);// DONE
 
 /*printf("\n \n");
 printf("\n totalInterarrivalTime %lf",totalInterarrivalTime);
 printf("\n totalServiceTime %lf",totalServiceTime);
 printf("\n totalQ1time %lf",totalQ1time);
 printf("\n totalQ2time %lf",totalQ2time);
 printf("\n totalstime %lf",totalstime);
 printf("\n totalEmulationTime %lf",totalEmulationTime);
 printf("\n Square of System Time=%.3f",systemtime);
 
 
 printf("\n Total No of packets %d %d",totalCountPackets111,droppedPackets111);
 printf("\n Total No of tokens %d %d",totalCountTokens111,droppedTokens111);*/
 
 /*printf("server");
 printf("\nStatistics:");
 printf("\naverage packet inter-arrival time = <real-value>");
 printf("\naverage packet service time = <real-value>");
 printf("\n111 111= <real-value>");
 printf("\naverage number of packets in Q2 = <real-value>");
 printf("\naverage number of packets at S = <real-value>");
 printf("\naverage time a packet spent in system = <real-value>");
 printf("\nstandard deviation for time spent in system = <real-value>");
 printf("\ntoken drop probability = <real-value>");
 printf("\npacket drop probability = <real-value>");
 printf("\n totalInterarrivalTime %lf",totalInterarrivalTime);
 printf("\n totalServiceTime %lf",totalServiceTime);
 
 printf("\n totalQ1time %lf",totalQ1time);
 printf("\n totalQ2time %lf",totalQ2time);
 printf("\n totalstime %lf",totalstime);
 printf("\n totalEmulationTime %lf",totalEmulationTime);
 
 printf("\n Total No of packets %d %d",totalCountPackets111,droppedPackets111);
 printf("\n Total No of tokens %d %d",totalCountTokens111,droppedTokens111);*/
     pthread_mutex_unlock(&lock);
 exit(0);
    
     }
}





if(My402ListEmpty(&packetlist)==0)
	{
		
	
	//printf("\nPacket list not Empty:-%d",My402ListLength(&packetlist));
	My402ListElem *first=My402ListFirst(&packetlist);
	pack *packing=(pack*)(first->obj);
	//printf("packing Number of Tokens %d",(packing->no_tokens));
	if((packing->no_tokens)<=numberoftokens)
	   {
	   pthread_mutex_lock(&lock);
	   //printf("Before Number of Tokens %d %d",numberoftokens,packing->no_tokens);
	   numberoftokens=numberoftokens-(packing->no_tokens);
	   packet_stay_q1=packing->arrival_Time_Q1;
	   
	   
	   
	   //////////////////////////////////Q2 Logic
	   
	   currentPacketNumber11=packing->currentPacketNumber;
	   arrival_Time_Q111=packing->arrival_Time_Q1;
	   no_tokens11=(packing->no_tokens);
	   
	   if((packing->Fileservicetime)!=0)
	   {
	   servicetime11=(packing->Fileservicetime);
	   }
	   if((packing->NotFileservicetime)!=0)
	   {
	   servicetime11=(packing->NotFileservicetime);
	   }
	   
	   //////////////////////////////////Q2 Logic
	   
	    //printf("After Number of Tokens %d %d",numberoftokens,packing->no_tokens);
	   My402ListUnlink(&packetlist,first);
	   gettimeofday(&start, NULL);  
	   endTime=start.tv_sec+(start.tv_usec/1000000.0);
	   
	   
	   
	   
       difference=(endTime-startTime)*1000;
	   ending_Time_Q111=difference;
	   
	   packet_stay_q1=(difference)-packet_stay_q1;
	   
	   dig = (int) difference;
	   no_digit=count_digitnumber(dig);


//printf("\n values in Token %d %f %f %f %f %d %f",currentPacketNumber11,arrival_Time_Q111,ending_Time_Q111,arrival_Time_Q211,ending_Time_Q211,no_tokens11,servicetime11);	   
	   //numberoftokens=numberoftokens-(packing->no_tokens);
        
        

	   totalQ1time+=packet_stay_q1;
	   if(no_digit==0)
	   printf("\n00000000%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==1)
	   printf("\n0000000%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==2)
	   printf("\n000000%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==3)
	   printf("\n00000%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==4)
	   printf("\n0000%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==5)
	   printf("\n000%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==6)
	   printf("\n00%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==7)
	   printf("\n0%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   if(no_digit==8)
	   printf("\n%.3lfms: p%d leaves Q1, time in Q1 = %.3lfms,",difference,(packing->currentPacketNumber),packet_stay_q1);
	   
	   printf("\n                  token bucket now has %d token",numberoftokens);
	   values1111=(packing->arrival_Time_BeforeQ1);
	   addingQueue2(currentPacketNumber11,arrival_Time_Q111,ending_Time_Q111,arrival_Time_Q211,ending_Time_Q211,no_tokens11,servicetime11,(packing->arrival_Time_BeforeQ1));       
      
	   pthread_mutex_unlock(&lock);
	   
	   
	   
	   }
	
	
	}  
	
	
	
	pthread_mutex_lock(&lock);
	h++;
	noofdjwjd=h;
	pthread_mutex_unlock(&lock);
	
	
 
	
	if(numberoftokens<=b2){pthread_mutex_lock(&lock);
	numberoftokens++;
pthread_mutex_unlock(&lock);}

/////////////////////////////////New Logic added/////////////

if( (droppedPackets111==num2))
	 {
	 pthread_mutex_lock(&lock);
	 
	 
 double standarddeviation=0;	 
 printf("\n \nStatistics:");
 int totalpackets=totalCountPackets111+droppedPackets111;
 double packet_arrivalTime=(totalInterarrivalTime/totalpackets);
 packet_arrivalTime=(packet_arrivalTime/1000);
 
 double serviceTime=(totalServiceTime/totalpackets);
 serviceTime=(serviceTime/1000);
 
 double totalq1=(totalQ1time/totalEmulationTime);
 double totalq2=(totalQ2time/totalEmulationTime);
 double totals=(totalServiceTime/totalEmulationTime);
 double total=(totalstime/totalpackets);
 total=(total/1000);
 
 int totaltok=totalCountTokens111+droppedTokens111;
 
 //printf("%d %d",droppedTokens111,totaltok);
 float tokendropprobability=0;
 tokendropprobability=(float)(droppedTokens111/totaltok);
 
 
 int packtot=totalCountPackets111+droppedPackets111;
 float dp=droppedPackets111;
 float packet_drop=(float)(dp/packtot);
 
 
 standarddeviation=((systemtime/totalpackets)-((totalstime*totalstime)/(totalpackets*totalpackets)));
 packet_arrivalTime=0;
 printf("\n average packet inter-arrival time = %.6f",packet_arrivalTime);
 printf("\n average packet service time = %.6f",serviceTime);
 printf("\n \n");
 totalq1=0;
 printf("\n average number of packets in Q1 = %.6f",totalq1);
 totalq2=0;
 printf("\n average number of packets in Q2 = %.6f",totalq2);	
 totals=0;
 printf("\n average number of packets at S = %.6f",totals);
 printf("\n \n");
 printf("\n average time a packet spent in system = %.6f",total);
 standarddeviation = pow((double)(standarddeviation/1000000),(double) 0.5);
 
 printf("\n standard deviation for time spent in system = %.6f",standarddeviation);
 float droppedTokens1=droppedTokens111;
 
 printf("\n \n");
 printf("\n token drop probability = %.6f",(droppedTokens1/totaltok));// DONE
 printf("\n packet drop probability = %.6f",packet_drop);// DONE
 
 /*printf("\n \n");
 printf("\n totalInterarrivalTime %lf",totalInterarrivalTime);
 printf("\n totalServiceTime %lf",totalServiceTime);
 printf("\n totalQ1time %lf",totalQ1time);
 printf("\n totalQ2time %lf",totalQ2time);
 printf("\n totalstime %lf",totalstime);
 printf("\n totalEmulationTime %lf",totalEmulationTime);
 printf("\n Square of System Time=%.3f",systemtime);
 
 
 printf("\n Total No of packets %d %d",totalCountPackets111,droppedPackets111);
 printf("\n Total No of tokens %d %d",totalCountTokens111,droppedTokens111);*/
 
 /*printf("server");
 printf("\nStatistics:");
 printf("\naverage packet inter-arrival time = <real-value>");
 printf("\naverage packet service time = <real-value>");
 printf("\n111 111= <real-value>");
 printf("\naverage number of packets in Q2 = <real-value>");
 printf("\naverage number of packets at S = <real-value>");
 printf("\naverage time a packet spent in system = <real-value>");
 printf("\nstandard deviation for time spent in system = <real-value>");
 printf("\ntoken drop probability = <real-value>");
 printf("\npacket drop probability = <real-value>");
 printf("\n totalInterarrivalTime %lf",totalInterarrivalTime);
 printf("\n totalServiceTime %lf",totalServiceTime);
 
 printf("\n totalQ1time %lf",totalQ1time);
 printf("\n totalQ2time %lf",totalQ2time);
 printf("\n totalstime %lf",totalstime);
 printf("\n totalEmulationTime %lf",totalEmulationTime);
 
 printf("\n Total No of packets %d %d",totalCountPackets111,droppedPackets111);
 printf("\n Total No of tokens %d %d",totalCountTokens111,droppedTokens111);*/
     pthread_mutex_unlock(&lock);
 exit(0);
    
     }

 if( (My402ListEmpty(&packetlist)==1)&& ((totalCountPackets111+droppedPackets111)==num2))
	{
	//printf("\ngggghjhjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj%d",(totalCountPackets111+droppedPackets111));
	pthread_exit(NULL);
	}
	}
	
	
	
/////////////////////////////////New Logic added/////////////
	
	
	
	
	

   
}

///Copywright from William Ching Slide////////
pthread_t user_threadID;
sigset_t new;
int count111=0;
void * handler11(char argv1[])
{

//printf("\n Press CTRL-C to deliver SIGINT\n");

//printf("%s",SIGINT);
while( count111 == 0)
{
act11.sa_handler = interrupt;
sigaction(SIGINT, &act11, NULL);

pthread_sigmask(SIG_UNBLOCK, &new, NULL);
}

if (count111 != 0)
{
pthread_kill(tid1111111111,SIGUSR1);
pthread_kill(tid1111111112,SIGUSR1);
pthread_kill(tid1111111113,SIGUSR1);
}

}

void interrupt(int sig)
{
 //printf("thread %d caught signal %d\n", thr_self(), sig);
 pthread_mutex_lock(&lock);
 //pthread_mutex_lock(&lock);
	 
	 
 double standarddeviation=0;	 
 printf("\n \nStatistics:");
 int totalpackets=totalCountPackets111+droppedPackets111;
 double packet_arrivalTime=(totalInterarrivalTime/totalpackets);
 packet_arrivalTime=(packet_arrivalTime/1000);
 
 double serviceTime=(totalServiceTime/totalpackets);
 serviceTime=(serviceTime/1000);
 
 double totalq1=(totalQ1time/totalEmulationTime);
 double totalq2=(totalQ2time/totalEmulationTime);
 double totals=(totalServiceTime/totalEmulationTime);
 double total=(totalstime/totalpackets);
 total=(total/1000);
 
 int totaltok=totalCountTokens111+droppedTokens111;
 
 //printf("%d %d",droppedTokens111,totaltok);
 float tokendropprobability=0;
 tokendropprobability=(float)(droppedTokens111/totaltok);
 
 
 int packtot=totalCountPackets111+droppedPackets111;
 float dp=droppedPackets111;
 float packet_drop=(float)(dp/packtot);
 
 
 standarddeviation=((systemtime/totalpackets)-((totalstime*totalstime)/(totalpackets*totalpackets)));
 packet_arrivalTime=0;
 printf("\n average packet inter-arrival time = %.6f",packet_arrivalTime);
 printf("\n average packet service time = %.6f",serviceTime);
 printf("\n \n");
 printf("\n average number of packets in Q1 = %.6f",totalq1);
 printf("\n average number of packets in Q2 = %.6f",totalq2);	
 printf("\n average number of packets at S = %.6f",totals);
 printf("\n \n");
 printf("\n average time a packet spent in system = %.6f",total);
 
standarddeviation = pow((double)(standarddeviation/1000000),(double) 0.5);
  
 printf("\n standard deviation for time spent in system = %.6f",standarddeviation);
 float droppedTokens1=droppedTokens111;
 
 printf("\n \n");
 printf("\n token drop probability = %.6f",(droppedTokens1/totaltok));// DONE
 printf("\n packet drop probability = %.6f",packet_drop);// DONE
 
/* printf("\n \n");
 printf("\n totalInterarrivalTime %lf",totalInterarrivalTime);
 printf("\n totalServiceTime %lf",totalServiceTime);
 printf("\n totalQ1time %lf",totalQ1time);
 printf("\n totalQ2time %lf",totalQ2time);
 printf("\n totalstime %lf",totalstime);
 printf("\n totalEmulationTime %lf",totalEmulationTime);
 printf("\n Square of System Time=%.3f",systemtime);
 
 
 printf("\n Total No of packets %d %d",totalCountPackets111,droppedPackets111);
 printf("\n Total No of tokens %d %d",totalCountTokens111,droppedTokens111);*/
 
 /*printf("server");
 printf("\nStatistics:");
 printf("\naverage packet inter-arrival time = <real-value>");
 printf("\naverage packet service time = <real-value>");
 printf("\n111 111= <real-value>");
 printf("\naverage number of packets in Q2 = <real-value>");
 printf("\naverage number of packets at S = <real-value>");
 printf("\naverage time a packet spent in system = <real-value>");
 printf("\nstandard deviation for time spent in system = <real-value>");
 printf("\ntoken drop probability = <real-value>");
 printf("\npacket drop probability = <real-value>");
 printf("\n totalInterarrivalTime %lf",totalInterarrivalTime);
 printf("\n totalServiceTime %lf",totalServiceTime);
 
 printf("\n totalQ1time %lf",totalQ1time);
 printf("\n totalQ2time %lf",totalQ2time);
 printf("\n totalstime %lf",totalstime);
 printf("\n totalEmulationTime %lf",totalEmulationTime);
 
 printf("\n Total No of packets %d %d",totalCountPackets111,droppedPackets111);
 printf("\n Total No of tokens %d %d",totalCountTokens111,droppedTokens111);*/
 pthread_mutex_unlock(&lock);
 exit(0);
count111++;
}
///Copywright from William Ching Slide////////

int main(int argc, char *argv[])
{
FILE *file;
int filecounter=0;





double mu1=0.35,mu2=0;



int i=0;



long serviceTm=0,interarrivalTm=0;
int totalPac=0;
int noToken=0;
int count=0;
char buf[1024];
char file222[100];
char *buf1;
int count2=1;



memset(&list, 0, sizeof(My402List));
My402ListInit(&list);


memset(&queue_secondlist, 0, sizeof(My402List));
My402ListInit(&queue_secondlist);





while(i<argc)
{
if(strcmp(argv[i],"-lambda")==0)
 {
 lamda2=atof(argv[i+1]);
 }
 
if(strcmp(argv[i],"-mu")==0)
 {
 mu2=atof(argv[i+1]);
 }
 
if(strcmp(argv[i],"-r")==0)
 {
 r2=atof(argv[i+1]);
 }
 
if(strcmp(argv[i],"-B")==0)
 {
 b2=atoi(argv[i+1]);
 }
 
if(strcmp(argv[i],"-P")==0)
 {
 p2=atoi(argv[i+1]);
 }
 
if(strcmp(argv[i],"-n")==0)
 {
 num2=atoi(argv[i+1]);
 }
 
if(strcmp(argv[i],"-t")==0)
 {
 
 strcpy(file222,argv[i+1]);
 
 filecounter=i+1;
 
 
 }
 
i++;
}

if(lamda2==0)
{
lamda2=lamda1;
}
if(mu2==0)
{
mu2=mu1;
}
if(r2==0)
{
r2=r1;
}
if(b2==0)
{
b2=b1;
}
if(p2==0)
{
p2=p1;
}
if(num2==0)
{
num2=num1;
}

float r3=1/r2;
if(r3>10)
 {
 r2=.1;
 }

 float lamda3=1/lamda2;
 if(lamda3>10)
 {
 lamda2=.1;
 }

 float service3=1/mu2;
 if(service3>10)
 {
 mu2=.1;
 }
 
 float f1=0;
 f1=(float)(num2);
 f1=f1*-1;
 printf("dhudhuijdijdji%f",f1);
 //exit(0);
 if(f1>2147483647.00)
 {
 printf("\n Num2 value is greater than 2147483647 %f",f1);
 exit(0);
 }
 f1=(float)(b2);	
 f1=f1*-1;
 if(f1>2147483647.00)
 {
 printf("\n b2 value is greater than 2147483647 %f",f1);
 exit(0);
 }
 f1=(float)(p2);
 f1=f1*-1;;
 if(f1>2147483647.00)
 {
 printf("\n p2 value is greater than 2147483647 %f",f1);
 exit(0);
 }
 





if(filecounter==0)
{
printf("\n Emulation Parameters:");
printf("\n     lambda = %.2lf",lamda2);
printf("\n     mu = %.2lf",mu2);
printf("\n     r = %.2lf",r2);
printf("\n     B = %ld",b2);
printf("\n     p = %ld",p2);
printf("\n     number to arrive =%ld",num2);
totalCountPackets=num2;
notFileServiceTime=mu2;
file11111111=1;
}







if(filecounter!=0)
{
file11111111=0;
printf("\n Emulation Parameters:");
printf("\n     r = %.2lf",r2);
printf("\n     B = %ld",b2);


printf("\ntsfile = %s",file222);
file = fopen( file222, "r" );
if(file==0)
 {
 printf( "\n Could not open file\n" );
 exit(1);
 }
 else
 {
 
 while (fgets(buf, sizeof(buf), file) != NULL)
  {
  buf1=buf;
  while(*buf1!='\n')
  {
   if((*buf1)==' ')
   {
   (*buf1)='\t';
   }
   buf1++;
  }
  
  warmup2 *warm=(warmup2*)malloc(sizeof(warmup2));
  
  if(count==0)
   {
   
   sscanf(buf,"%d",&totalPac);
   
   
   warm->currentPacketNumber=count2;
   }
   else
   {
   sscanf(buf,"%ld \t %d \t %ld",&interarrivalTm,&noToken,&serviceTm);
  
   
   if(warm!=NULL)
    {
	
	warm->currentPacketNumber=count2;
	
	warm->totalPackets=totalPac;
	num2 = totalPac;
	totalCountPackets=num2;
	warm->serviceTime=serviceTm;
	warm->noTokens=noToken;
	warm->interarrivalTime=interarrivalTm;
	
	
   (void)My402ListAppend(&list, (void*)warm);
    }
	else
	{
	printf("\n Not enough Memory");
	}
   
   
   }
   count2++;
   count++;
            
  }
 
 }
 
}/// File counter

 int err111,err112,err113;
 
 
 rtoken=r2;
 
 char *date;
 time_t Sysshs = time(0);
 long Curyhshshj = (long) Sysshs;
 
 date=ctime(&Curyhshshj);
 
 if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
 
 printf("\n\n00000000.000ms: emulation begins");
 gettimeofday(&start, NULL);  
 startTime=start.tv_sec+(start.tv_usec/1000000.0);
 //printf("startTime:%f",startTime);
 
 ////////////Copyright from William Cheng////////////
sigemptyset(&new);
sigaddset(&new, SIGINT);
pthread_sigmask(SIG_BLOCK, &new, NULL);
pthread_create(&user_threadID, NULL, handler11, argv[1]);
////////////Copyright from William Cheng///////////////


 
 err111 = pthread_create(&(tid1111111111), NULL, &doSomeThing, NULL);
 if (err111 != 0)
 printf("\ncan't create thread :[%s]", strerror(err111));
 
 err112 = pthread_create(&(tid1111111112), NULL, &doSomeThing1, NULL);
 if (err112 != 0)
 printf("\ncan't create thread :[%s]", strerror(err112));
 
 err113= pthread_create(&(tid1111111113), NULL, &server, NULL);
 if (err113 != 0)
 printf("\ncan't create thread :[%s]", strerror(err113));
 
 
 
 
 pthread_join(user_threadID, NULL);
 pthread_join(tid1111111111, NULL);
 pthread_join(tid1111111112, NULL);
 
 pthread_join(tid1111111113, NULL);
 
return 0;
}
