
/*
 * The Arsenal Library
 * Copyright (c) 2009 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */


#include "lalr.h"

AR_NAMESPACE_BEGIN

#define LALR_DEFAULT_CONFIG_NUM			2000
#define LALR_DEFAULT_CONFIG_NODE_NUM	2000
#define LALR_DEFAULT_CONFIG_LIST_NUM	2000
/****************************************lalrConfig aux*****************************************/


static lalrConfig_t		*__g_free_list = NULL;
static arSpinLock_t		__g_config_lock;




static AR_INLINE lalrConfig_t*	__create_config()
{
		if(__g_free_list == NULL)
		{
				return AR_NEW(lalrConfig_t);
		}else
		{
				lalrConfig_t	*res;
				AR_LockSpinLock(&__g_config_lock);

				res  = __g_free_list;
				__g_free_list = (lalrConfig_t*)__g_free_list->forward;

				AR_UnLockSpinLock(&__g_config_lock);
				return res;
		}
}

static AR_INLINE void	__destroy_config(lalrConfig_t *config)
{
		AR_ASSERT(config);
		AR_LockSpinLock(&__g_config_lock);
		config->forward = (lalrConfigList_t*)__g_free_list;
		__g_free_list = config;
		AR_UnLockSpinLock(&__g_config_lock);

}


static AR_INLINE void __init_config_freelist()
{
		size_t i;
		AR_InitSpinLock(&__g_config_lock);
		__g_free_list = NULL;

		for(i = 0; i < LALR_DEFAULT_CONFIG_NUM; ++i)
		{
				__destroy_config(AR_NEW(lalrConfig_t));
		}
		
}

static AR_INLINE void __uninit_config_freelist()
{
		uint32_t count = 0;

		while(__g_free_list)
		{
				lalrConfig_t	*tmp = (lalrConfig_t*)__g_free_list->forward;
				AR_DEL(__g_free_list);
				__g_free_list = tmp;
				count++;
		}

		{
				wchar_t buf[1024];
				AR_swprintf(buf, 1024, L"Total consume lalrConfig_t == %u", count);
				AR_printf(L"%ls\r\n", buf);
		}

		AR_UnInitSpinLock(&__g_config_lock);
}


/****************************************lalrConfigNode_t aux*****************************************/


static lalrConfigNode_t		*__free_node_list = NULL;
static arSpinLock_t			__g_node_lock;




static AR_INLINE lalrConfigNode_t*		__create_node()
{
		if(__free_node_list == NULL)
		{
				return AR_NEW(lalrConfigNode_t);
		}else
		{
				lalrConfigNode_t *res;
				AR_LockSpinLock(&__g_node_lock);

				res = __free_node_list;
				__free_node_list = __free_node_list->next;
				AR_UnLockSpinLock(&__g_node_lock);
				return res;
		}
}

static AR_INLINE void					__destroy_node(lalrConfigNode_t *node)
{
		AR_ASSERT(node);
		AR_LockSpinLock(&__g_node_lock);
		node->next = __free_node_list;
		__free_node_list = node;
		AR_UnLockSpinLock(&__g_node_lock);
}


static AR_INLINE void __init_node_freelist()
{
		size_t i;

		AR_InitSpinLock(&__g_node_lock);
		__free_node_list = NULL;

		for(i = 0; i < LALR_DEFAULT_CONFIG_NODE_NUM; ++i)
		{
				__destroy_node(AR_NEW(lalrConfigNode_t));
		}
}


static AR_INLINE void __uninit_node_freelist()
{
		uint32_t count = 0;
		while(__free_node_list)
		{
				lalrConfigNode_t *tmp = __free_node_list->next;
				AR_DEL(__free_node_list);
				__free_node_list = tmp;
				count++;
		}

		{
				wchar_t buf[1024];
				AR_swprintf(buf, 1024, L"Total consume lalrConfigNode_t == %u", count);
				AR_printf(L"%ls\r\n", buf);
		}

		AR_UnInitSpinLock(&__g_node_lock);
}




/****************************************lalrConfigList_t aux*****************************************/


static lalrConfigList_t		*__free_config_list = NULL;
static arSpinLock_t			__g_config_list_lock;




static AR_INLINE lalrConfigList_t*		__create_config_list()
{
		if(__free_config_list == NULL)
		{
				return AR_NEW(lalrConfigList_t);
		}else
		{
				lalrConfigList_t *res;
				AR_LockSpinLock(&__g_config_list_lock);

				res = __free_config_list;
				__free_config_list = (lalrConfigList_t*)__free_config_list->head;
				AR_UnLockSpinLock(&__g_config_list_lock);
				return res;
		}
}

static AR_INLINE void					__destroy_config_list(lalrConfigList_t *lst)
{
		AR_ASSERT(lst);
		

		AR_LockSpinLock(&__g_config_list_lock);
		lst->head = (lalrConfigNode_t*)__free_config_list;
		__free_config_list = lst;
		AR_UnLockSpinLock(&__g_config_list_lock);
}



static AR_INLINE void __init_config_list_freelist()
{
		size_t i;
		AR_InitSpinLock(&__g_config_list_lock);
		__free_config_list = NULL;

		for(i = 0; i < LALR_DEFAULT_CONFIG_LIST_NUM; ++i)
		{
				__destroy_config_list(AR_NEW(lalrConfigList_t)); 
		}
}


static AR_INLINE void __uninit_config_list_freelist()
{
		uint32_t count = 0;
		while(__free_config_list)
		{
				lalrConfigList_t	*tmp = (lalrConfigList_t*)__free_config_list->head;
				AR_DEL(__free_config_list);
				__free_config_list = tmp;
				count++;
		}
		
		{
				wchar_t buf[1024];
				AR_swprintf(buf, 1024, L"Total consume lalrConfigList_t == %u", count);
				AR_printf(L"%ls\r\n", buf);
		}

		AR_UnInitSpinLock(&__g_config_list_lock);
}




AR_NAMESPACE_END


