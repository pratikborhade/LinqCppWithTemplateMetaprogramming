#include "TestDataCreator.h"

SUITE( SelectClauseTest )
{
	TEST( SelectUsingFunctor )
	{
		auto data = Creator<TestData>::GetInstance().GetData();
		auto& vec = Creator<TestData>::GetInstance().GetVector();
		
		auto list = data.Select( []( const TestData& data ) -> const int &{ return data.f1; } ).ToList();
		
		for( int i=0; i<vec.size(); ++i )
		{
			CHECK_EQUAL( vec[i].f1,list[i] );
		}
	}

	TEST(SelectUsingOffset)
	{
		auto data = Creator<TestData>::GetInstance().GetData();
		auto& vec = Creator<TestData>::GetInstance().GetVector();
		
		auto list = data.Select( &TestData::f2 ).ToList();
		
		for( int i=0; i<vec.size(); ++i )
		{
			CHECK_EQUAL( vec[i].f2,list[i] );
		}
	}
	
	TEST( SelectUsingFunctorReturningMod20 )
	{
		auto data = Creator<TestData>::GetInstance().GetData();
		auto& vec = Creator<TestData>::GetInstance().GetVector();
		
		auto list = data.Select( []( const TestData& data ) -> int{ return data.f1%20; } ).ToList();
		
		for( int i=0; i<vec.size(); ++i )
		{
			CHECK_EQUAL( vec[i].f1 % 20,list[i] );
		}
	}
}