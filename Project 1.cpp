
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
using namespace std;
void readParse(vector<string>* res, vector<string>* labels, vector<int>* location);
string lower(string s);
void encode(vector<string>* res, vector<string>* type,vector<string>* labels, vector<int>* location, ofstream* outFile);
void iAlu(vector<string> type, int val, ofstream* outFile);
void load(vector<string> type, int instr, ofstream* outFile);
void store(vector<string> type, int instr, ofstream* outFile);
void lhi(vector<string> type, int instr, ofstream* outFile);
void iJump(vector<string> type, int instr, ofstream* outFile);
void branch(vector<string> type, vector<string> labels, vector<int> location, int instr, int num, ofstream* outFile);
void rMath(vector<string> type, int instr, ofstream* outFile);
void jJump(vector<string> type, vector<string> labels, vector<int> location, int instr, int num, ofstream* outFile);

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main()
{
	ofstream outFile("output.txt");
	vector<string> result;
	vector<string> temp;
	vector<string> labels;
	vector<int> location;
	readParse(&result, &labels, &location);
	encode(&result, &temp, &labels, &location, &outFile);
    return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void readParse(vector<string>* res, vector<string>* labels, vector<int>* location)
{
	ifstream myfile("Input.txt");
	string instr;
	int index = -1;
	while (getline(myfile, instr)) {
		index++;
		char str[50];
		strcpy(str, instr.c_str());
		char* cur;
		cur = strtok(str, " ,\n\t#()");
		while (cur != NULL) {
			string item = lower(cur);
			if (item[item.length() - 1] == ':')
			{
				string l = item.substr(0, item.length() - 1);
				labels->push_back(l);
				location->push_back(index);
			}
			else
			{
				res->push_back(item);
			}
			cur = strtok(NULL, " ,\n\t#()");
		}
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
string lower(string s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (isupper(s[i]))
		{
			s[i] = tolower(s[i]);
		}
	}
	return s;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void encode(vector<string>* res, vector<string>* type, vector<string>* labels, vector<int>* location, ofstream* outFile)
{
	string loadi[25] = { "lb","lh","lw","lbu","lhu" };
	string storei[25] = { "sb","sh","sw" };
	string mathi[25] = { "addi","addui","subi","subui","andi","ori","xori","seqi","snei","slti","sgti","slei","sgei","sequi","sneui","sltui","sgtui","sleui","sgeui","slli","srli","srai" };
	string branchi[25] = { "beqz","bnez" };
	string mathr[25] = { "sll","srl","sra","sltu","sgtu","sleu","sgeu","add","addu","sub","subu","and","or","xor","seq","sne","slt","sgt","sle","sge" };
	string jump[25] = { "j","jal" };
	string jumpi[25] = { "jr","jalr" };
	int lopcode[25] = { 32,33,35,36,37 };
	int sopcode[25] = { 40,41,43 };
	int mopcode[25] = { 8,9,10,11,12,13,14,24,25,26,27,28,29,48,49,50,51,52,53,54,55,56 };
	int bopcode[25] = { 4,5 };
	int mropcode[25] = { 4,6,7,18,19,20,21,32,33,34,35,36,37,38,40,41,42,43,44,45 };
	int jopcode[25] = { 2,3 };
	int jiopcode[25] = { 18,19 };
	int lhiCode = 15;
	int instruction;
	int count = 0;
	for (int i = 0; i < res->size(); i++)
	{
		if (res->at(i) == "lhi")
		{
			type->push_back(res->at(i));
			type->push_back(res->at(i+1));
			type->push_back(res->at(i+2));
			instruction = lhiCode;
			lhi(*type, instruction, outFile);
			type->clear();
			count++;
		}
		for (int j = 0; j < 25; j++)
		{
			if (res->at(i) == loadi[j])
			{
				type->push_back(res->at(i));
				type->push_back(res->at(i + 1));
				type->push_back(res->at(i + 2));
				type->push_back(res->at(i + 3));
				instruction = lopcode[j];
				load(*type, instruction, outFile);
				type->clear();
				count++;
			}
			else if (res->at(i) == storei[j])
			{
				type->push_back(res->at(i));
				type->push_back(res->at(i + 1));
				type->push_back(res->at(i + 2));
				type->push_back(res->at(i + 3));
				instruction = sopcode[j];
				store(*type, instruction, outFile);
				type->clear();
				count++;
			}
			else if (res->at(i) == mathi[j])
			{
				type->push_back(res->at(i));
				type->push_back(res->at(i + 1));
				type->push_back(res->at(i + 2));
				type->push_back(res->at(i + 3));
				instruction = mopcode[j];
				iAlu(*type, instruction, outFile);
				type->clear();
				count++;
			}
			else if (res->at(i) == branchi[j])
			{
				type->push_back(res->at(i));
				type->push_back(res->at(i + 1));
				type->push_back(res->at(i + 2));
				type->push_back(res->at(i + 3));
				instruction = bopcode[j];
				branch(*type, *labels, *location, instruction, count, outFile);
				type->clear();
				count++;
			}
			else if (res->at(i) == mathr[j])
			{
				type->push_back(res->at(i));
				type->push_back(res->at(i + 1));
				type->push_back(res->at(i + 2));
				type->push_back(res->at(i + 3));
				instruction = mropcode[j];
				rMath(*type, instruction, outFile);
				type->clear();
				count++;
			}
			else if (res->at(i) == jump[j])
			{
				type->push_back(res->at(i));
				type->push_back(res->at(i + 1));
				instruction = jopcode[j];
				jJump(*type, *labels, *location, instruction, count, outFile);
				type->clear();
				count++;
			}
			else if (res->at(i) == jumpi[j])
			{
				type->push_back(res->at(i));
				type->push_back(res->at(i + 1));
				type->push_back(res->at(i + 2));
				type->push_back(res->at(i + 3));
				instruction = jiopcode[j];
				iJump(*type, instruction, outFile);
				type->clear();
				count++;
			}
		}
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void load(vector<string> type, int instr, ofstream* outFile)
{
	string rs1 = type[3].substr(1,type[3].length());
	string rs2 = type[1].substr(1, type[1].length());;
	string imm = type[2];
	int rs1Num = atoi(rs1.c_str()) << 21;
	int rs2Num = atoi(rs2.c_str()) << 16;
	int immNum = atoi(imm.c_str()) & 0x0000FFFF;
	int code = (instr << 26) | rs1Num | rs2Num | immNum;
	cout << hex << setw(8) << setfill('0') << code << dec << setfill(' ') << endl;
	*outFile << hex << setw(8) << setfill('0') << code << dec << setfill(' ');
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void store(vector<string> type, int instr, ofstream* outFile)
{
	string rs1 = type[2].substr(1, type[2].length());
	string rs2 = type[3].substr(1, type[3].length());;
	string imm = type[1];
	int rs1Num = atoi(rs1.c_str()) << 21;
	int rs2Num = atoi(rs2.c_str()) << 16;
	int immNum = atoi(imm.c_str()) & 0x0000FFFF;
	int code = (instr << 26) | rs1Num | rs2Num | immNum;
	cout << hex << setw(8) << setfill('0') << code << dec << setfill(' ') << endl;
	*outFile << hex << setw(8) << setfill('0') << code << dec << setfill(' ');
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void iAlu(vector<string> type, int instr, ofstream* outFile)
{
	string rs1 = type[2].substr(1, type[2].length());
	string rs2 = type[1].substr(1, type[1].length());
	string imm = type[3];
	int rs1Num = atoi(rs1.c_str()) << 21;
	int rs2Num = atoi(rs2.c_str()) << 16;
	int immNum = atoi(imm.c_str()) & 0x0000FFFF;
	int code = (instr << 26) | rs1Num | rs2Num | immNum;
	cout << hex << setw(8) << setfill('0') << code << dec << setfill(' ') << endl;
	*outFile << hex << setw(8) << setfill('0') << code << dec << setfill(' ');
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void iJump(vector<string> type, int instr, ofstream* outFile)
{
	string rs1 = type[1].substr(1, type[1].length());
	int rs1Num = atoi(rs1.c_str()) << 21;
	int rs2Num = 0;
	int immNum = 0;
	int code = (instr << 26) | rs1Num | rs2Num | immNum;
	cout << hex << setw(8) << setfill('0') << code << dec << setfill(' ') << endl;
	*outFile << hex << setw(8) << setfill('0') << code << dec << setfill(' ');
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void branch(vector<string> type, vector<string> labels, vector<int> location, int instr, int num, ofstream* outFile)
{
	string rs1 = type[1].substr(1, type[1].length());
	string offset = type[2];
	int rs1Num = atoi(rs1.c_str()) << 21;
	int rs2Num = 0 << 16;
	int labelLoc = 0;
	for (int i = 0; i < labels.size(); i++)
	{
		if (offset == labels[i])
		{
			labelLoc = location[i];
		}
	}
	int offsetNum = ((labelLoc - (num+1)) *4) & 0x0000FFFF;
	int code = (instr << 26) | rs1Num | rs2Num | offsetNum;
	cout << hex << setw(8) << setfill('0') << code << dec << setfill(' ') << endl;
	*outFile << hex << setw(8) << setfill('0') << code << dec << setfill(' ');
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void lhi(vector<string> type, int instr, ofstream* outFile)
{
	string rs2 = type[1].substr(1, type[1].length());;
	string imm = type[2];
	int rs1Num = 0;
	int rs2Num = atoi(rs2.c_str()) << 16;
	int immNum = atoi(imm.c_str()) & 0x0000FFFF;
	int code = (instr << 26) | rs1Num | rs2Num | immNum;
	cout << hex << setw(8) << setfill('0') << code << dec << setfill(' ') << endl;
	*outFile << hex << setw(8) << setfill('0') << code << dec << setfill(' ');
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void rMath(vector<string> type, int instr, ofstream* outFile)
{
	string rs1 = type[2].substr(1, type[2].length());
	string rs2 = type[3].substr(1, type[3].length());
	string rd = type[1].substr(1, type[1].length());
	int rs1Num = atoi(rs1.c_str()) << 21;
	int rs2Num = atoi(rs2.c_str()) << 16;
	int rdNum = atoi(rd.c_str()) << 11;
	int code = (0 << 26) | rs1Num | rs2Num | rdNum | instr;
	cout << hex << setw(8) << setfill('0') << code << dec << setfill(' ') << endl;
	*outFile << hex << setw(8) << setfill('0') << code << dec << setfill(' ');
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void jJump(vector<string> type, vector<string> labels, vector<int> location, int instr, int num, ofstream* outFile)
{
	string offset = type[1];
	int labelLoc = 0;
	for (int i = 0; i < labels.size(); i++)
	{
		if (offset == labels[i])
		{
			labelLoc = location[i];
		}
	}
	int offsetNum = ((labelLoc - (num+1))*4) & 0x03FFFFFF;
	int code = (instr << 26) | offsetNum;
	cout << hex << setw(8) << setfill('0') << code << dec << setfill(' ') << endl;
	*outFile << hex << setw(8) << setfill('0') << code << dec << setfill(' ');
}