#include "test.h"


AR_NAMESPACE_BEGIN




struct __arsenal_uri_tag;
typedef struct __arsenal_uri_tag		arURI_t;

struct __arsenal_uri_tag
{
		arString_t		*scheme;
		arString_t		*user_info;
		arString_t		*host;
		uint_16_t		port;
		arString_t		*path;
		arString_t		*query;
		arString_t		*fragment;
};


arURI_t*		AR_CreateURI(const wchar_t *uri);
void			AR_DestroyURI(arURI_t *uri);
void			AR_ClearURI(arURI_t *uri);
int_t			AR_CompURI(const arURI_t *l, const arURI_t *r);

arStatus_t		AR_NormalizeURI(arURI_t *uri);
arStatus_t		AR_URIToString(const arURI_t *uri, arString_t *str);

const wchar_t*	AR_GetURIScheme(const arURI_t *uri);
arStatus_t		AR_SetURIScheme(arURI_t *uri, const wchar_t *str);

const wchar_t*	AR_GetURIUserInfo(const arURI_t *uri);
arStatus_t		AR_SetURIUserInfo(arURI_t *uri, const wchar_t *str);

const wchar_t*	AR_GetURIHost(const arURI_t *uri);
arStatus_t		AR_SetURIHost(arURI_t *uri, const wchar_t *str);

uint_16_t		AR_GetURIPort(const arURI_t *uri);
void			AR_SetURIPort(arURI_t *uri, uint_16_t port);

const wchar_t*	AR_GetURIAuthority(const arURI_t *uri);
arStatus_t		AR_SetURIAuthority(arURI_t *uri, const wchar_t *str);

const wchar_t*	AR_GetURIPath(const arURI_t *uri);
arStatus_t		AR_SetURIPath(arURI_t *uri, const wchar_t *str);

const wchar_t*	AR_GetURIRawQuery(const arURI_t *uri);
arStatus_t		AR_SetURIRawQuery(arURI_t *uri, const wchar_t *str);

arStatus_t		AR_GetURIQuery(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIQuery(arURI_t *uri, const wchar_t *str);


const wchar_t*	AR_GetURIFragment(const arURI_t *uri);
arStatus_t		AR_SetURIFragment(arURI_t *uri, const wchar_t *str);


/*

	void setPathEtc(const std::string& pathEtc);
		/// Sets the path, query and fragment parts of the URI.
		
	std::string getPathEtc() const;
		/// Returns the path, query and fragment parts of the URI.
		
	std::string getPathAndQuery() const;
		/// Returns the path and query parts of the URI.	
*/
AR_NAMESPACE_END
