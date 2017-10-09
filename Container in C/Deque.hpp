#include<string.h>
#include<stdlib.h>
#include<limits.h>
#define blocksize 8

#define xstr(s) str(s)
#define str(s) #s
#define foo(t) Deque_##t

#define Deque_DEFINE(t)\
struct Deque_##t;\
typedef struct Deque_##t##_Iterator{\
	int index =0;\
	int iswarped =0 ;\
	char name[20] = "Iterator";\
	Deque_##t *dq;\
	void (*inc)(struct Deque_##t##_Iterator*); \
	void (*dec)(struct Deque_##t##_Iterator*);\
	t& (*deref)(struct Deque_##t##_Iterator*);\
}Deque_##t##_Iterator;\
\
\
typedef struct Deque_##t{  \
int capacity = blocksize;                     \
t *arr;                                         \
int qfront =0;                                   \
int rear =-1;                                     \
int qsize =0;                                    \
int (*size)(struct Deque_##t*);                   \
bool (*empty)(struct Deque_##t*);                   \
char type_name[sizeof(xstr(foo(t)))];                \
void (*push_back)(struct Deque_##t*, t);              \
void (*push_front)(struct Deque_##t*, t);              \
t& (*front)(struct Deque_##t*);                         \
t&  (*back)(struct Deque_##t*);                          \
void (*pop_front)(struct Deque_##t*);                     \
void (*pop_back)(struct Deque_##t*);                       \
bool (*isfull)(struct Deque_##t*);                          \
void (*resize)(struct Deque_##t*);                           \
t& (*at)(struct Deque_##t*, int);\
Deque_##t##_Iterator(*begin)(struct Deque_##t*);\
Deque_##t##_Iterator(*end)(struct Deque_##t*);\
bool (*comp)(const t&,const t&);\
int (*comp_wrap)(const void *,const void *);\
void (*dtor)(struct Deque_##t*);\
void (*clear)(struct Deque_##t*);\
void (*sort)(struct Deque_##t*,Deque_##t##_Iterator,Deque_##t##_Iterator);\
}Deque_##t;                                                   \
\
\
void Deque_##t##_clear(struct Deque_##t* q)\
{\
	q->qfront=0;\
	q->qsize=0;\
	free(q->arr);\
	q->arr = (t*)malloc(sizeof(t)*blocksize);\
	q->capacity = blocksize;\
}\
\
\
void Deque_##t##_dtor(struct Deque_##t* q)\
{\
	free(q->arr);\
}\
\
\
void Deque_##t##_Iterator_dec(struct Deque_##t##_Iterator *itr)\
{\
itr->index--;\
if(itr->index<0)\
{\
	itr->index = itr->dq->capacity;\
}\
	\
}\
\
\
void Deque_##t##_Iterator_inc(struct Deque_##t##_Iterator *itr)\
{\
	itr->index++;\
	if(itr->index>=itr->dq->capacity)\
		itr->index=0;\
}\
\
bool Deque_##t##_Iterator_equal(struct Deque_##t##_Iterator itr1,struct Deque_##t##_Iterator itr2)\
{\
	if(itr1.index==(itr2.index))\
		return true;\
		else\
		return false;\
}\
\
\
t& Deque_##t##_Iterator_deref(struct Deque_##t##_Iterator *itr)\
{\
	return(itr->dq->arr[itr->index]);\
}\
\
\
t* Deque_##t##_Iterator_derefaddr(struct Deque_##t##_Iterator *itr)\
{\
	return(&(itr->dq->arr[itr->index]));\
}\
\
\
Deque_##t##_Iterator Deque_##t##_begin(Deque_##t *q)\
{\
	Deque_##t##_Iterator iterator;\
	iterator.dq = q;\
	iterator.inc = &Deque_##t##_Iterator_inc;\
	iterator.deref = &Deque_##t##_Iterator_deref;\
	iterator.index = q->qfront;\
	return iterator;\
}\
\
\
Deque_##t##_Iterator Deque_##t##_end(Deque_##t *q)\
{\
	int rear;\
	Deque_##t##_Iterator iterator;\
	iterator.inc = &Deque_##t##_Iterator_inc;\
	iterator.dec = &Deque_##t##_Iterator_dec;\
	iterator.deref = &Deque_##t##_Iterator_deref;\
	iterator.dq = q;\
	rear = q->qfront + q->qsize;\
	if(rear>=q->capacity)\
	{\
		rear = rear - q->capacity;\
		iterator.index = rear;\
		iterator.iswarped =1;\
	}\
	else\
	{\
		iterator.index = rear;\
		iterator.iswarped =0;\
	}\
	return iterator;\
}\
\
\
bool Deque_##t##_isfull(Deque_##t *q)                            \
{                                                                 \
	if((q->qfront==0&&q->rear==q->capacity-1)||q->qfront==q->rear+1) \
		return true;                                                \
}                                                                    \
\
\
bool Deque_##t##_empty(Deque_##t *q)                                   \
{                                                                       \
	if(q->qsize==0)                                                      \
		return true;\
	else \
		return false;\
}                                                                         \
\
\
void Deque_##t##_push_front(Deque_##t *q, t elem) \
{\
	if(q->qsize>=q->capacity)\
	{\
		q->resize(q);\
	}\
	q->qfront = q->qfront-1;\
	if(q->qfront<0)\
	{\
		q->qfront = q->qfront +q->capacity;\
	}\
	q->arr[q->qfront] = elem;\
	q->qsize = q->qsize +1;\
}\
\
\
void Deque_##t##_push_back(Deque_##t *q, t elem) \
{\
	int rear =0;\
	if(q->qsize>=q->capacity)\
	{\
		q->resize(q);\
	}\
	rear = q->qfront + q->qsize;\
	q->qsize = q->qsize +1;\
	if(rear>=q->capacity)\
	{\
		rear = rear - q->capacity;\
	}\
	q->arr[rear] = elem;\
}\
\
\
void Deque_##t##_pop_back(Deque_##t *q)          \
{\
	q->qsize--;\
}	\
\
\
void Deque_##t##_pop_front(Deque_##t *q)                 \
{\
	q->qsize--;\
	q->qfront = q->qfront +1;\
	if(q->qfront >= q->capacity)\
	{\
		q->qfront = q->qfront - q->capacity;\
	}\
}	\
\
\
t& Deque_##t##_front(Deque_##t *q)  \
{\
	return (q->arr[q->qfront]);\
}	\
\
\
t& Deque_##t##_back(Deque_##t *q)        \
{\
	int rear;\
	rear = q->qfront + q->qsize -1;\
	if(rear>q->capacity)\
	{\
		rear = rear - q->capacity;\
	}\
	return q->arr[rear];\
}	\
\
\
t& Deque_##t##_at(Deque_##t *q, int index)\
{\
	int logical_index = 0;\
	logical_index = q->qfront +index;\
	if(logical_index >=q->capacity)\
	{\
		logical_index = index -(q->capacity - q->qfront);\
	}\
	return (q->arr[logical_index]);\
}\
\
\
void Deque_##t##_resize(Deque_##t *q)                      \
{\
	int i,j;\
	t *newq = (t *)malloc(2*q->capacity*sizeof(t));\
	assert(newq!=0);\
	j = q->qfront;\
	for(i=0;i<q->qsize;i++)\
	{\
		newq[i] = q->arr[j];\
		j = j+1;\
		if(j>=q->capacity)\
		{\
			j = j-q->capacity;\
		}\
	}\
	free(q->arr);\
	q->qfront =0;\
	q->capacity = 2*q->capacity;\
	q->arr = newq;\
}	\
\
\
int Deque_##t##_size(struct Deque_##t *q)\
{ \
return q->qsize;\
}\
\
\
bool Deque_##t##_equal(Deque_##t deq1,Deque_##t deq2)\
{\
	int i=0;\
	if(deq1.size(&deq1) != deq2.size(&deq2))\
	{\
		return false;\
	}\
	else\
	{\
		int size = deq1.size(&deq1);\
		for(i=0;i<size;i++)\
		{\
			if(deq1.comp(deq1.at(&deq1,i),deq2.at(&deq2,i)))\
			{\
			return false;\
			}\
			else\
			{\
				if(!(deq1.comp(deq1.at(&deq1,i),deq2.at(&deq2,i)))&&!(deq1.comp(deq2.at(&deq2,i),deq1.at(&deq1,i))))\
				{\
				continue;\
				}\
				else\
				{\
				return false;\
				}\
			}\
		}\
		\
	}\
	\
}\
\
\
typedef struct comp##t\
{\
	t a;\
	bool (*comp)(const t&,const t&);\
}comp##t;\
\
\
int comp_wrap_##t(const void *o1,const void *o2)\
{\
	comp##t * os1 = (comp##t *)o1;\
	comp##t * os2 = (comp##t *)o2;\
	if(os2->comp(os1->a,os2->a))\
	{\
		return -1;\
	}\
	else if (os2->comp(os2->a,os1->a))\
	{\
		return 1;\
	}\
    else{\
		return 0;\
	}\
}\
\
void Deque_##t##_sort(struct Deque_##t* q,Deque_##t##_Iterator itr_front,Deque_##t##_Iterator itr_end)\
{\
	int i=0;\
	int start = itr_front.index;\
	int end = itr_end.index;\
	int size = end - start;\
	if (size < 0) size = q->capacity + size;\
	size = size;\
	struct comp##t *tmp = (comp##t*)malloc(sizeof(comp##t)*size);\
	for (i = 0;i<size;i++)\
		 {\
		\
			tmp[i].comp = q->comp;\
			tmp[i].a =q->arr[(start+i)%q->capacity];\
		 }\
		 \
		 qsort(tmp,size,sizeof(comp##t),q->comp_wrap);\
		 for(i=0;i<size;i++)\
		 {\
			 q->arr[(start+i)%q->capacity] = tmp[i].a;\
		 }\
	free(tmp);\
}\
\
\
\
void Deque_##t##_ctor(Deque_##t *q,bool(*f)(const t&,const t&)) \
{\
	q->arr = (t*)malloc(sizeof(t)*blocksize);  \
	q->empty = &Deque_##t##_empty;	\
	strncpy(q->type_name,xstr(foo(t)),sizeof(xstr(foo(t))));\
	q->push_back = &Deque_##t##_push_back;           \
	q->push_front = &Deque_##t##_push_front;          \
	q->front = &Deque_##t##_front;                \
	q->back = &Deque_##t##_back;                   \
	q->front = &Deque_##t##_front;                  \
	q->pop_front = &Deque_##t##_pop_front;           \
	q->pop_back = &Deque_##t##_pop_back;              \
	q->isfull = &Deque_##t##_isfull;                   \
	q->resize = &Deque_##t##_resize;  	\
	q->size = &Deque_##t##_size;\
	q->begin = &Deque_##t##_begin;\
	q->end = &Deque_##t##_end;\
	q->at = &Deque_##t##_at;\
	q->comp =f;\
	q->comp_wrap=&comp_wrap_##t;\
	q->clear =&Deque_##t##_clear;\
	q->dtor = &Deque_##t##_dtor;\
	q->sort =&Deque_##t##_sort;\
}































