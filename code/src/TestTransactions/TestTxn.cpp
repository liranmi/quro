// 2006 Rilson Nascimento
// Test transactions

#include <transactions.h>
#include <CETxnInputGenerator.h>
#include <cstdlib>
#include <TxnHarnessSendToMarketTest.h>
#include <DMSUTtest.h>

using namespace TPCE;

eTxnType TxnType = TRADE_STATUS;
RNGSEED	Seed = 0;

// shows program usage
void Usage()
{
	cout<<"\nUsage: TestTxn [option]"<<endl<<endl;
	cout<<"  where"<<endl;
	cout<<"   Option		Description"<<endl;
	cout<<"   =========		========================"<<endl;
	cout<<"   -r number		optional random number"<<endl;
	cout<<"   -t number		Transaction type"<<endl;
	cout<<"\t\t\tA - TRADE_ORDER"<<endl;
	cout<<"\t\t\t    TRADE_RESULT"<<endl;
	cout<<"\t\t\t    MARKET_FEED"<<endl;
	cout<<"\t\t\tC - TRADE_LOOKUP"<<endl;
	cout<<"\t\t\tD - TRADE_UPDATE"<<endl;
	cout<<"\t\t\tE - TRADE_STATUS (default)"<<endl;
	cout<<"\t\t\tF - CUSTOMER_POSITION"<<endl;
	cout<<"\t\t\tG - BROKER_VOLUME"<<endl;
	cout<<"\t\t\tH - SECURITY_DETAIL"<<endl;
	cout<<"\t\t\tJ - MARKET_WATCH"<<endl;
	cout<<"\t\t\tK - DATA_MAINTENANCE"<<endl;
	cout<<"\t\t\tL - TRADE_CLEANUP"<<endl<<endl;
	cout<<"Note: Trade Order triggers Trade Result and Market Feed"<<endl
	    <<"      when the type of trade is Market (type_is_market=1)"<<endl;
}


// parse command line
bool ParseCommandLine( int argc, char *argv[] )
{
	int   arg;
	char  *sp;
	char  *vp;

	if (argc < 2) 
	{
		// use default
		return(true);
	}

	/*
	*  Scan the command line arguments
	*/
	for ( arg = 1; arg < argc; ++arg ) {
		/*
		*  Look for a switch 
		*/
		sp = argv[arg];
		if ( *sp == '-' ) 
		{
			++sp;
		}
		*sp = (char)tolower( *sp );
		
		/*
		*  Find the switch's argument.  It is either immediately after the
		*  switch or in the next argv
		*/
		vp = sp + 1;
			// Allow for switched that don't have any parameters.
			// Need to check that the next argument is in fact a parameter
			// and not the next switch that starts with '-'.
			//
		if ( (*vp == 0) && ((arg + 1) < argc) && (argv[arg + 1][0] != '-') )
		{
			vp = argv[++arg];
		}
		
		/*
		*  Parse the switch
		*/
		switch ( *sp ) {
		case 't':
			switch ( *vp) {
			case 'A':
				TxnType = TRADE_ORDER;
				break;
			case 'B':
				TxnType = TRADE_RESULT;
				break;
			case 'C':
				TxnType = TRADE_LOOKUP;
				break;
			case 'D': 
				TxnType = TRADE_UPDATE;
				break;
			case 'E':
				TxnType = TRADE_STATUS;
				break;
			case 'F':
				TxnType = CUSTOMER_POSITION;
				break;
			case 'G':
				TxnType = BROKER_VOLUME;
				break;
			case 'H':
				TxnType = SECURITY_DETAIL;
				break;
			case 'I':
				TxnType = MARKET_FEED;
				break;
			case 'J':
				TxnType = MARKET_WATCH;
				break;
			case 'K':
				TxnType = DATA_MAINTENANCE;
				break;
			case 'L':
				TxnType = TRADE_CLEANUP;
				break;
			default:
				return(false);
			}
			break;
		case 'r':
			sscanf(vp, "%d", &Seed);
			break;
		default:
			return(false);
		}
	}
	return(true);
}

