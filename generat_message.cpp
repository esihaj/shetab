#include <string>
#include <iostream>
#include <fstream>
#include "message_specifics.h"
using namespace std;

Acquirer read_acquirer_info();
Message read_transaction_data(Acquirer acquirer);
string generate_message(Message tr);
string encode_message(string message);
string hex(char num);//1)aslan tabe bashe? 2) esmesh chi baseh?
string strip_string(string num);
string ullong_to_str(unsigned long long num);//implemented for 2 digits only 
unsigned long long str_to_ullong(string str);
string pad_zero(string str, int length);//not implemented yet
string generate_bit_pattern();//in ha behtare ke ba const refrence pas bedam chon obj e sangini e!

int     main()
{
	Acquirer acquirer_info = read_acquirer_info();
	string user_input;
	cin >> user_input;
	while (user_input != "$")
	{
		Message transaction = read_transaction_data(acquirer_info);
		transaction.ISO_bit_pattern = generate_bit_pattern();
		transaction.message = generate_message(transaction);
		transaction.message = encode_message(transaction.message);
		cout << transaction.message << endl;
		cin >> user_input;
	}
	return 0;
}
string  generate_bit_pattern()
{
	return "35363738303038313830303030303030";
}
string  strip_string(string num)
{
	for (unsigned int i = 0; i < num.length(); i++)
	{
		if (num[i] == '-')
		{
			num.erase(num.begin() + i);
			i--;
		}
	}
	return num;
}
string  generate_message(Message tr)
{
	string msg;
	msg = tr.ISO_message_type + tr.ISO_bit_pattern +
		tr.P2 + tr.P4 + tr.P6 + tr.P7 + tr.acquirer.P10 +
		tr.P11 + tr.P12 + tr.P13 + tr.P14 +
		tr.acquirer.P25 + tr.acquirer.P32 + tr.P33;
	return msg;
}
string  hex(char num)
{
	return (string("3")) + num;
}
string  encode_message(string message)
{
	string ascii;//init lazem dare ?
	for (unsigned int i = 0; i < message.length(); i++)
		ascii += hex(message[i]);
	return ascii;
}
string  pad_zero(string str, int length)
{
	if (str.length() >= length)//if len(str) >= specified len => return without change;!?
		return str;
	while (str.length() != length)
	{
		str = "0" + str;
	}
	return str;
}
string  ullong_to_str(unsigned long long num)
{
	string tmp, str;
	if (num == 0)
		return string("0");
	while (num != 0)
	{
		tmp.push_back(num % 10 + '0');
		num /= 10;
	}
	for (int i = tmp.length() - 1; i >= 0; i--)
	{
		str += tmp[i];
	}
	return str;
}
unsigned long long str_to_ullong(string str)
{
	unsigned long long num = 0;
	unsigned int len = str.length();
	for (unsigned int i = 0; i < len; i++)
		num = num * 10 + str[i] - '0';
	return num;
}
Acquirer read_acquirer_info()
{
	Acquirer acquirer;
	string input_buff;
	ifstream input;
	input.open("acquirer.info");

	//P25: Device Type
	input >> input_buff;
	if (input_buff == "ATM")
		acquirer.P25 = "02";
	else if (input_buff == "POS")
		acquirer.P25 = "14";
	else if (input_buff == "INTERNET")
		acquirer.P25 = "59";

	//Acquirer ID
	input >> input_buff;
	acquirer.P32 = pad_zero(ullong_to_str(input_buff.length()), _2_DIGITS);
	acquirer.P32 += input_buff;

	//exchange rate
	input >> input_buff;
	acquirer.P10 = pad_zero(input_buff, P10_MAX);
	input.close();
	return acquirer;
}
Message read_transaction_data(Acquirer acquirer)
{
	Message transaction;
	Date date;
	string input_buff;

	transaction.acquirer = acquirer;
	cin >> date.year >> date.month >> date.day;
	cin >> date.hour >> date.minute >> date.second;

	transaction.date = date;

	//P12 & P13 local transaction data & time
	transaction.P13 = pad_zero(ullong_to_str(date.month), _2_DIGITS);
	transaction.P13 += pad_zero(ullong_to_str(date.day), _2_DIGITS);
	transaction.P12 = pad_zero(ullong_to_str(date.hour), _2_DIGITS);
	transaction.P12 += pad_zero(ullong_to_str(date.minute), _2_DIGITS);
	transaction.P12 += pad_zero(ullong_to_str(date.second), _2_DIGITS);

	//P7 date & time of data transmition
	transaction.P7 = transaction.P13 + transaction.P12;

	//P2 card number
	cin >> input_buff;
	input_buff = strip_string(input_buff);
	transaction.P2 = pad_zero(ullong_to_str(input_buff.length()), _2_DIGITS);
	transaction.P2 += input_buff;

	//P4(acquirer) & P6(issuer) mablagh
	cin >> input_buff;
	transaction.P4 = pad_zero(input_buff, P4_MAX);
	transaction.P6 = pad_zero(ullong_to_str(
		str_to_ullong(transaction.P4)
		* str_to_ullong(transaction.acquirer.P10)), P6_MAX);

	cin >> input_buff;//read "end" and remove it from STDIN

	return transaction;
}