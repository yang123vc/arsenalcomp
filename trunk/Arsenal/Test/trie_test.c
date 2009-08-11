#include "test.h"


AR_NAMESPACE_BEGIN

#if defined(AR_DEBUG)


typedef struct __arsenal_trie_node_tag	arTrieNode_t;


typedef struct __arsenal_trie_node_tag
{
		wchar_t					ch;
		void					*final;
		arTrieNode_t			**nodes;
		size_t					count;
}arTrieNode_t;

static arTrieNode_t*	__create_node(wchar_t ch)
{
		arTrieNode_t *res;

		res = AR_NEW(arTrieNode_t);
		res->ch = ch;
		res->final = NULL;
		res->nodes = NULL;
		res->count = 0;
		return res;
}

static void				__destroy_node(arTrieNode_t *node)
{
		AR_ASSERT(node != NULL);
		if(node)
		{
				if(node->nodes)AR_DEL(node->nodes);
				AR_DEL(node);
		}
}

static void __insert_to_node_order(arTrieNode_t *node, arTrieNode_t *child)
{
		size_t i;
		AR_ASSERT(node != NULL && child != NULL);
		
		node->nodes = (arTrieNode_t**)AR_realloc(node->nodes, sizeof(arTrieNode_t*) * (node->count + 1));
		
		for(i = 0; i < node->count && node->nodes[i]->ch < child->ch; ++i);
		
		if(i < node->count)AR_ASSERT(node->nodes[i]->ch != child->ch);
		while(i < node->count)
		{
				arTrieNode_t *tmp = node->nodes[i];
				node->nodes[i] = child;
				child = tmp;
				++i;
		}
		node->nodes[node->count++] = child;
}

static void __insert_to_node(arTrieNode_t *node, arTrieNode_t *child)
{
		AR_ASSERT(node != NULL && child != NULL);
		
		node->nodes = (arTrieNode_t**)AR_realloc(node->nodes, sizeof(arTrieNode_t*) * (node->count + 1));

		node->nodes[node->count++] = child;
}

static int_t __index_of_node(const arTrieNode_t *node, wchar_t c)
{
		int_t l,r,m;
		AR_ASSERT(node != NULL);

		l = 0; r = (int_t)node->count - 1;
		while(l <= r)
		{
				m = (l + r)/2;
				if(node->nodes[m]->ch == c)
				{
						return m;
				}else if(node->nodes[m]->ch < c)
				{
						l = m + 1;
				}else
				{
						r = m - 1;
				}
		}
		return -1;
}

arTrieNode_t*	__find_node(arTrieNode_t *node, wchar_t c)
{
		int_t idx;
		AR_ASSERT(node != NULL);
		idx = __index_of_node(node, c);

		return idx == -1 ? NULL : node->nodes[idx];
}

arTrieNode_t*	__find_and_insert(arTrieNode_t *node, wchar_t c)
{
		int_t idx;
		arTrieNode_t *chd;

		idx = __index_of_node(node, c);

		if(idx != -1)
		{
				chd = node->nodes[idx];
		}else
		{
				chd = __create_node(c);
				__insert_to_node_order(node, chd);
		}
		return chd;
}



typedef arTrieNode_t	arTrie_t;

arTrie_t*		AR_CreateTrie()
{
		return __create_node(L'\0');
}


void			AR_ClearTrie(arTrie_t *trie)
{
		arTrieNode_t set = {L'\0', false, NULL,0};

		size_t i,k;

		AR_ASSERT(trie != NULL);
		
		__insert_to_node(&set, trie);

		for(i = 0; i < set.count; ++i)
		{
				arTrieNode_t	*tmp = set.nodes[i];
				for(k = 0; k < tmp->count; ++k)
				{
						__insert_to_node(&set, tmp->nodes[k]);
				}
				if(i > 0)__destroy_node(tmp);
		}

		if(set.nodes)AR_DEL(set.nodes);
		
		trie->count = 0;
		AR_DEL(trie->nodes);
		trie->nodes = NULL;
}

void			AR_DestroyTrie(arTrie_t *trie)
{
		AR_ClearTrie(trie);
		AR_DEL(trie);
}



bool	AR_InsertToTrie(arTrie_t *trie, const wchar_t *str, void *data)
{
		arTrieNode_t *curr;
		AR_ASSERT(trie != NULL && data != NULL);
		curr = trie;
		while(*str)curr = __find_and_insert(curr, *str++);

		if(curr->final)
		{
				return false;
		}else
		{
				curr->final = data;
				return true;
		}
}


bool	AR_RemoveFromTrie(arTrie_t *trie, const wchar_t *str)
{
		arTrieNode_t *curr;
		AR_ASSERT(trie != NULL);
		curr = trie;
		while(*str && curr)curr = __find_node(curr, *str++);
		
		if(curr)
		{
				if(curr->final)
				{
						curr->final = NULL;
						return true;
				}else
				{
						return false;
				}
		}else
		{
				return false;
		}
}

void*	AR_MatchTrie(const arTrie_t *trie, const wchar_t *str, bool case_intensive)
{
		const arTrieNode_t *curr;
		AR_ASSERT(trie != NULL);
		curr = trie;
		
		while(*str && curr)
		{
				wchar_t c = case_intensive ? AR_towlower(*str) : *str;
				curr = __find_node((arTrieNode_t*)curr, c);
				str++;
		}
		
		if(curr)
		{
				return curr->final;
		}else
		{
				return NULL;		
		}

}



void print_node_count(const arTrieNode_t *node)
{
		size_t i;
		//printf("node->set.count == %d\r\n", node->set.count);
		if(node->count > 8)
		{
				printf("node->set.count == %d\r\n", node->count);
				for(i = 0; i < node->count; ++i)
				{
						wprintf(L"node->nodes[%d]->ch == %c\r\n", i,node->nodes[i]->ch);
				}
				//getchar();
		}

		for(i = 0; i < node->count; ++i)
		{
				print_node_count(node->nodes[i]);
		}

}


void trie_tree_test()
{
		arTrie_t		*trie;
		size_t			i;
		size_t count;
		wchar_t buf[1024];


		const wchar_t *keys[] = 
		{
				L"baby",
				L"bachelor",
				L"badge", 
				L"jar" ,
				L"baby",
		};
		int arr[4] = {1000,10000,100000,1000000};
		//printf("sizeof(arTrie_t) == %d\r\n", sizeof(arTrie_t));
		//getchar();

		trie = AR_CreateTrie();
		srand(0);
		for(i = 0; i < sizeof(keys)/ sizeof(keys[0]); ++i)
		{
				AR_InsertToTrie(trie, keys[i], (void*)(i + 1));
		}
		
		
		count = 0;
		for(i = 0;i < 20000; ++i)
		{
				uint64_t  n = arr[AR_rand64()%4];
RE_GET:
				AR_u64tow_buf(buf, 1024, AR_rand64() % n, 10);
				if(AR_wcslen(buf) < 3)
						goto RE_GET;
				
				if(!AR_InsertToTrie(trie, buf, (void*)(i + 1)))
				{
						wprintf(L"failed == %s, %d\r\n", buf,count++);
				}
		}

		printf("match == %d\r\n", AR_MatchTrie(trie, L"JaR", true));
		//print_node_count(trie);

		
		AR_DestroyTrie(trie);
}





#endif


AR_NAMESPACE_END