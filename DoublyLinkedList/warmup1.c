/*
 * Author:      William Chia-Wei Cheng (bill.cheng@acm.org)
 *
 * @(#)$Id: listtest.c,v 1.1 2012/12/24 23:18:19 william Exp $
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "cs402.h"

#include "my402list.h"

typedef struct warmup2 {
   char transactionType;
   long transactionTime;
   int transactionAmtDecimalBefore;
   
   char transactiondescription[1024];

   
} warmup2;



///////////////////////Copyright from William Ching /////////////////////////////////////////

void BubbleForward(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2)
    /* (*pp_elem1) must be closer to First() than (*pp_elem2) */
{
    My402ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
    void *obj1=elem1->obj, *obj2=elem2->obj;
    My402ListElem *elem1prev=My402ListPrev(pList, elem1);
/*  My402ListElem *elem1next=My402ListNext(pList, elem1); */
/*  My402ListElem *elem2prev=My402ListPrev(pList, elem2); */
    My402ListElem *elem2next=My402ListNext(pList, elem2);

    My402ListUnlink(pList, elem1);
    My402ListUnlink(pList, elem2);
    if (elem1prev == NULL) {
        (void)My402ListPrepend(pList, obj2);
        *pp_elem1 = My402ListFirst(pList);
    } else {
        (void)My402ListInsertAfter(pList, obj2, elem1prev);
        *pp_elem1 = My402ListNext(pList, elem1prev);
    }
    if (elem2next == NULL) {
        (void)My402ListAppend(pList, obj1);
        *pp_elem2 = My402ListLast(pList);
    } else {
        (void)My402ListInsertBefore(pList, obj1, elem2next);
        *pp_elem2 = My402ListPrev(pList, elem2next);
    }
}


void BubbleSortForwardList(My402List *pList, int num_items)
{
    My402ListElem *elem=NULL;
    int i=0;

    if (My402ListLength(pList) != num_items) {
        fprintf(stderr, "List length is not %1d in BubbleSortForwardList().\n", num_items);
        exit(1);
    }
    for (i=0; i < num_items; i++) {
        int j=0, something_swapped=FALSE;
        My402ListElem *next_elem=NULL;

        for (elem=My402ListFirst(pList), j=0; j < num_items-i-1; elem=next_elem, j++) {
			warmup2 *obj11111=(warmup2*)(elem->obj);
           // int cur_val=(int)(elem->obj), next_val=0;
		    long cur_val=(long)(obj11111->transactionTime);
			long next_val=0;

            next_elem=My402ListNext(pList, elem);
			warmup2 *obj11112=(warmup2*)(next_elem->obj);
            next_val = (long)(obj11112->transactionTime);

            if (cur_val > next_val) {
                BubbleForward(pList, &elem, &next_elem);
                something_swapped = TRUE;
            }
        }
        if (!something_swapped) break;
    }
}

////////////////////////////////////Copyright from William Ching///////////////////////////



