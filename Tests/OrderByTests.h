#include "TestDataCreator.h"

SUITE( OrderByClauseTest )
{
	TEST( OrderByF1Asc )
	{
		auto data = Creator<TestData>::GetInstance().GetData();
		auto ordered = data.OrderBy( &TestData::f1 );
		
		auto last = *ordered;
		++ordered;
		for( auto val: ordered.ToList() )
		{
			CHECK_ASSERT( last.f1 <= val.f1 ); 
			last = val;
		}
	}
	
	TEST( OrderByF2Desc )
	{
		auto &data = Creator<TestData>::GetInstance().GetData();
		auto ordered = data.OrderBy( &TestData::f2, OrderByType::desc );
		
		auto last = *ordered;
		++ordered;
		for( auto val: ordered )
		{
			CHECK_ASSERT( last.f2 >= val.f2 ); 
			last = val;
		}
	}
	
	TEST( OrderByF1AscF2Desc )
	{
	}
}