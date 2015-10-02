/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines. Remove this comment and provide
 * a summary of your allocator's design here.
 */

#include "mm_alloc.h"
#include <stdlib.h>
#include<stdio.h>
//#include <unistd.h>
#define a(x) ((((x) - 1) >> 2) << 2) + 4



/* Your final implementation should comment out this macro. */
#define MM_USE_STUBS



void *base = NULL;



void* mm_malloc(size_t size)
{
	 s_block_ptr b, last;
	size_t s;
	s = a(size);
	if(base){
	last = base;
	while (b && !(b->free && b->size >= s)){
	last = b;
	b = b->next;
	}
	if(b){
	if((b->size - s) >= (24)) split_block(b, s);
	b->free = 0;
	}
	else{
	b = extend_heap(last, s);
	if(!b) return NULL;
	}
	}
	else{
	b = extend_heap(NULL, s);
	if(!b) return NULL;
	base = b;
	}
	return b->data;
}

void* mm_realloc(void* ptr, size_t size)
{
	 size_t s;
	s_block_ptr b, new;
	void *newp;
	if(!ptr) return (malloc(size));
	if (valid_addr(ptr)){
	s = a(size);
	b = get_block(ptr);
	if (b->size >= s){
	if(b->size - s >= (24)) split_block(b, s);
	}
	else{
	if(b->next && b->next->free && (b->size + 20 + b->next->size) >= s){
	fusion(b);
	if(b->size - s >= (24)) split_block(b, s);
	}
	else{
	newp = malloc(s);
	if (!newp) return NULL;
	new = get_block(newp);
	int *sdata, *ddata;
	size_t i;
	sdata = b->ptr;
	ddata = new->ptr;
	for(i = 0; i * 4 < b->size && i * 4 < new->size; i++)
	ddata[i] = sdata[i];
	free(ptr);
	return newp;
	}
	}
	return ptr;
	}
	return NULL;
}



int valid_addr(void *p){
	if (base){
	if(p > base && p < sbrk(0)) return (p == (get_block(p))->ptr);
	}
	return 0;
}

void mm_free(void* ptr)
{



	s_block_ptr b;
	if (valid_addr(ptr)){
	b = get_block(ptr);
	b->free = 1;
	if(b->prev && b->prev->free) b = fusion(b->prev);
	if(b->next){
	fusion(b);
	}
	else{
	if(b->prev) b->prev->next = NULL;
	else base = NULL;
	brk(b);
	}
	}
}
