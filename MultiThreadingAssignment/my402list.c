/*
 * Author:      Arpit Mittal
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "my402list.h"



//////////////////////////////////////////////////////Starting Doubly linked List Code

//////////////////////////////First Function Init
int My402ListInit(My402List* list)
{
     list->num_members=0;
     list->anchor.next=NULL;
     list->anchor.prev=NULL;
	 list->anchor.obj=NULL;
     return TRUE;
}

//////////////////////////////First Function Append
int  My402ListAppend(My402List* list, void* i)
{
 My402ListElem *check1;
 My402ListElem *my402=(My402ListElem*)malloc(sizeof(My402ListElem));
 if(my402!=NULL)
	{
         
      if( (list->anchor.prev)==NULL)  
		{
      list->num_members=(list->num_members)+1;
	  my402->obj=i;
      my402->next=list;
	  my402->prev=list;
      list->anchor.next=my402;
	  list->anchor.prev=my402;
      return TRUE;
        }
	  else if( (list->anchor.prev)!=NULL)
		{
      check1=list->anchor.prev;
	  list->num_members=(list->num_members)+1;
	  my402->obj=i;
	  check1->next=my402;
	  my402->next=list;
	  my402->prev=check1;
	  list->anchor.prev=my402;
      return TRUE;
       }



	}
 else
	{
    printf("Out of Memory in Append Function");
    return FALSE;
	}

}

/////////////////////////////////////////////////////Third Function Implement
///Returns the first list element or NULL if the list is empty [BC: updated 1/16/2013]. 
My402ListElem *My402ListFirst(My402List* list)
{
My402ListElem  *first;
if( (list->num_members)==0)
	{
    first=NULL;
	return first;
	}
else if((list->num_members)>0)
	{
    first=list->anchor.next;
	return first;
	}

}
/////////////////////////////////////////////////////Fourth Function Implement
////Returns elem->next or NULL if elem is the last item on the list
My402ListElem *My402ListNext(My402List* list, My402ListElem* first)
{
 if( (first->next)==list)
	{
	 return NULL;
	}
 else
	{
      if( (first->next)!=list)
      return (first->next);
	}


}

///////////////////////////////////////////////////Fifth Function Implement
///Unlink and delete elem from the list. Please do not delete the object pointed to by elem 
//and do not check if elem is on the list. 
void My402ListUnlink(My402List* list, My402ListElem* element)
{
 My402ListElem *first,*second;
 if(element!=NULL)
	{

	   if( ( (list->anchor.next)==element) && ((list->anchor.prev)==element) )// First and last Element
		{
         list->num_members=0;
         list->anchor.next=NULL;
         list->anchor.prev=NULL;
	     list->anchor.obj=NULL;
		 free(element);
		}
       else if( (list->anchor.next)==element)// First Element 
		{
         first=list;
		 second=element->next;
		 list->num_members=(list->num_members)-1;
		 list->anchor.next=second;
		 second->prev=list;
         free(element);
        }
	   else if((list->anchor.prev)==element)// Last Element
		{
         first=element->prev;
         second=list;
		 list->num_members=(list->num_members)-1;
		 first->next= list;
         list->anchor.prev=first;
         free(element);
		}
		else// Middle Element
		{
		 list->num_members=(list->num_members)-1;
         first=element->prev;
		 second=element->next;
		 first->next=second;
		 second->prev=first;
		 free(element);
        }
	}
  else if(element==NULL)
	{
	  printf("Element is not passed Properly:-NULL Value");
	}


}

/////////////////////////////////////////////////Sixth Function Implement
//Returns TRUE if the list is empty. Returns FALSE otherwise.
int  My402ListEmpty(My402List* list)
{
if( (list->num_members)==0)
	return TRUE;
else if((list->num_members)>0)
	return FALSE;
}


/////////////Seventh Function implement
//Returns the number of elements in the list. 
int  My402ListLength(My402List* list)
{
return (list->num_members);
}


////////////////////////////Eigth Function Implement
//Returns the last list element or NULL if the list is empty
My402ListElem *My402ListLast(My402List* list)
{
My402ListElem  *first;
if( (list->num_members)==0)
	{
    first=NULL;
	return first;
	}
else if((list->num_members)>0)
	{
    first=list->anchor.prev;
	return first;
	}

}

/////////////////////////////////Ninth function Implement
//Returns elem->prev or NULL if elem is the first item on the list [BC: updated 1/16/2013]. 
//Please do not check if elem is on the list. 
My402ListElem *My402ListPrev(My402List* list, My402ListElem* element)
{
if( (element->prev)==list)
	{
	 return NULL;
	}
 else
	{
      if( (element->prev)!=list)
      return (element->prev);
	}

}

//////////////////////////////////////////Tenth Function Implement
//Unlink and delete all elements from the list and make the list empty. 
//Please do not delete the objects pointed to be the list elements
void My402ListUnlinkAll(My402List* list)
{
     list->num_members=0;
     list->anchor.next=NULL;
     list->anchor.prev=NULL;
	 list->anchor.obj=NULL;
     
}

////////////////////////////////////////////////Eleventh Function
//Add obj before First(). This function returns TRUE if the operation is performed successfully and returns 
//FALSE otherwise.
int  My402ListPrepend(My402List* list, void* i)
{
My402ListElem *check1;
My402ListElem *my402=(My402ListElem*)malloc(sizeof(My402ListElem));
 if(my402!=NULL)
	{
         
      if( (list->anchor.next)==NULL)  
		{
      list->num_members=(list->num_members)+1;
	  my402->obj=i;
      my402->next=list;
	  my402->prev=list;
      list->anchor.next=my402;
	  list->anchor.prev=my402;
      return TRUE;
        }
	  else if( (list->anchor.next)!=NULL)
		{
      check1=list->anchor.next;
	  list->num_members=(list->num_members)+1;
	  my402->obj=i;
	  list->anchor.next=my402;
	  my402->next=check1;
	  my402->prev=list;
	  check1->prev=my402;
	  return TRUE;
       }



	}
 else
	{
    printf("Out of Memory in Prepend Function");
    return FALSE;
	}
}
/////////////////////////////////////////////////////////////////12 th Function 
//Insert obj between elem and elem->prev. If elem is NULL, then this is the same as Prepend(). 
//This function returns TRUE if the operation is performed successfully and returns FALSE otherwise. 
//Please do not check if elem is on the list.
int  My402ListInsertBefore(My402List* list, void* i, My402ListElem* element)
{
My402ListElem *first,*second;
if(element==NULL)
	{
return (My402ListPrepend(list, i));

	}
else if(element!=NULL)
	{
    
	My402ListElem *my402=(My402ListElem*)malloc(sizeof(My402ListElem));
	if(my402!=NULL)
		{
       if((element->prev)==list)// First Node
		{
		 list->num_members=(list->num_members)+1;
         second=list->anchor.next;
		 my402->obj=i;
		 list->anchor.next=my402;
		 my402->next=second;
		 my402->prev=list;
		 second->prev=my402;
         return TRUE;
        }
		else if((element->prev)!=list)// Last Node and Middle Node
		{
		 list->num_members=(list->num_members)+1;
         first=element->prev;
         second=element;
		 my402->obj=i;
		 first->next=my402;
		 my402->next=second;
		 my402->prev=first;
         second->prev=my402;


         return TRUE;
		}
		}
		else
		{
		 return FALSE;
		}

	}



}
//////////////////////////////////////////////////////////////////////13 Th Function
//Insert obj between elem and elem->next. If elem is NULL, then this is the same as Append(). 
//This function returns TRUE if the operation is performed successfully and returns FALSE otherwise. 
//Please do not check if elem is on the list.

int  My402ListInsertAfter(My402List* list, void* i, My402ListElem* element)
{
	My402ListElem *first,*second;
if(element==NULL)
	{
   return (My402ListAppend(list, i));
    
	}
else if(element!=NULL)
	{

   My402ListElem *my402=(My402ListElem*)malloc(sizeof(My402ListElem));
	if(my402!=NULL)
		{
       if((element->next)==list)// Last Node
		{
		 list->num_members=(list->num_members)+1;
         my402->obj=i;
		 first=element;
		 second=list;
		 first->next=my402;
         my402->next=list;
		 my402->prev=first;
		 list->anchor.prev=my402;
         return TRUE;
        }
		else if((element->next)!=list)// First Node and Middle Node
		{
		 list->num_members=(list->num_members)+1;
         
		 my402->obj=i;
		 first=element;
		 second=element->next;
		 first->next=my402;
		 my402->next=second;
		 my402->prev=first;
		 second->prev=my402;



		 
         return TRUE;
		}
		}
		else
		{
		 return FALSE;
		}

	}

}

////////////////////////////////////////////////////////////////////////////////////14 Th Function
// Returns the list element elem such that elem->obj == obj [BC: fixed 1/18/2013]. 
//Returns NULL if no such element can be found.
My402ListElem *My402ListFind(My402List* list, void* i)
{
	My402ListElem *first,*last;
if( (list->num_members)==0)
	{
	return NULL;
	}
else if((list->num_members)!=0)
	{
		 first=list->anchor.next;
		 last=list->anchor.prev;
		 while(first!=last)
			{
			 if(first->obj == i)
				 return first;
			 first=first->next;
			}
			if(first==last)
			{
              if(first->obj == i)
				{
				 return first;
				}
				else
				{
                  return NULL;
				}
			}

			

	}


}




