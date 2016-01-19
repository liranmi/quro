/*
 * This file is released under the terms of the Artistic License.  Please see
 * the file LICENSE, included in this package, for details.
 *
 * Copyright (C) 2006 Rilson Nascimento
 *               2010 Mark Wong
 *
 * Base class for transacation classes
 * 13 June 2006
 */

#ifndef TXN_BASE_DB_H
#define TXN_BASE_DB_H

#include <string>
using namespace std;

#include "DBT5Consts.h"
using namespace TPCE;

#include "locking.h"

#ifdef WORKLOAD_TPCE
#include "DBConnection.h"
#else
//#include "DBConnection.h"
class CDBConnection;
#endif

class CTxnBaseDB
{
public:
	CDBConnection *pDB;

protected:
	void commitTransaction();
	void reconect();

	void rollbackTransaction();

	void startTransaction();

	string escape(string);
#ifdef WORKLOAD_TPCE
	void execute(const TBrokerVolumeFrame1Input *,
			TBrokerVolumeFrame1Output *);

	void execute(const TCustomerPositionFrame1Input *,
			TCustomerPositionFrame1Output *);
	void execute(const TCustomerPositionFrame2Input *,
			TCustomerPositionFrame2Output *);

	void execute(const TDataMaintenanceFrame1Input *);

	void execute(const TMarketFeedFrame1Input *, TMarketFeedFrame1Output *,
        CSendToMarketInterface *);

	void execute(const TMarketWatchFrame1Input *, TMarketWatchFrame1Output *);

	void execute(const TSecurityDetailFrame1Input *,
			TSecurityDetailFrame1Output *);

	void execute(const TTradeCleanupFrame1Input *);

	void execute(const TTradeLookupFrame1Input *, TTradeLookupFrame1Output *);
	void execute(const TTradeLookupFrame2Input *, TTradeLookupFrame2Output *);
	void execute(const TTradeLookupFrame3Input *, TTradeLookupFrame3Output *);
	void execute(const TTradeLookupFrame4Input *, TTradeLookupFrame4Output *);
#ifdef REORDER
	void execute(PTradeResultTxnInput, PTradeResultTxnOutput);
	void execute(PTradeOrderTxnInput, PTradeOrderIntermediate, PTradeOrderTxnOutput);
#else
	void execute(const TTradeOrderFrame1Input *, TTradeOrderFrame1Output *);
	void execute(const TTradeOrderFrame2Input *, TTradeOrderFrame2Output *);
	void execute(const TTradeOrderFrame3Input *, TTradeOrderFrame3Output *);
	void execute(const TTradeOrderFrame4Input *, TTradeOrderFrame4Output *);

	void execute(const TTradeResultFrame1Input *, TTradeResultFrame1Output *);
	void execute(const TTradeResultFrame2Input *, TTradeResultFrame2Output *);
	void execute(const TTradeResultFrame3Input *, TTradeResultFrame3Output *);
	void execute(const TTradeResultFrame4Input *, TTradeResultFrame4Output *);
	void execute(const TTradeResultFrame5Input *);
	void execute(const TTradeResultFrame6Input *, TTradeResultFrame6Output *);
#endif
	void execute(const TTradeStatusFrame1Input *, TTradeStatusFrame1Output *);

	void execute(const TTradeUpdateFrame1Input *, TTradeUpdateFrame1Output *);
	void execute(const TTradeUpdateFrame2Input *, TTradeUpdateFrame2Output *);
	void execute(const TTradeUpdateFrame3Input *, TTradeUpdateFrame3Output *);

#ifdef DB_PGSQL
	void setReadCommitted();
	void setReadUncommitted();
	void setRepeatableRead();
	void setSerializable();

#endif
#endif //WORKLOAD_TPCE
public:
	CTxnBaseDB(CDBConnection *pDB);
	~CTxnBaseDB();
};


#endif // TXN_BASE_DB_H