// Fill out your copyright notice in the Description page of Project Settings.


#include "DTreeT.h"

DTreeT::DTreeT() {
	this->_rootName = "root";
	this->_isLeaf = false;
}

void DTreeT::SetRootName(string name) {
	this->_rootName = name;
}

void DTreeT::SetNodeToLeaf(bool isLeaf) {
	this->_isLeaf = isLeaf;
}

void DTreeT::AddChild(string str, DTreeT tree) {
	this->_children.insert(make_pair(str, tree));
}

string DTreeT::GetRootName() const {
	return this->_rootName;
}

void DTreeT::PrintTree(int val) const {
	string tmp = "      ";
	if (this->_isLeaf) {
		for (int p = 0; p < val + 1; p++)
			cout << tmp;
		cout << "@---[LEAF] " << this->_rootName << endl;
	}
	else {
		for (int i = 0; i < val; i++)
			cout << tmp;
		cout << "@---[ATTR] " << this->_rootName << endl;
		for (std::map<std::string, DTreeT>::const_iterator iter = _children.begin(); iter != _children.end(); ++iter)
		{
			for (int i = 0; i < val + 1; i++)
				cout << tmp;
			cout << "@---[ATTRVAL] " << iter->first << std::endl;
			(iter->second).PrintTree(val + 2);
		}
	}
}

void PrintData(const DTreeData* data) {
	cout << "Attributes: " << endl;
	for (string item : data->_attrs) {
		cout << item << " ";
	}
	cout << endl << endl << "Values: " << endl;
	for (vector<string> item : data->_vals) {
		for (string val : item) {
			cout << val << " ";
		}
		cout << endl;
	}
	cout << endl << endl;
}

vector<string> GetAttrVal(const string& attr, const  DTreeData* data) {
	vector<string> vals;
	string value;
	vector<string>::const_iterator itr;
	for (vector<string> item : data->_vals) {
		itr = find(data->_attrs.begin(), data->_attrs.end(), attr);
		value = item[itr - data->_attrs.begin()];
		if (find(vals.begin(), vals.end(), value) == vals.end()) {
			vals.push_back(value);
		}
	}
	return vals;
}

DTreeData GetDatax(const string& filename) {
	DTreeData treeData;
	ifstream fileStream("gg.dat");
	string row, value;
	int index = 0;
	while (!fileStream.eof()) {
		getline(fileStream, row);
		istringstream string_stream(row);

		if (index < 1) {
			while (getline(string_stream, value, ',')) {
				treeData._attrs.push_back(value);
			}
			index++;
		}
		else {
			vector<string> tmp;
			while (getline(string_stream, value, ',')) {
				tmp.push_back(value);
			}
			treeData._vals.push_back(tmp);
		}
	}
	treeData._xAxis = treeData._attrs.size();
	treeData._yAxis = treeData._vals.size();
	fileStream.close();

	return treeData;
}

DTreeData GetValues(const string& attr, const string& val, const  DTreeData* data) {
	DTreeData result;
	vector<string> value;
	vector<string>::const_iterator iterator;
	iterator = find(data->_attrs.begin(), data->_attrs.end(), attr);

	if (iterator != data->_attrs.end()) {
		int id = iterator - data->_attrs.begin();
		for (vector<vector<string>>::const_iterator itr = data->_vals.begin(); itr != data->_vals.end(); ++itr)
			if ((*itr)[id] == val)
				result._vals.push_back(*itr);

		result._attrs = data->_attrs;
		result._xAxis = data->_xAxis;
		result._yAxis = result._vals.size();
	}
	return result;
}