// Trade Order
void TradeOrder(CDBConnection* pConn, CCETxnInputGenerator* pTxnInputGenerator)
{
	// SendToMarket test class that can call Trade-Result and Market-Feed 
	// via the MEE - Market Exchange Emulator when type_is_market = 1. 
	// These two txns run async.
	CSendToMarketTest	m_pSendToMarket;

	// trade order harness code (TPC provided)
	// this class uses our implementation of CTradeOrderDB class
	CTradeOrder		m_TradeOrder(pConn, &m_pSendToMarket);

	// trade order input/output parameters
	TTradeOrderTxnInput	m_TradeOrderTxnInput;
	TTradeOrderTxnOutput	m_TradeOrderTxnOutput;
	
	// using TPC-provided input generator class
	bool	bExecutorIsAccountOwner;
	INT32	iTradeType;
	pTxnInputGenerator->GenerateTradeOrderInput( m_TradeOrderTxnInput, iTradeType, bExecutorIsAccountOwner );

	m_TradeOrder.DoTxn(&m_TradeOrderTxnInput, &m_TradeOrderTxnOutput);	// Perform Trade Order
}


// Trade Status
void TradeStatus(CDBConnection* pConn, CCETxnInputGenerator* pTxnInputGenerator)
{
	// trade status harness code (TPC provided)
	// this class uses our implementation of CTradeStatusDB class
	CTradeStatus		m_TradeStatus(pConn);

	// trade status input/output parameters
	TTradeStatusTxnInput	m_TradeStatusTxnInput;
	TTradeStatusTxnOutput	m_TradeStatusTxnOutput;
	
	// using TPC-provided input generator class
	pTxnInputGenerator->GenerateTradeStatusInput( m_TradeStatusTxnInput );

	m_TradeStatus.DoTxn(&m_TradeStatusTxnInput, &m_TradeStatusTxnOutput);	// Perform Trade Status
}


// Trade Lookup
void TradeLookup(CDBConnection* pConn, CCETxnInputGenerator* pTxnInputGenerator)
{
	// trade lookup harness code (TPC provided)
	// this class uses our implementation of CTradeLookupDB class
	CTradeLookup		m_TradeLookup(pConn);

	// trade lookup input/output parameters
	TTradeLookupTxnInput	m_TradeLookupTxnInput;
	TTradeLookupTxnOutput	m_TradeLookupTxnOutput;
	
	// using TPC-provided input generator class
	pTxnInputGenerator->GenerateTradeLookupInput( m_TradeLookupTxnInput );

	m_TradeLookup.DoTxn(&m_TradeLookupTxnInput, &m_TradeLookupTxnOutput);	// Perform Trade Lookup
}


// Trade Update
void TradeUpdate(CDBConnection* pConn, CCETxnInputGenerator* pTxnInputGenerator)
{
	// trade update harness code (TPC provided)
	// this class uses our implementation of CTradeUpdateDB class
	CTradeUpdate		m_TradeUpdate(pConn);

	// trade update input/output parameters
	TTradeUpdateTxnInput	m_TradeUpdateTxnInput;
	TTradeUpdateTxnOutput	m_TradeUpdateTxnOutput;
	
	// using TPC-provided input generator class
	pTxnInputGenerator->GenerateTradeUpdateInput( m_TradeUpdateTxnInput );

	m_TradeUpdate.DoTxn(&m_TradeUpdateTxnInput, &m_TradeUpdateTxnOutput);	// Perform Trade Update
}


// Customer Position
void CustomerPosition(CDBConnection* pConn, CCETxnInputGenerator* pTxnInputGenerator)
{
	// customer position harness code (TPC provided)
	// this class uses our implementation of CCustomerPositionDB class
	CCustomerPosition		m_CustomerPosition(pConn);

	// customer position input/output parameters
	TCustomerPositionTxnInput	m_CustomerPositionTxnInput;
	TCustomerPositionTxnOutput	m_CustomerPositionTxnOutput;
	
	// using TPC-provided input generator class
	pTxnInputGenerator->GenerateCustomerPositionInput( m_CustomerPositionTxnInput );

	m_CustomerPosition.DoTxn(&m_CustomerPositionTxnInput, &m_CustomerPositionTxnOutput);	// Perform Customer Position
}


