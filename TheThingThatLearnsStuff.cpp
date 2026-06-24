#include <iostream>
#include <tuple>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <thread>
#include <chrono>


const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string RESET = "\033[0m";

using Data = std::tuple<std::tuple<int, int>, int>;
std::vector<Data> data = {};

double sigmoid(double x)
{
	return 1.0 / (1.0 + std::exp(-x));
}

double lr = 0.1;
double conf = 0.50;
double rfe = 2;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dist(-1, rfe);

std::vector<double> brain = {dist(gen), dist(gen), dist(gen)};


bool stuff()
{	
	double w1 = brain[0];
	double w2 = brain[1];
	double bias = brain[2];
	bool wac;
	wac = true;
	int pred;

	for (auto& dat : data)
	{
		auto inputs = std::get<0>(dat);
		int expected = std::get<1>(dat);
		int a = std::get<0>(inputs);
		int b = std::get<1>(inputs);
		double mut_a;
		double mut_b;
		mut_a = a * w1;
		mut_b = b * w2;
		double output;
		output = mut_a + mut_b + bias;
		output = sigmoid(output);
		if (output >= conf)
		{
			pred = 1;
		}
		else
		{
			pred = 0;
		}
		if (pred == expected)
		{
			std::cout << YELLOW << "Prediction matches expected output" << RESET << "\n";
		}
		else
		{
			int off;
			off = pred - expected;
			double showoff;
			showoff = output - expected;
			w1 = w1 - lr * off * a;
			w2 = w2 - lr * off * b;
			bias = bias - lr * off;
			std::cout << RED << "Ouput does not match, off by " << showoff << RESET << "\n";
			wac = false;
		}
	}
	brain[0] = w1;
	brain[1] = w2;
	brain[2] = bias;
	bool rv;
	rv = wac;
	return rv;
}

int askAI(int a, int b)
{
	double output = sigmoid(a * brain[0] + b * brain[1] + brain[2]);
	int pred = (output >= 0.5) ? 1 : 0;
	return pred;
}

int main()
{
	for (int i = 0; i < 4; i++)
	{
		std::string astr;
		std::cout << "A: ";
		std::getline(std::cin, astr);
		std::string bstr;
		std::cout << "B: ";
		std::getline(std::cin, bstr);
		std::string excstr;
		std::cout << "Expected: ";
		std::getline(std::cin, excstr);
		int a;
		int b;
		int exc;
		a = std::stoi(astr);
		b = std::stoi(bstr);
		exc = std::stoi(excstr);
		data.push_back(Data{ {a, b}, exc });
		std::cout << "-----------\n";
	}
	bool correct;
	correct = false;
	while (!correct)
	{
		auto outp = stuff();
		if (outp)
		{
			for (auto& dat : data)
			{
				auto inputs = std::get<0>(dat);
				int expected = std::get<1>(dat);
				int a = std::get<0>(inputs);
				int b = std::get<1>(inputs);
				int res;
				res = askAI(a, b);
				if (res == expected)
				{
					correct = true;
					break;
				}
				else
				{
					correct = false;
					break;
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	std::cout << GREEN << "-----------" << RESET << "\n";
	std::cout << GREEN << "Succesfully recognised pattern from data" << RESET << "\n";
	std::cout << GREEN << "-----------" << RESET << "\n";
	bool RUNNING = true;

	while (RUNNING)
	{
		std::string astr;
		std::string bstr;

		std::cout << "A: ";
		std::getline(std::cin, astr);

		std::cout << "B: ";
		std::getline(std::cin, bstr);

		if (astr == "exit" || bstr == "exit")
		{
			RUNNING = false;
			continue;
		}

		int a = std::stoi(astr);
		int b = std::stoi(bstr);

		int pred = askAI(a, b);

		std::cout << "Prediction: " << pred << "\n";
	}
}
