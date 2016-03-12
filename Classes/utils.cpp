#include "utils.h"
#include <math.h>
#include <vector>
#include "SVector2D.h"
//--------------------------itos------------------------------------
//	converts an integer to a string
//------------------------------------------------------------------		
string itos(int arg)
{
    ostringstream buffer;
	
	//send the int to the ostringstream
    buffer << arg;	
	
	//capture the string
    return buffer.str();		
}


//--------------------------ftos------------------------------------
//	converts a float to a string
//------------------------------------------------------------------		
string ftos(float arg)
{
    ostringstream buffer;
	
	//send the int to the ostringstream
    buffer << arg;	
	
	//capture the string
    return buffer.str();		
}
//-------------------------------------Clamp()-----------------------------------------
//
//	clamps the first argument between the second two
//
//-------------------------------------------------------------------------------------
void Clamp( double &arg, float min, float max)
{
	if (arg < min)
	{
		arg = min;
	}

	if (arg > max)
	{
		arg = max;
	}
}

SVector2D julidistance( SPoint first, SPoint second)
{
	SVector2D r(second.x-first.x,second.y-first.y);

	return r;
}

float distance(SPoint first, SPoint second)
{
	float r;
	r=sqrt((first.x-second.x)*(first.x-second.x)+(first.y-second.y)*(first.y-second.y));
	return r;
}

void sortmi2max(vector<vector <double> > &sortvet)
{ 
	for(int  i = 0; i < sortvet.size()-1; i++ )
	{
		for (int j=0;j<sortvet.size()-i-1;j++)
		{
			if (sortvet[j][1]>sortvet[j+1][1])
			{
				swap(sortvet[j],sortvet[j+1]);
			}
			
		}
		
	}

} 