warmup2 *obj11111;
void display(My402List* list)
{


My402ListElem *check1,*start1;
time_t time1111;
char *date;
char *buffer12;
char date111[24];
char desc1111[26];
char *desc;
char transacAmt[15],transacAmt1[15];
int amt=0,amtlength=0,balance=0;

int count111=0,count222=0;

//printf("%s","aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
printf("+-----------------+--------------------------+----------------+----------------+");
printf("\n");
printf("|       Date      | Description              |         Amount |        Balance |");
printf("\n");
printf("+-----------------+--------------------------+----------------+----------------+");
printf("\n");


if( (list->anchor.next)!=NULL)
	{

check1=list->anchor.next;
start1=check1;


while (check1->next  != list)
        {
	        obj11111=(warmup2 *)(check1->obj);
	        //printf("%c %ld %d %s",obj11111->transactionType,obj11111->transactionTime,obj11111->transactionAmtDecimalBefore,obj11111->transactiondescription);
            time1111=  (time_t) (obj11111->transactionTime);
			//printf("\n \n \n Time of Transaction");
           // printf("\nthe time is %s %ld", ctime(&time1111), obj11111->transactionTime);
			date=ctime(&time1111);
			/*printf("\n date is");
			printf("%s",date);
			printf("\n length is");
			printf("%d",strlen(date));*/
			buffer12=date;
			count111=0;
			count222=0;

            while ((*buffer12)!='\n')
            {
                if( (count111>=10)&&(count111<=18))
				{
                  
				}
				else
				{
					date111[count222]=(*buffer12);
					count222++;
				}
				buffer12++;
				count111++;
				
            }
			date111[count222]='\0';
            //printf("\n Dateaaaaaaaaaa is \n");
			//printf("%s",date111);
			// printf("%c %s %d %s",obj11111->transactionType,date111,obj11111->transactionAmtDecimalBefore,obj11111->transactiondescription);
           printf("| ");
		   printf("%s",date111);
		   printf(" |");
		   printf(" ");
		   //printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
		   desc=obj11111->transactiondescription;
		   //////////////////////////////////////
		   buffer12=desc;
			count111=0;
			count222=0;

            while ((*buffer12)!='\0')
            {
              
				if(count222<=23)
				{
					if((*buffer12)!='\n')
					{
				desc1111[count222]=(*buffer12);
					count222++;
					}
				
				
				
				}
				buffer12++;
				count111++;
				
            }
			while(count222<=23)
			   {
           desc1111[count222]=' ';
		   count222++;
		   //buffer12++;
		       }
			
           desc1111[count222]='\0';

		   /////////////////////////////////////

		   printf("%s |",desc1111);

			//desc1111[count222]='\0';
             sprintf(transacAmt, "%d", (obj11111->transactionAmtDecimalBefore));
			 //printf("\n Transaction Amt %s",transacAmt);
			 //printf("\n length %d",strlen(transacAmt));
			char dhdshs[16];
			int jfh,khf;
			 int bssjnsnjsjn=0;
			 int dfg=0;
			 //printf("\n Transaction Amt %s",transacAmt);
			 //printf("\n length %d",strlen(transacAmt));
			 amt=0;
             amtlength=0;
			// printf("\n Transaction Amt");
			// printf("%d",obj11111->transactionAmtDecimalBefore);

             if((obj11111->transactionAmtDecimalBefore)<10000000)
		{

			 
			 if((obj11111->transactionType)=='+')
			{
             transacAmt1[amt]=' ';
			 amt++;
			}
			else
			{
             transacAmt1[amt]=')';
             amt++;
			}


			 amtlength=strlen(transacAmt)-1;
			 int templength = amtlength;
			 			//	printf("%d",amtlength);
			 transacAmt1[amt]=transacAmt[amtlength];
			 amtlength--;
			 amt++;
			 transacAmt1[amt]=transacAmt[amtlength];
			 amtlength--;
			 amt++;
			 transacAmt1[amt]='.';
			 amt++;
			 while(amtlength>=0)
			{
				 if(bssjnsnjsjn == 3 || bssjnsnjsjn == 6)
				{
                   transacAmt1[amt]=',';
				   amt++;

			    }
				 transacAmt1[amt]=transacAmt[amtlength];
            amtlength--;
			amt++;
			bssjnsnjsjn++;
			}
	         dfg=12-amt;
			 while(dfg>=0)
			{
				 transacAmt1[amt]=' ';
             dfg--;
			 amt++;
			}
			if((obj11111->transactionType)=='+')
			{
             transacAmt1[amt]=' ';
			 amt++;
			}
			else
			{
             transacAmt1[amt]='(';
             amt++;
			}
             
			 transacAmt1[amt]='\0';

             
             
			 //printf("\nFinal Transaction Amount %s",transacAmt1);
			
			 for(jfh = 0, khf = amt-1 ; jfh < amt; jfh++, khf--)
			{
				dhdshs[jfh] = transacAmt1[khf];
			 }
			 //jfh++;
			 dhdshs[jfh]='\0';
			  if(templength == 1)
			{
				 jfh = jfh - 5;
				dhdshs[jfh] = '0';
			 }

			   if(templength == 0)
			{
				 jfh = jfh - 5;
				dhdshs[jfh] = '0';
				jfh++;
				dhdshs[jfh] = '.';
				jfh++;
				dhdshs[jfh] = '0';


			 }

             
		}
		else
		{
         if((obj11111->transactionType)=='+')
			{
             //dhdshs="?,???,???.??";
			 strcpy(dhdshs," ?,???,???.?? ");
			}
			else
			{
             //dhdshs="(?,???,???.??)";
			 strcpy(dhdshs,"(?,???,???.??)");
			}
		}


			printf(" %s",dhdshs);
			printf(" |");

			///////////////////////////////////////////////////////Balance Logic


			 if((obj11111->transactionType)=='+')
			{
            balance+=obj11111->transactionAmtDecimalBefore;
			}
			 if((obj11111->transactionType)=='-')
			{
		    balance-=obj11111->transactionAmtDecimalBefore;
			}

			//printf("\nBalance %d",balance);
			int balance1=0;
			if(balance>0)
			{
				balance1=balance;
			}
			if(balance<0)
			{
				balance1=balance*-1;
			}

			
            if(balance1<1000000000)
			{
			sprintf(transacAmt, "%d", (balance));
			amt=0;
            amtlength=0;
			
			 if(balance<0)
				{
             transacAmt1[amt]=')';
			    }
				else
				{
             transacAmt1[amt]=' ';
				}
             amt++;
			
             amtlength=strlen(transacAmt)-1;
			 transacAmt1[amt]=transacAmt[amtlength];
			 amtlength--;
			 amt++;
			 transacAmt1[amt]=transacAmt[amtlength];
			 amtlength--;
			 amt++;
			 transacAmt1[amt]='.';
			 amt++;
			 bssjnsnjsjn=0;
			 while(amtlength>=0)
			{
				 if(bssjnsnjsjn == 3 || bssjnsnjsjn == 6)
				{
                   transacAmt1[amt]=',';
				   amt++;

			    }
				 transacAmt1[amt]=transacAmt[amtlength];
            amtlength--;
			amt++;
			bssjnsnjsjn++;
			}
	         dfg=12-amt;
			 while(dfg>=0)
			{
				 transacAmt1[amt]=' ';
             dfg--;
			 amt++;
			}

			 if(balance<0)
				{
             transacAmt1[amt]='(';
			    }
				else
				{
             transacAmt1[amt]=' ';
				}
             amt++;
 
             
			 transacAmt1[amt]='\0';
			 

			// printf("\n transacAmt1");
			 //printf("%s %d %d",transacAmt1,balance ,balance1);
             
			 //printf("\nFinal Transaction Amount %s",transacAmt1);
			
			 for(jfh = 0, khf = amt-1 ; jfh < amt; jfh++, khf--)
			{   
				//transacAmt1[khf]
				dhdshs[jfh] = transacAmt1[khf];
			 }
			 //jfh++;
			 dhdshs[jfh]='\0';
			 jfh=0;
			 for(jfh=0;jfh<strlen(dhdshs);jfh++)
			 {
             if(dhdshs[jfh]=='-')
				 {
				 dhdshs[jfh]=' ';
				 jfh++;
				 if(dhdshs[jfh]==',')
				 dhdshs[jfh]=' ';



				 }
			 
			 }

			}
			else
			{
              if(balance>0)
				{
              strcpy(dhdshs," ?,???,???.?? ");
				}
			  if(balance<0)
				{
              strcpy(dhdshs,"(?,???,???.??)");
				}
			}
            
			 //printf("\nTransaction Amt %s %s",transacAmt1,dhdshs);
			// printf("\nlength %d %d",strlen(transacAmt1),strlen(dhdshs));
             int templength=strlen(dhdshs);
            
			if(balance<0)
			{

			 if(dhdshs[templength-3]==' ')
			{
               dhdshs[templength-3]='0';
			   dhdshs[templength-5]='0';
			}
			}
			else if(balance>0)
			{
             if(strlen(transacAmt1)==2)
				{
				dhdshs[templength-1]='\0';
				
				 dhdshs[templength-2]=transacAmt1[1];
				 if(dhdshs[templength-3]==' ')
					{
				 dhdshs[templength-3]='0';
				 dhdshs[templength-4]='.';
				 
				 dhdshs[templength-5]='0';

					}

				}
			}
			else if(balance==0)
			{
             printf("          0.00  ");
			printf("|");
			}

            if(strlen(transacAmt1)!=2 && balance != 0)
			{
			printf(" %s",dhdshs);
			printf(" |");
			}
			else if(strlen(transacAmt1)==2 && balance != 0)
			{
            if(balance<0)
            printf("    %s",dhdshs);
			printf("  |");
			}
			



			//////////////////////////////////////////////////////Balance lOgic

			



		   /////////////////////////////////////

		  // printf("%s |",desc1111);
		    printf("\n");
            check1 = check1->next;
        }
		obj11111=(warmup2 *)(check1->obj);
		time1111=  (time_t) (obj11111->transactionTime);


           date=ctime(&time1111);
			/*printf("\n date is");
			printf("%s",date);
			printf("\n length is");
			printf("%d",strlen(date));*/
			buffer12=date;
			count111=0;
			count222=0;

            while ((*buffer12)!='\n')
            {
                if( (count111>=10)&&(count111<=18))
				{
                  
				}
				else
				{
					date111[count222]=(*buffer12);
					count222++;
				}
				buffer12++;
				count111++;
				
            }
			
			date111[count222]='\0';




		//printf("\n \n \n Time of Transaction");
          //  printf("\nthe time is %s %ld", ctime(&time1111), obj11111->transactionTime);
//printf("%c",((obj11111->transactionType)));
//printf("%c %s %d %s",obj11111->transactionType,date111,obj11111->transactionAmtDecimalBefore,obj11111->transactiondescription);
 printf("| ");
printf("%s",date111);
printf(" |");
printf(" ");
//printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
		   desc=obj11111->transactiondescription;
		   //////////////////////////////////////
		   buffer12=desc;
			count111=0;
			count222=0;

            while ((*buffer12)!='\0')
            {
              
				if(count222<=23)
				{
					if((*buffer12)!='\n')
					{
				desc1111[count222]=(*buffer12);
					count222++;
					}
				
				
				
				}
				buffer12++;
				count111++;
				
            }
			while(count222<=23)
			   {
           desc1111[count222]=' ';
		   count222++;
		   //buffer12++;
		       }
			
           desc1111[count222]='\0';

		   /////////////////////////////////////

		   printf("%s |",desc1111);


           //desc1111[count222]='\0';
             sprintf(transacAmt, "%d", (obj11111->transactionAmtDecimalBefore));
			 //printf("\n Transaction Amt %s",transacAmt);
			 //printf("\n length %d",strlen(transacAmt));
			char dhdshs[16];
			int jfh,khf;
			int bssjnsnjsjn=0;
			 int dfg=0;
			 //printf("\n Transaction Amt %s",transacAmt);
			 //printf("\n length %d",strlen(transacAmt));
			 amt=0;
             amtlength=0;
			// printf("\n Transaction Amt");
			// printf("%d",obj11111->transactionAmtDecimalBefore);

             if((obj11111->transactionAmtDecimalBefore)<10000000)
		{

			 
			 if((obj11111->transactionType)=='+')
			{
             transacAmt1[amt]=' ';
			 amt++;
			}
			else
			{
             transacAmt1[amt]=')';
             amt++;
			}
             amtlength=strlen(transacAmt)-1;
				int templength = amtlength;
			 transacAmt1[amt]=transacAmt[amtlength];
			 amtlength--;
			 amt++;
			 transacAmt1[amt]=transacAmt[amtlength];
			 amtlength--;
			 amt++;
			 transacAmt1[amt]='.';
			 amt++;
			
			 while(amtlength>=0)
			{
				 if(bssjnsnjsjn == 3 || bssjnsnjsjn == 6)
				{
                   transacAmt1[amt]=',';
				   amt++;

			    }
				 transacAmt1[amt]=transacAmt[amtlength];
            amtlength--;
			amt++;
			bssjnsnjsjn++;
			}
	         dfg=12-amt;
			 while(dfg>=0)
			{
				 transacAmt1[amt]=' ';
             dfg--;
			 amt++;
			}
			if((obj11111->transactionType)=='+')
			{
             transacAmt1[amt]=' ';
			 amt++;
			}
			else
			{
             transacAmt1[amt]='(';
             amt++;
			}
             
			 transacAmt1[amt]='\0';
             
			 //printf("\nFinal Transaction Amount %s",transacAmt1);
			
			 for(jfh = 0, khf = amt-1 ; jfh < amt; jfh++, khf--)
			{
				dhdshs[jfh] = transacAmt1[khf];
			 }
			 //jfh++;
			 dhdshs[jfh]='\0';

             if(templength == 1)
			{
				 jfh = jfh - 5;
				dhdshs[jfh] = '0';
			 }

			   if(templength == 0)
			{
				 jfh = jfh - 5;
				dhdshs[jfh] = '0';
				jfh++;
				dhdshs[jfh] = '.';
				jfh++;
				dhdshs[jfh] = '0';


			 }
		}
		else
		{
         if((obj11111->transactionType)=='+')
			{
             //dhdshs="?,???,???.??";
			 strcpy(dhdshs," ?,???,???.?? ");
			}
			else
			{
             //dhdshs="(?,???,???.??)";
			 strcpy(dhdshs,"(?,???,???.??)");
			}
		}
            printf(" %s",dhdshs);
			printf(" |");
			
			///////////////////////////////////////////////////////Balance Logic
///////////////////////////////////////////////////////Balance Logic
			 if((obj11111->transactionType)=='+')
			{
            balance+=obj11111->transactionAmtDecimalBefore;
			}
			 if((obj11111->transactionType)=='-')
			{
		    balance-=obj11111->transactionAmtDecimalBefore;
			}
			//printf("\nBalance %d",balance);

			int balance1=0;
			if(balance>0)
			{
				balance1=balance;
			}
			if(balance<0)
			{
				balance1=balance*-1;
			}

			
            if(balance1<1000000000)
			{
			sprintf(transacAmt, "%d", (balance));
			amt=0;
            amtlength=0;
			
			 if(balance<0)
				{
             transacAmt1[amt]=')';
			    }
				else
				{
             transacAmt1[amt]=' ';
				}
             amt++;
			
             amtlength=strlen(transacAmt)-1;
			 transacAmt1[amt]=transacAmt[amtlength];
			 amtlength--;
			 amt++;
			 transacAmt1[amt]=transacAmt[amtlength];
			 amtlength--;
			 amt++;
			 transacAmt1[amt]='.';
			 amt++;
			 bssjnsnjsjn=0;
			// printf("%d",amtlength);
			 while(amtlength>=0)
			{
				 if(bssjnsnjsjn == 3 || bssjnsnjsjn == 6)
				{
                   transacAmt1[amt]=',';
				   amt++;

			    }
				 transacAmt1[amt]=transacAmt[amtlength];
            amtlength--;
			amt++;
			bssjnsnjsjn++;
			}
	         dfg=12-amt;
			 while(dfg>=0)
			{
				 transacAmt1[amt]=' ';
             dfg--;
			 amt++;
			}

			 if(balance<0)
				{
             transacAmt1[amt]='(';
			    }
				else
				{
             transacAmt1[amt]=' ';
				}
             amt++;
 
             
			 transacAmt1[amt]='\0';

			// printf("\nTransaction Amt %s",transacAmt1);
			 //printf("\nlength %d",strlen(transacAmt1));


			// printf("\n transacAmt1");
			 //printf("%s %d %d",transacAmt1,balance ,balance1);
             
			 //printf("\nFinal Transaction Amount %s",transacAmt1);
			
			 for(jfh = 0, khf = amt-1 ; jfh < amt; jfh++, khf--)
			{   
				//transacAmt1[khf]
				dhdshs[jfh] = transacAmt1[khf];
			 }
			 //jfh++;
			 dhdshs[jfh]='\0';
			 jfh=0;
			 for(jfh=0;jfh<strlen(dhdshs);jfh++)
			 {
             if(dhdshs[jfh]=='-')
				 {
				 dhdshs[jfh]=' ';
				 jfh++;
				 if(dhdshs[jfh]==',')
				 dhdshs[jfh]=' ';



				 }
			 
			 }

			}
			else
			{
              if(balance>0)
				{
              strcpy(dhdshs," ?,???,???.?? ");
				}
			  if(balance<0)
				{
              strcpy(dhdshs,"(?,???,???.??)");
				}
			}


			int templength=strlen(dhdshs);
            
			if(balance<0)
			{

			 if(dhdshs[templength-3]==' ')
			{
               dhdshs[templength-3]='0';
			   dhdshs[templength-5]='0';
			}
			}
			else if(balance>0)
			{
             if(strlen(transacAmt1)==2)
				{
				dhdshs[templength-1]='\0';
				
				 dhdshs[templength-2]=transacAmt1[1];
				 if(dhdshs[templength-3]==' ')
					{
				 dhdshs[templength-3]='0';
				 dhdshs[templength-4]='.';
				 
				 dhdshs[templength-5]='0';

					}

				}
			}
			else if(balance==0)
			{
             printf("          0.00  ");
			printf("|");
			}

            if(strlen(transacAmt1)!=2 &&  balance != 0)
			{
			printf(" %s",dhdshs);
			printf(" |");
			}
			else if(strlen(transacAmt1)==2 &&  balance != 0)
			{
				if(balance<0)
            printf(" %s",dhdshs);
			printf(" |");
			}
			






			//////////////////////////////////////////////////////Balance lOgic

			//////////////////////////////////////////////////////Balance lOgic


//printf("%s",obj11111->transactiondescription);
printf("\n");
printf("+-----------------+--------------------------+----------------+----------------+");
printf("\n");
/*printf("\nNo of Members in List");
printf("%d",(list->num_members));

printf("Previous checking \n");
start1=list->anchor.prev;
while ( start1 != list)
        {
	obj11111=(warmup2 *)(start1->obj);
	printf("\n \n \n Time of Transaction");
	        printf("%c",((obj11111->transactionType)));
			printf("\n \n");
printf("%c %ld %d %s",obj11111->transactionType,obj11111->transactionTime,obj11111->transactionAmtDecimalBefore,obj11111->transactiondescription);

            start1 = start1->prev;
        }*/
//printf("%d",((start1->obj)));
	}


}