// Broker Volume
void BrokerVolume(CDBConnection* pConn, CCETxnInputGenerator* pTxnInputGenerator)
{
	// Broker Volume harness code (TPC provided)
	// this class uses our implementation of CBrokerVolumeDB class
	CBrokerVolume		m_BrokerVolume(pConn);

	// broker volume input/output parameters
	TBrokerVolumeTxnInput	m_BrokerVolumeTxnInput;
	TBrokerVolumeTxnOutput	m_BrokerVolumeTxnOutput;
	
	// using TPC-provided input generator class
	pTxnInputGenerator->GenerateBrokerVolumeInput( m_BrokerVolumeTxnInput );

	m_BrokerVolume.DoTxn(&m_BrokerVolumeTxnInput, &m_BrokerVolumeTxnOutput);	// Perform Broker Volume
}


// Security Detail
void SecurityDetail(CDBConnection* pConn, CCETxnInputGenerator* pTxnInputGenerator)
{
	// Security Detail harness code (TPC provided)
	// this class uses our implementation of CSecurityDetailDB class
	CSecurityDetail		m_SecurityDetail(pConn);

	// security detail input/output parameters
	TSecurityDetailTxnInput	m_SecurityDetailTxnInput;
	TSecurityDetailTxnOutput	m_SecurityDetailTxnOutput;
	
	// using TPC-provided input generator class
	pTxnInputGenerator->GenerateSecurityDetailInput( m_SecurityDetailTxnInput );

	m_SecurityDetail.DoTxn(&m_SecurityDetailTxnInput, &m_SecurityDetailTxnOutput);	// Perform Security Detail
}


// Market Watch
void MarketWatch(CDBConnection* pConn, CCETxnInputGenerator* pTxnInputGenerator)
{
	// Market Watch harness code (TPC provided)
	// this class uses our implementation of CMarketWatchDB class
	CMarketWatch		m_MarketWatch(pConn);

	// Market Watch input/output parameters
	TMarketWatchTxnInput	m_MarketWatchTxnInput;
	TMarketWatchTxnOutput	m_MarketWatchTxnOutput;
	
	// using TPC-provided input generator class
	pTxnInputGenerator->GenerateMarketWatchInput( m_MarketWatchTxnInput );

	m_MarketWatch.DoTxn(&m_MarketWatchTxnInput, &m_MarketWatchTxnOutput);	// Perform Market Watch
}


// Data Maintenance
void DataMaintenance(CDM* pCDM)
{
	// using TPC-provided Data Maintenance class to perform the Data Maintenance transaction.
	// Testing all tables
	for (int i = 0; i <= 11; i++)
	{
		pCDM->DoTxn();
	}
}


// Trade Cleanup
void TradeCleanup(CDM* pCDM)
{
	// using TPC-provided Data Maintenance class to perform the Trade Cleanup.
	pCDM->DoCleanupTxn();
}

// Auto Random number generator
unsigned int AutoRng()
{
	struct timeval tv;
	struct tm ltr;
	gettimeofday(&tv, NULL);
	struct tm* lt = localtime_r(&tv.tv_sec, &ltr);
	return (((lt->tm_hour * MinutesPerHour + lt->tm_min) * SecondsPerMinute + lt->tm_sec) * MsPerSecond + tv.tv_usec / 1000);
}

