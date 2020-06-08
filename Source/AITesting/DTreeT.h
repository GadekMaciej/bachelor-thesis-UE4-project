// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



/**
 * 
 */


#include "CoreMinimal.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct DTreeData {
	int _yAxis = 0;
	int _xAxis = 0;
	vector<string> _attrs;
	vector<vector<string>> _vals;
};

class AITESTING_API DTreeT {
public:
	DTreeT();
	void SetRootName(string);
	void SetNodeToLeaf(bool);
	void AddChild(string, DTreeT);
	void PrintTree(int = 0) const;
	string GetRootName() const;
	string GetDecision(map<string, string>);
private:
	string _rootName;
	map<string, DTreeT> _children;
	bool _isLeaf;
};

void PrintData(const DTreeData*);
vector<string> GetAttrVal(const string&, const DTreeData*);
DTreeData GetDatax(const string&);
DTreeData GetValues(const string&, const string&, const DTreeData*);
double CalcValEntropy(const DTreeData*);
double CalcAttrEntropy(const string&, const DTreeData*);
DTreeT* MakeTree(const DTreeData*);
DTreeT* PrepareTree();