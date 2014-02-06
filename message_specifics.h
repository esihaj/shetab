#include <string>
#ifndef _SHETAB_MSG_SPECS_
#define _SHETAB_MSG_SPECS_
using std::string;
struct Acquirer{
	string P10;//exchange rate
	string P25;//device type
	string P32;//acquirer ID
};
struct Date{
	int year, month, day;
	int hour, minute, second;
};
struct Message{
	string ISO_message_type;
	string ISO_bit_pattern;
	string P2;//card number
	string P4;//mablagh e transaction acquirer 
	string P6;//mablagh e transaction issuer
	string P7;//date and time of data transmit!
	string P11;//shomare peygiri
	string P12;//local time of transaction 
	string P13;//local date of transaction
	string P14;//expiration date
	string P33;//issuer ID
	Acquirer acquirer;//echange rate, device type, acquirer ID
	Date date;
	string message;
};

#define ISO_msg_type_MAX	4 
#define ISO_bit_pattern_MAX 16 
#define P2_MAX				19 
#define P4_MAX				12 //mablagh e transaction acquirer 
#define P6_MAX				12 //mablagh e transaction issuer
#define P7_MAX				10 //date and time of data transmit!
#define P10_MAX				8 //exchange rate
#define P11_MAX				6 //shomare peygiri
#define P12_MAX				6 //local time of transaction 
#define P13_MAX				4 //local date of transaction
#define P14_MAX				4 //expiration date
#define P25_MAX				2 //device type
#define P32_MAX				11 //acquirer ID
#define P33_MAX				11 //issuer ID
#define _2_DIGITS			    2

#endif//_SHETAB_MSG_SPECS_