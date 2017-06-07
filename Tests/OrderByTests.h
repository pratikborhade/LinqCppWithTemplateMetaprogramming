#include "TestDataCreator.h"

SUITE( OrderByClauseTest )
{
	TEST( OrderByF1Asc )
	{
		auto data = Creator<TestData>::GetInstance().GetData();
		auto ordered = data.OrderBy( &TestData::f1 );
		
		auto last = *ordered;
		++ordered;
		auto count = 0;
		for( auto iter = ordered; iter != ordered.end(); ++iter )
		{
			auto &val = *iter;
			CHECK_EQUAL( last.f1 <= val.f1, true ); 
			last = val;
			++count;
		}
	}
	
	TEST( OrderByF2Desc )
	{
		auto data = Creator<TestData>::GetInstance().GetData();
		auto ordered = data.OrderBy( &TestData::f2, OrderByType::desc );
		
		auto last = *ordered;
		++ordered;
		for( auto val: ordered )
		{
			CHECK_EQUAL( last.f2 >= val.f2, true ); 
			last = val;
		}
	}
	
	TEST( OrderByF1AscF2Desc )
	{
		auto data = Creator<TestData>::GetInstance().GetData();
		auto ordered = data.OrderBy( &TestData::f1, OrderByType::asc, &TestData::f2, OrderByType::desc );
		
		auto last = *ordered;
		++ordered;
		for( auto val: ordered )
		{
			CHECK_EQUAL( last.f1 <= val.f1, true ); 
			if( last.f1 == val.f1 )
				CHECK_EQUAL( last.f2 >= val.f2, true );
			last = val;
		}
	}
}