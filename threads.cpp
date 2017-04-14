// ConsoleApplication20.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <gtest\gtest.h>
#include <future>
#include <memory>
void function1()
{
	int j = 0;
	for (unsigned int i = 0; i < 100000000; ++i)
	{
		j = i;
		j++;
		--j;
		j++;
		j--;
	}
}
void function2()
{
	int j = 0;
	for (unsigned int i = 0; i < 100000000; ++i)
	{
		j = i + 1000;
		j++;
		--j;
		j++;
		j--;
	}
}
std::vector<int> function3()
{
	std::vector<int> vec;
	for (unsigned int i = 0; i < 1000000; ++i)
		vec.push_back(i);
	return vec;
}
void function3T(std::promise<std::vector<int>> && p)
{
	std::vector<int> vec;
	for (unsigned int i = 0; i < 1000000; ++i)
		vec.push_back(i);
	p.set_value(vec);
}

void functionWithThreads()
{
	std::thread t1(function1);
	std::thread t2(function2);
	std::promise<std::vector<int> > p;
	auto f = p.get_future();
	std::thread t(&function3T, std::move(p));
	t1.join();
	t2.join();
	t.join();
	std::vector<int> vec = f.get();
}

void functionWithoutThreads()
{
	function1();
	function2();
	auto vec = function3();
}

void functionWithAsync()
{
	std::async(std::launch::async, function1);
	std::async(std::launch::async, function2);
	auto fut = std::async(std::launch::async, function3);
	auto vec = fut.get();
}


int main(int argc, char **argv)  // simple example when multithreading are better then sequence programms. Async is good for get value, but it's probably slower then normall function without multithreading
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(functionWithThr, schouldExecuteFunction)
{
	functionWithThreads();
}
TEST(functionWithoutThr, schouldExecuteFunction)
{
	functionWithoutThreads();
}

TEST(functionWithAsc, schouldExecuteFunction)
{
	functionWithAsync();
}
