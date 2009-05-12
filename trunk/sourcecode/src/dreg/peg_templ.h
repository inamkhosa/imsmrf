/* templated version of Parsing Expression Grammar implementation
implements the following operations:
[A-Za-z] ;	'for' ; e1 e2 ; e1 / e2 ;  e? ; e* ; e+ ; e{low,high} ; &e ; !e ; .
the ::Matches(it,pC) functions expect an iterator it with  the operations:
int iterator::operator*(); iterator& iterator::operator++();
iterator values returned by *it must always be integers >=0.
The implementation uses negative values internally to cope with default
arguments.

Caution: This is an implementation for educational purposes.
Performance improvements using template metagprogramming should be possible.

*/
#ifndef PEGTEMPL_H_
#define PEGTEMPL_H_

#include <limits.h>

namespace peg_templ {
template<int CHARVALUE>
struct Char
{
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		return (* p == CHARVALUE ? (++p, true) : false);
	}
};
template < int LOWER, int UPPER,
          int LOWER2 = -1, int UPPER2 = -1,
          int LOWER3 = -1, int UPPER3 = -1,
          int LOWER4 = -1, int UPPER4 = -1 >
struct In
{
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		if( * p >= LOWER && * p <= UPPER )
		{
			++p;
			return (true);
		}
		if( LOWER2 == -1 )
		{
			return (false);
		}
		if( *p >= LOWER2 && *p <= UPPER2 )
		{
			++p;
			return (true);
		}
		if( LOWER3 == -1 )
		{
			return (false);
		}
		if( *p >= LOWER3 && *p <= UPPER3 )
		{
			++p;
			return (true);
		}
		if( LOWER4 == -1 )
		{
			return (false);
		}
		if( *p >= LOWER4 && *p <= UPPER4 )
		{
			++p;
			return (true);
		}
		return (false);
	}
};

template<int LOWER, int UPPER, typename T>
struct For
{
	enum EMinMaxFor { eMinFor = LOWER, eMaxFor = UPPER };
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		It pStart = p;
		int i;

		for( i = 0; i <= UPPER; i++ )
		{
			if( !T::Matches( p, pC ) )
			{
				break;
			}
		}
		return ( (i >= LOWER) ? (true) : (p = pStart, false));
	}
};
template<typename T>
struct Option
{
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		(void)T::Matches( p, pC );
		return (true);
	}
};
template<typename T>
struct OptRepeat
{
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		while( T::Matches( p, pC ))
		{
		}
		return (true);
	}
};
template<typename T>
struct PlusRepeat
{
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		if( !T::Matches( p, pC ))
		{
			return (false);
		}

		while( T::Matches( p, pC ))
		{
		}
		return (true);
	}
};
struct MatchTrue
{
	enum EMinMax { eMinChar = INT_MAX, eMaxChar = INT_MIN };
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		return (true);
	}
};
struct MatchFalse
{
	enum EMinMax { eMinChar = INT_MAX, eMaxChar = INT_MIN };
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		return (false);
	}
};

template < class T0, class T1,
          class T2 = MatchFalse, class T3 = MatchFalse, class T4 = MatchFalse,
          class T5 = MatchFalse, class T6 = MatchFalse, class T7 = MatchFalse
>
struct Or
{
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		return (T0::Matches( p, pC ) || T1::Matches( p, pC )
		        ||  T2::Matches( p, pC ) || T3::Matches( p, pC )
		        || T4::Matches( p, pC )  || T5::Matches( p, pC )
		        || T6::Matches( p, pC )  || T7::Matches( p, pC ));
	}
};
template < int C0, int C1, int C2 = INT_MIN, int C3 = INT_MIN, int C4 = INT_MIN,
          int C5 = INT_MIN, int C6 = INT_MIN, int C7 = INT_MIN >
struct OneOfChars
{
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		if( * p == C0 || * p == C1 )
		{
			++p;
			return (true);
		}
		if( C2 == INT_MIN )
		{
			return (false);
		}
		if( *p == C2 )
		{
			++p;
			return (true);
		}
		if( C3 == INT_MIN )
		{
			return (false);
		}
		if( *p == C3 )
		{
			++p;
			return (true);
		}
		if( C4 == INT_MIN )
		{
			return (false);
		}
		if( *p == C4 )
		{
			++p;
			return (true);
		}
		if( C5 == INT_MIN )
		{
			return (false);
		}
		if( *p == C5 )
		{
			++p;
			return (true);
		}
		if( C6 == INT_MIN )
		{
			return (false);
		}
		if( *p == C6 )
		{
			++p;
			return (true);
		}
		if( C7 == INT_MIN )
		{
			return (false);
		}
		if( *p == C7 )
		{
			++p;
			return (true);
		}
	}
};

template < typename T0,           typename T1,
          typename T2 = MatchTrue, typename T3 = MatchTrue,
          typename T4 = MatchTrue, typename T5 = MatchTrue,
          typename T6 = MatchTrue, typename T7 = MatchTrue >
struct And
{
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		It pStart = p;
		if(     T0::Matches( p, pC ) && T1::Matches( p, pC )
		        &&  T2::Matches( p, pC ) && T3::Matches( p, pC )
		        && T4::Matches( p, pC )  && T5::Matches( p, pC )
		        && T6::Matches( p, pC ) && T7::Matches( p, pC ))
		{
			return (true);
		}
		else
		{
			p = pStart;
			return (false);
		}
	}
};
template < int C0,         int C1 = INT_MIN, int C2 = INT_MIN, int C3 = INT_MIN,
          int C4 = INT_MIN, int C5 = INT_MIN, int C6 = INT_MIN, int C7 = INT_MIN >
struct String
{
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		if( !(* p == C0) )
		{
			return (false);
		}
		It pStart = p;
		++p;
		if( C1 == INT_MIN )
		{
			return (true);
		}
		if( !(*p == C1) )
		{
			p = pStart;
			return (false);
		}
		++p;
		if( C2 == INT_MIN )
		{
			return (true);
		}
		if( !(*p == C2) )
		{
			p = pStart;
			return (false);
		}
		++p;
		if( C3 == INT_MIN )
		{
			return (true);
		}
		if( !(*p == C3) )
		{
			p = pStart;
			return (false);
		}
		++p;
		if( C4 == INT_MIN )
		{
			return (true);
		}
		if( !(*p == C4) )
		{
			p = pStart;
			return (false);
		}
		++p;
		if( C5 == INT_MIN )
		{
			return (true);
		}
		if( !(*p == C5) )
		{
			p = pStart;
			return (false);
		}
		++p;
		if( C6 == INT_MIN )
		{
			return (true);
		}
		if( !(*p == C6) )
		{
			p = pStart;
			return (false);
		}
		++p;
		if( C7 == INT_MIN )
		{
			return (true);
		}
		if( !(*p == C7) )
		{
			p = pStart;
			return (false);
		}
		++p;
		return (true);
	}
};
template<typename T>
struct Not
{
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		It p0 = p;

		return (!T::Matches( p0, pC ));
	}
};
template<typename T>
struct Peek
{
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		It p0 = p;

		return (T::Matches( p0, pC ));
	}
};
template<int nToSkip>
struct Advance
{
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		for( int i = 0; i < nToSkip; i++ )
		{
			++p;
		}
		return (true);
	}
};

template<typename Saver, typename T>
struct Group
{
	template<typename It, typename Context>
	static bool Matches( It&p, Context* pC )
	{
		It p0 = p;
		bool b = T::Matches( p, pC );

		Saver::Save( pC, p0, p, b );
		return (b);
	}
};
};
#endif