// main
int main(int argc, char* argv[])
{
	const char *server = "localhost";
	const char *db = "dbt5";
	const char *port = "5432";

	if (!ParseCommandLine(argc, argv))
	{
		Usage();
		return(-1);
	}

	try
	{
		// database connection
		CDBConnection		m_Conn( server, db, port );
	
		// initialize Input Generator
		//
		CLogFormatTab fmt;
		CEGenLogger log(eDriverEGenLoader, 0, "TxnTest.log", &fmt);
	
		char*	szInDir = "EGen_v3.14/flat_in";
	
		CInputFiles	inputFiles;
		inputFiles.Initialize(eDriverEGenLoader, iDefaultLoadUnitSize, iDefaultLoadUnitSize, szInDir);
	
		TDriverCETxnSettings	m_DriverCETxnSettings;
	
		CCETxnInputGenerator	m_TxnInputGenerator(inputFiles, iDefaultLoadUnitSize, iDefaultLoadUnitSize, 
								500, 10*HoursPerWorkDay, &log, &m_DriverCETxnSettings);
	
		if (Seed == 0) 
		{
			srand(AutoRng());
			Seed = rand();
		}
		cout<<"Seed: "<<Seed<<endl<<endl;
		m_TxnInputGenerator.SetRNGSeed( Seed );

		// Initialize DM - Data Maintenance class
		// DM is used by Data-Maintenance and Trade-Cleanup transactions
		CDMSUTtest	m_CDMSUT( &m_Conn );	// Data-Maintenance SUT interface (provided by us)
		CDM		m_CDM( &m_CDMSUT, &log, inputFiles, iDefaultLoadUnitSize, iDefaultLoadUnitSize,
						10, 500, 1 );	// provided by TPC

		//  Parse Txn type
		switch ( TxnType ) 
		{
			case TRADE_ORDER:
			case TRADE_RESULT:
			case MARKET_FEED:
				cout<<"=== Testing Trade Order, Trade Result and Market Feed ==="<<endl<<endl;
				TradeOrder( &m_Conn, &m_TxnInputGenerator );
				break;
			case TRADE_LOOKUP:
				cout<<"=== Testing Trade Lookup ==="<<endl<<endl;
				TradeLookup( &m_Conn, &m_TxnInputGenerator );
				break;
			case TRADE_UPDATE:
				cout<<"=== Testing Trade Update ==="<<endl<<endl;
				TradeUpdate( &m_Conn, &m_TxnInputGenerator );
				break;
			case TRADE_STATUS:
				cout<<"=== Testing Trade Status ==="<<endl<<endl;
				TradeStatus( &m_Conn, &m_TxnInputGenerator );
				break;
			case CUSTOMER_POSITION:
				cout<<"=== Testing Customer Position ==="<<endl<<endl;
				CustomerPosition( &m_Conn, &m_TxnInputGenerator );
				break;
			case BROKER_VOLUME:
				cout<<"=== Testing Broker Volume ==="<<endl<<endl;
				BrokerVolume( &m_Conn, &m_TxnInputGenerator );
				break;
			case SECURITY_DETAIL:
				cout<<"=== Testing Security Detail ==="<<endl<<endl;
				SecurityDetail( &m_Conn, &m_TxnInputGenerator );
				break;
			case MARKET_WATCH:
				cout<<"=== Testing Market Watch ==="<<endl<<endl;
				MarketWatch( &m_Conn, &m_TxnInputGenerator );
				break;
			case DATA_MAINTENANCE:
				cout<<"=== Testing Data Maintenance ==="<<endl<<endl;
				DataMaintenance( &m_CDM );
				break;
			case TRADE_CLEANUP:
				cout<<"=== Testing Trade Cleanup ==="<<endl<<endl;
				TradeCleanup( &m_CDM );
				break;
			default:
				cout<<"wrong txn type"<<endl;
				return(-1);
		}
	}
	catch (CBaseErr *pErr)
	{
		cout<<endl<<"Error "<<pErr->ErrorNum()<<": "<<pErr->ErrorText();
		if (pErr->ErrorLoc()) {
			cout<<" at "<<pErr->ErrorLoc()<<endl;
		} else {
			cout<<endl;
		}
		return(1);
	}
	// exceptions thrown by pqxx
	//
	catch (const pqxx::broken_connection &e) // broken connection
	{
		cout<<"libpxx: "<<e.what()<<endl;
		return 3;
	}
	catch (const pqxx::sql_error &e)
	{
		cout<<"SQL error: "<<e.what()<<endl
		    <<"Query was: '"<<e.query()<<"'"<<endl;
		return 3;
	}
	catch (const exception &e)
	{
		cout<<e.what()<<endl;
		return 3;
	}

	pthread_exit(NULL);
	return(0);
}
