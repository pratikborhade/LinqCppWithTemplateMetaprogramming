#include "TestDataCreator.h"

SUITE( WhereClauseTests )
{
	TEST( TestIfWhereFiltersData )
	{
		auto data = Creator<TestData>::GetInstance().GetData();
		
		auto list = data.Where( []( const TestData& data ) ->bool { return data.f1 > 50; } ).ToList();
		
		for( auto &elem : list )
		{
			CHECK_EQUAL( elem.f1 > 50, true );
		}
	}
	
	TEST( TestIfWhereFiltersDataInFloat )
	{
		auto data = Creator<TestData>::GetInstance().GetData();
		
		auto list = data.Where( []( const TestData& data ) ->bool { return data.f2 < 50; } ).ToList();
		
		for( auto &elem : list )
		{
			CHECK_EQUAL( elem.f2 < 50, true );
		}
	}
}