#include "media/filereadpkt.h"

e_MsRetCode FileReadSigPktInit( h_SigPkt hSigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = hSigPkt_i;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poSigPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poSigPkt->oFileReadPkt.lFileOffset = 0;
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

e_MsRetCode FileReadSigPktDelete( h_DataPkt hSigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = hSigPkt_i;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poSigPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( 0 == --poSigPkt->unRefCount )
		{
			memset( &poSigPkt->oFileReadPkt, 0, sizeof (t_FileReadPkt));
		}
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

