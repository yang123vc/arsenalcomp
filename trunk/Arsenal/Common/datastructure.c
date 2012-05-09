/*
 * The Arsenal Library
 * Copyright (c) 2009-2012 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */

#include "common.h"

/******************************************************Print**************************************************************/

AR_NAMESPACE_BEGIN



/*

 */


arList_t*       AR_CreateList(AR_ds_destroy_func_t dtor, void *ctx)
{
        arList_t *lst;
        
        lst = AR_NEW0(arList_t);
        
        if(lst)
        {
                lst->dtor = dtor;
                lst->usr_ctx = ctx;
        }
        
        return lst;
}


void            AR_DestroyList(arList_t *lst)
{
        AR_ASSERT(lst != NULL);
        AR_ClearList(lst);
        AR_DEL(lst);
        lst = NULL;
}


void AR_ClearList(arList_t *lst)
{
        arListNode_t *node, *tmp;
        AR_ASSERT(lst != NULL);
        node = lst->head;
        
        while(node)
        {
                tmp = node->next;
                
                if(lst->dtor)
                {
                        lst->dtor(node->data, lst->usr_ctx);
                }
                
                AR_DEL(node);
                node = tmp;
        }
        
        lst->count = 0;
        lst->head = lst->tail = NULL;
        
}


arStatus_t      AR_InsertToListByNode(arList_t *lst, arListNode_t *node, void *data)
{
        arListNode_t *new_node;
        AR_ASSERT(lst != NULL);
        
        new_node = AR_NEW0(arListNode_t);
        
        if(new_node == NULL)
        {
                return AR_E_NOMEM;
        }
        
        new_node->data = data;
        
        if(lst->count == 0)
        {
                AR_ASSERT(lst->head == NULL && lst->tail == NULL);
                lst->head = lst->tail = new_node;
        }else
        {
                AR_ASSERT(lst->head != NULL && lst->tail != NULL);
                if(node == NULL)                /*如果node为空，则將￼新node插入队首*/
                {
                        new_node->prev = NULL;
                        new_node->next = lst->head;
                        lst->head->prev = new_node;
                        lst->head = new_node;
                }else
                {
                        new_node->prev = node;
                        new_node->next = node->next;
                        if(node->next != NULL)
                        {
                                node->next->prev = new_node;
                        }else
                        {
                                AR_ASSERT(node == lst->tail);
                                lst->tail = new_node;
                        }
                        node->next = new_node;
                }
        }
        
        lst->count += 1;
        return AR_S_YES;
        
}


void      AR_RemoveFromList(arList_t *lst, arListNode_t *node)
{
        AR_ASSERT(lst != NULL && node != NULL);
        
        if(node == lst->head)
        {
                lst->head = node->next;
                if(lst->head != NULL)
                {
                        lst->head->prev = NULL;
                }

        }else
        {
                node->prev->next = node->next;
                if(node->next == NULL)
                {
                        lst->tail = node->prev;
                }else
                {
                        node->next->prev = node->prev;
                }
        }
        
        if(lst->dtor)
        {
                lst->dtor(node->data, lst->usr_ctx);
        }
        
        AR_DEL(node);

        lst->count--;
        if(lst->count == 0)
        {
                lst->head = lst->tail = NULL;
        }
        
}


size_t          AR_GetListCount(const arList_t *lst)
{
        AR_ASSERT(lst != NULL);
        return lst->count;
}




AR_NAMESPACE_END