/* ---------------------	-- main() ----------------------- */

///////////////Doubts
//Sort Condition check,Tfile.txt hogi ya nahi
////////////////////



int main(int argc, char *argv[])
{
	char buf[1026];
	char bufarpit[1026];

	FILE *file,*file1,*file2;
 
   char transactionType,transactionType1;
   long transactionTime,transactionTime1;
   double transactionAmtDecimalBefore;
   char transactionAmtDecimalBefore1[1024];
   char transactiondescription[1024],transactiondescription1[1024];

   char transactionType2;
   long transactionTime2;
   char transactionAmtDecimalBefore2111[1024];
   char transactiondescription2[1024];

   int count=0;
   //warmup2 warmup2structure;

   My402List list;
   //printf("Start of Program");
   memset(&list, 0, sizeof(My402List));
   My402ListInit(&list);
   time_t Sysshs = time(0);
   long Curyhshshj = (long) Sysshs;

   // printf("Start Warm up");
//////////////////////////////////////////Malformed Command Exceptions Handling

  //./warmup1(malformed command)

  if(argc ==1)
	{
	  printf("Malformed Command");
	  exit(1);
	}


  

 // ./warmup1 -y sort(malformed command)
                
 // ./warmup1 xyz(malformed command)
                
  
  //./warmup1 abc def ghi(malformed command)
                
  
  //./warmup1 ksjdfjwiejofjasdfjowkejokjaosijfioejfsiejriwjeirjwier(malformed command)

 


if((strcmp(argv[1],"sort"))!=0)
	{
   printf("Malformed Command");
   	  exit(1);
	}


	 //./warmup1 sort -x


                    
  //./warmup1 sort /usr/bin/xyzz(input file /usr/bin/xyzz does not exist)
                
  
 // ./warmup1 sort /etc/sysidcfg(input file /etc/sysidcfg cannot be opened - access denies)
                
 // ./warmup1 sort /etc/inet/secret/xyzz(input file /etc/inet/secret/xyzz cannot be opened - access denies)
                
  
 // ./warmup1 sort /etc(input file /etc is a directory)
                
  
 // ./warmup1 sort /etc/motd(input file is not in the right format)
                

  //./warmup1 sort ~/.login(input file is not in the right format)




//////////////////////////////////////////

	if(argc ==3)// read from Tfile, argv[2] has TFILE
	{
		//printf("First Arguement:%s",argv[0]);
		//printf("Second Arguement:%s",argv[1]);
		//printf("Third Arguement:%s",argv[2]);
		
		/////////////////////File Reading Logic

        file = fopen( argv[2], "r" );
		//file1= fopen( argv[2], "r" );

        /* fopen returns 0, the NULL pointer, on failure */
        
		////////////////////Not File Reading Logic
        
     
	}
    else if(argc==2)// Tfile not specified.Program reads from STDIN, argv[1] has TFILE
	{
        //printf("First Arguement:%s",argv[0]);
		//printf("Second Arguement:%s",argv[1]);
		file=stdin;
		file2 = file;

		//file1=stdin;
	}
	else
	{
		printf("Arguements are Wrong");
		exit(1);
	}

	///////////////////////////////////////////////File Reading Logic

	int check111=0;
	char check2;
    if ( file == 0 )
        {
            printf( "Could not open file\n" );
			exit(1);
        }
        else 
        {
			
  //          char tab123[100];
			char * buf1;
			//int count=0;
//			int i=0;
			int iiiii=0;
			//if(fgets(buf, sizeof(buf), file) == NULL){printf("Empty File");}
			//warmup2structure *my4021=(My402ListElem*)malloc(sizeof(My402ListElem));
			
			while (fgets(buf, sizeof(buf), file) != NULL) 
			{
            buf1=buf;
			iiiii=0;
//			printf ("\n%s", buf);
		
			if(argc ==3)// read from Tfile, argv[2] has TFILE
			{

			file1= fopen( argv[2], "r" );

			}
			if (argc==2)// Tfile not specified.Program reads from STDIN, argv[1] has TFILE
			{
			
	//			file1=stdin;
			}

				int dghdhjd=0;
			while(*buf1!='\n')
				{
					if((*buf1)=='\t')
                    iiiii++;
					buf1++;
					dghdhjd++;
				}
                //printf("\n \n Length of Buffer %d",dghdhjd);

				if(dghdhjd>1024)
				{
					printf("\n Buffer length is more than 1024 characters");
					exit(1);
				}
				
			//printf("\n\n I++++++++++++++	%d",iiiii);
            if(iiiii!=3)
				{
				printf("Tfile doesnot contain 4 string fields with exactly 3 <TAB> characters being the delimeters");
			exit(1);
				}
            
            buf1=buf;
			check111=0;
			//check2='t';
			while((*buf1)!='\t')
				{
               check111++;
			   check2=*buf1;
			   buf1++;
				}
				
			if(check111!=1)
				{
				printf("\n \n More than 1 character in Transaction Type");
				exit(1);
				}
				//printf("\n \n check2%c",check2);
			if((check2=='+')||(check2=='-'))
				{
				
                
				}
				else
				{
              printf("\n \n Format of Transaction Type is wrong");
				exit(1);
				}
			
			char *checksh = buf;
				int ui = 0;
				for (ui=0 ;ui < 3 ;ui++ )
				{
					while((*checksh)!='\t')
					{
						checksh++;
					}
					checksh++;
	                //printf("\n \n checksh %s",checksh);
				}
                
//                printf("\n \n checksh %s",checksh);

				if (*checksh == NULL || *checksh == '\n' )
				{
					printf("Transaction Description is empty ");
					exit(1);
				}
					int match=0;				
				


            sscanf(buf,"%c \t %ld \t %s \t %s",&transactionType1,&transactionTime1,&transactionAmtDecimalBefore1,&transactiondescription1);
            //////////////////////////////////////File Logic Matching//////////////////////


			if (argc == 3 )
			{
			
			while (fgets(bufarpit, sizeof(bufarpit), file1) != NULL) 
			{
				sscanf(bufarpit,"%c \t %ld \t %s \t %s",&transactionType2,&transactionTime2,&transactionAmtDecimalBefore2111,&transactiondescription2);
				if (transactionTime1 == transactionTime2)
				{
					match++;
				}
//				printf("\n%s %s %ld",bufarpit,buf,transactionTime2);
				if (match > 1)
				{
					printf("Time Stamp is not unique");
					exit(1);
				}
			}
			}
			
//printf("%d",match);


			/////////////////////////////////////


			//printf("\n \n TRANSACTION DESCRIPTION");
			//printf("%s",transactionAmtDecimalBefore1);

			char *test1;
			int coutrttt = 0;
			int ghhddh=0;
			test1 = transactionAmtDecimalBefore1;
           
            while (*test1 != '\0')
			{
				coutrttt++;
				if((*test1)=='.')
				{
                ghhddh=1;
				}

				test1++;
			}

			if(ghhddh!=1)
				{
				printf("\n Not have period");
				exit(1);
				}
            
            coutrttt = 0;
			test1 = transactionAmtDecimalBefore1;
			while (*test1 != '.')
			{
				coutrttt++;
				test1++;
			}
			
			char *test2 = test1;
			/*if (coutrttt > 7)
			{
				printf ("Error  more then 7");
				exit(1);
			}*/
			 //printf("\n \n coutrttt");
			//printf("%d",coutrttt);
			int cuntrrr = 0;
			while (*test2 != '\0')
			{
				cuntrrr++;
				test2++;
			}
    

			if ((cuntrrr-1)!=2)
			{
				printf ("Error  more then 2 Decimals in Transaction Amt Decimal Before");
				exit(1);
			}






			sscanf(buf,"%c \t %ld \t %lf \t %s",&transactionType,&transactionTime,&transactionAmtDecimalBefore,&transactiondescription);
			


			if(transactionAmtDecimalBefore<0)
				{
            printf("transactionAmtDecimalBefore is -ve must should be POSITIVE");
			exit(1);
				}

			if(transactionTime<0)
				{
            printf("Transaction Time must be between 0 and current System Time");
			exit(1);
				}


				if(transactionTime>Curyhshshj)
				{
		    printf("Transaction Time cannot be greater than System time");
            exit(1);

				}

			char *start_ptr = buf;
            char *tab_ptr = strchr(start_ptr,*(transactiondescription));
			
			warmup2 *warm=(warmup2*)malloc(sizeof(warmup2));
			if(warm!=NULL)
				{

			warm->transactionType=transactionType;
			warm->transactionTime=transactionTime;

			int ihu = 0;
			char dollar[30];
			char penny[30];
			while (transactionAmtDecimalBefore1[ihu] != '.')
			{
				dollar[ihu] = transactionAmtDecimalBefore1[ihu];
				ihu++;
			}
				dollar[ihu] = '\0';
			ihu++;
			int iho=0;
			while (transactionAmtDecimalBefore1[ihu] != '\0')
			{
				penny[iho] = transactionAmtDecimalBefore1[ihu];
				ihu++;
				iho++;
			}
				penny[iho] = '\0';
				
			strcat(dollar,penny);

			 warm->transactionAmtDecimalBefore= atoi(dollar);

			sprintf(warm->transactiondescription,tab_ptr);
			(void)My402ListAppend(&list, (void*)warm);


			count++;
				}
			


			
            

            }
			
		/*warmup2 *obj=(warmup2 *)obj2;
		warmup2 *obj3=(warmup2 *)obj1;*/
            // ("checking %c %ld %d %s",warm1[i11].transactionType,warm1[i11].transactionTime,warm1[i11].transactionAmtDecimalBefore,warm1[i11].transactiondescription);
//printf("checking %c %ld %d %s",obj->transactionType,obj->transactionTime,obj->transactionAmtDecimalBefore,obj->transactiondescription);
//printf("checking %c %ld %d %s",obj3->transactionType,obj3->transactionTime,obj3->transactionAmtDecimalBefore,obj3->transactiondescription);

				//}


//			fclose( file );
//			fclose( file1 );
        }

	//////////////////////////////////////////////
	//printf("Displaying");
	int length=My402ListLength(&list);
	BubbleSortForwardList(&list,length);
    display(&list);
	//printf("End Warm up");
    return(0);
}
