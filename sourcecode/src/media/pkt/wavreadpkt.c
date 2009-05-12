#include "media/wavreadpkt.h"

e_MsRetCode WavReadSigPktInit( h_SigPkt hSigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = hSigPkt_i;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poSigPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poSigPkt->oWavReadPkt.lFileOffset = 0;
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

e_MsRetCode WavReadSigPktDelete( h_DataPkt hSigPkt_i )
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
			memset( &poSigPkt->oWavReadPkt, 0, sizeof (t_WavReadPkt));
		}
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

