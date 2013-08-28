
//#define __AR_CHECK_RET_STATUS__	1

/*
#if defined(__cplusplus)
		#define __AR_CHECK_RET_STATUS__	1
#endif
*/


typedef enum 
{
		AR_S_YES = 0,
		AR_S_NO,

		AR_S_EQ,
		AR_S_NEQ,
		AR_S_LE,
		AR_S_GE,
		AR_S_EQLE,
		AR_S_EQGE,

		AR_E_NOMEM = 0x1000,
		AR_E_INVAL,
		AR_E_MALFORMAT,
		AR_E_RANGE,
		AR_E_NOTFOUND,
		AR_E_EXISTED,
		AR_E_RESERVED,
		AR_E_EMPTY,
		AR_E_NOMORE,
		AR_E_IGNORED,
		
		AR_E_SMALLBUF,
		AR_E_BADENCCONV,
		AR_E_NOTREADY,
		AR_E_NOTSUPPORTED,
		AR_E_NOTMATCHED,
        AR_E_ALREADY,
        
		AR_E_SYS,
		AR_E_FILE,
		AR_E_PATH,

		AR_E_INTR,
        AR_E_WOULDBLOCK,
        AR_E_IO,

        AR_E_PERM,
        AR_E_NOTDIR,
        AR_E_ISDIR,
        AR_E_READONLY,
        AR_E_NOTEMPTY,
        AR_E_ACCES,
        AR_E_NOTENOUGH,
        
		AR_E_NET,
		AR_E_TIMEOUT,
		AR_E_CANCELLED,

		AR_E_LEXMPTYMATCH,
		AR_E_LEXNOTREADY,

		AR_E_PSRINVTERM,

		AR_E_TGUINVOPCODE,
		AR_E_TGUINVOPRAND,
		AR_E_TGUINVOBJ,
		AR_E_TGUINVCALL,

		AR_E_ABORT,
		AR_E_FAIL
}__tag_ret_error_t;


#if !defined(__AR_CHECK_RET_STATUS__)


typedef	__tag_ret_error_t		arStatus_t;

#define AR_GET_STATUS(_status)	(_status)

#else


/*
		之所以在C++编译器下使用这个类，是因为避免出现
		if(!status)
		{

		}

		这种情况

		注意，这部分仅仅是测试使用！
*/

class arStatus_t
{
public:
		int m_code;
public:

		arStatus_t()
		{
				m_code = AR_S_YES;
		}

		explicit arStatus_t(ar_bool_t t);
		explicit arStatus_t(ar_int_32_t t);
		explicit arStatus_t(ar_int_64_t t);
		explicit arStatus_t(ar_uint_32_t t);
		explicit arStatus_t(ar_uint_64_t t);


		arStatus_t(__tag_ret_error_t t)
		{
				m_code = t;
		}


		arStatus_t(const arStatus_t &other)
		{
				m_code = other.m_code;
		}


		arStatus_t& operator=(const arStatus_t &other)
		{
				m_code = other.m_code;
				return *this;
		}

		arStatus_t& operator=(__tag_ret_error_t t)
		{
				m_code = t;
				return *this;
		}

		ar_bool_t operator==(const arStatus_t &other)
		{
				return m_code == other.m_code;
		}

		ar_bool_t operator!=(const arStatus_t &other)
		{
				return m_code != other.m_code;
		}


		ar_bool_t operator==(__tag_ret_error_t code)
		{
				return m_code == code;
		}

		ar_bool_t operator!=(__tag_ret_error_t code)
		{
				return m_code != code;
		}
};

#define AR_GET_STATUS(_status)	((_status).m_code)

#endif