double CalcValEntropy(const DTreeData* data) {
	vector<string> vectorXCol;
	vector<string>::const_iterator iterator;
	vector<int> vectorXColSize;
	int vectorSum = data->_vals.size();

	for (vector< vector<string> >::const_iterator iter = data->_vals.begin(); iter != data->_vals.end(); ++iter)
	{
		iterator = find(vectorXCol.begin(), vectorXCol.end(), (*iter)[data->_xAxis - 1]);
		if (iterator == vectorXCol.end())
		{
			vectorXCol.push_back((*iter)[data->_xAxis - 1]);
			vectorXColSize.push_back(1);
		}
		else
			++vectorXColSize[iterator - vectorXCol.begin()];
	}
	vector<double> partsEntropy(vectorXColSize.size(), .0);
	double entropy = .0;

	for (int i = 0; i != partsEntropy.size(); ++i)
	{
		partsEntropy[i] = static_cast<double>(vectorXColSize[i]) / vectorSum;
		entropy -= partsEntropy[i] * log2(partsEntropy[i]);
	}
	return entropy;
}

double CalcAttrEntropy(const string& attr, const DTreeData* data) {
	vector<string> vals = GetAttrVal(attr, data);
	DTreeData tmpData;
	double entropy = .0, entropyVal = .0;
	for (vector<string>::const_iterator iter = vals.begin(); iter != vals.end(); ++iter) {
		tmpData = GetValues(attr, *iter, data);
		entropyVal = CalcValEntropy(&tmpData);
		entropy += entropyVal * tmpData._yAxis / data->_yAxis;
	}
	return entropy;
}

DTreeT* MakeTree(const DTreeData* data) {
	DTreeT* tree = new DTreeT();
		
	DTreeT	tmp;
	double entropy = CalcValEntropy(data), infGainLocal = .0, entropyAttr = .0, infGain = .0;
	for (vector<string>::const_iterator iter = data->_attrs.begin(); iter != data->_attrs.end() - 1; ++iter)
	{
		entropyAttr = CalcAttrEntropy(*iter, data);
		infGainLocal = entropy - entropyAttr;
		if (infGainLocal > infGain)
		{
			infGain = infGainLocal;
			tree->SetRootName(*iter);
		}
	}
	if (tree->GetRootName() != "root")
	{
		vector<string> children = GetAttrVal(tree->GetRootName(), data);
		for (vector<string>::const_iterator iter = children.begin(); iter != children.end(); ++iter)
		{
			DTreeData treeData = GetValues(tree->GetRootName(), *iter, data);
			tmp = *MakeTree(&treeData);
			tree->AddChild(*iter, tmp);
		}
	}
	else
	{
		tree->SetNodeToLeaf(true);
		tree->SetRootName((data->_vals[0])[data->_xAxis - 1]);
	}
	return tree;
}

DTreeT* PrepareTree() {
	DTreeData* data = new DTreeData();
	vector<string> aw{ "NumberOfAllies", "CurrentHealth", "CurrentShield", "Class" };
	vector<string> aw1{ "Many", "LessThanHalf", "MoreThanHalf", "GoGetHealthPack" };
	vector<string> aw2{ "Many", "LessThanHalf", "LessThanHalf", "HideAndRegen" };
	vector<string> aw3{ "None", "LessThanHalf", "LessThanHalf", "ChargeAtPlayer" };
	vector<string> aw4{ "None", "MoreThanHalf", "MoreThanHalf", "ChargeAtPlayer" };
	vector<string> aw5{ "None", "MoreThanHalf", "LessThanHalf", "ChargeAtPlayer" };
	vector<string> aw6{ "None", "LessThanHalf", "MoreThanHalf", "ChargeAtPlayer" };
	vector<vector<string>> tm{ aw1, aw2, aw3, aw4, aw5, aw6};
	data->_attrs = aw;
	data->_vals = tm;
	data->_xAxis = aw.size();
	data->_yAxis = 6;
	//PrintData(data);
	DTreeT* tree = MakeTree(data);
	//tree.PrintTree();
	return tree;
}

string DTreeT::GetDecision(map<string, string> input) {
	if (this->_isLeaf) {
		return this->_rootName;
	}

	for (auto child : this->_children) {
		if (child.first == input[this->_rootName]) {
			return child.second.GetDecision(input);
		}
	}

	return NULL;
}