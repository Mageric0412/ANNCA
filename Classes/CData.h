#ifndef DATA_H
#define DATA_H
//turn off the warnings for the STL
#pragma warning (disable : 4786)

//------------------------------------------------------------------------
//
//	Name: Crowd_TData.h
//
//  Author: Mat Buckland 2002
//
//  Desc: class to manipulate the gesture data for the RecognizeIt mouse
//        gesture recognition code project
//-------------------------------------------------------------------------

#include <vector>
#include <iomanip>
#include <windows.h>

#include "defines.h"
#include "utils.h"

using namespace std;



//------------------------------------------------------------------------
//
//  class to handle the training data
//------------------------------------------------------------------------
class Crowd_TData
{
private:

	vector<vector<double> > ANN_SetIn;
	vector<vector<double> > ANN_SetOut;

	vector< vector<double> >    m_Vecout;
	//string  m_vecNames[30];

	vector<vector<double> > m_vecPatterns;

	int            AnglePatterns;


	int            m_iVectorSize;

	void           InitData();


public:

	Crowd_TData(int NumStartPatterns,
		int VectorSize)    :AnglePatterns(NumStartPatterns),
		m_iVectorSize(VectorSize)

	{
		InitData();

		CreatANN_TrainSet();
	}

	vector<double>  ClassifyPattern(int val);

	bool    AddTraingData(vector<double> &data, vector<double> New);

	void CreatANN_TrainSet();

	vector<vector<double> > GetInputSet() {return ANN_SetIn;}
	vector<vector<double> > GetOutputSet(){return ANN_SetOut;}

};

#endif
