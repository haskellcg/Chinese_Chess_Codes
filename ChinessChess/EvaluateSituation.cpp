#include "EvaluateSituation.h"
#include <iostream>
using std::cout;
using std::endl;
double chessManValRateInOpenMiddle[17]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
double chessManValRateInEnd[17]	      ={1,1.05,1.05,1.05,1.05,1.2,1.2,1.2,1.2,0.8,0.8,1.3,1.3,1.3,1.3,1.3,0.95};
int chessManImportanceValue[16]={30,2,2,3,3,6,6,12,12,6,6,2,2,2,2,2};
int chessManCheckAndBalanceValue[7][7]={
				{ 0,-1,-1, 2, 4, 3,-2},
				{-2, 2, 0, 2, 4, 2,-2},
				{-2, 0, 3, 2, 4, 2, 1},
				{-2, 1, 1, 8, 4, 2, 1},
				//连环马加分
				{-2, 1, 1, 2, 4, 9, 1},
				//担子炮加分
				{-2, 1, 1, 2, 10, 2, 1},
				//霸王车加分
				{-2, 0, 0, 3, 5, 3, 7}
				//过河兵牵手加分
				};
int chessManFlexibilityValue[3][16]={
				{1,1,1,1,1,2,2,4,4,2,2,1,1,1,1,1},
				{2,1,1,1,1,2,2,5,5,2,2,1,1,1,1,1},
				{3,1,1,1,1,2,2,5,5,2,2,2,2,2,2,2}
				};
int chessManStaticPosVal[2][7][256]=
{
	//for normal red
	{
		//king
		{
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,1  ,1  ,1  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,10 ,10 ,10 ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,15 ,20 ,15 ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  
		},
		//advisor
		{
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,30 ,0  ,30 ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,22 ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,30 ,0  ,30 ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  
		},
		//bishop
		{
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,25 ,0  ,0  ,0  ,25 ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,20 ,0  ,0  ,0  ,35 ,0  ,0  ,0  ,20 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,30 ,0  ,0  ,0  ,30 ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  
		},
		//knight
		{
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,70 ,80 ,90 ,80 ,70 ,80 ,90 ,80 ,70 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,80 ,110,125,90 ,70 ,90 ,125,110,80 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,100,120,125,120,125,120,100,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,100,120,130,110,130,120,100,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,110,110,120,100,120,110,110,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,100,100,110,100,110,100,100,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,80 ,90 ,100,100,90 ,100,100,90 ,80 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,80 ,80 ,90 ,90 ,80 ,90 ,90 ,80 ,80 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,70 ,75 ,75 ,70 ,50 ,70 ,75 ,75 ,70 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,60 ,70 ,75 ,70 ,60 ,70 ,75 ,70 ,60 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  
		},
		//rook
		{
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,160,170,160,150,150,150,160,170,160,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,170,180,170,190,250,190,170,180,170,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,170,190,200,220,240,220,200,190,170,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,180,220,210,240,250,240,210,220,180,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,180,220,210,240,250,240,210,220,180,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,180,220,210,240,250,240,210,220,180,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,170,190,180,220,240,220,180,190,170,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,170,180,170,170,160,170,170,180,170,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,160,170,160,160,150,160,160,170,160,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,150,160,150,160,150,160,150,160,150,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  
		},
		//cannon
		{
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,125,130,100,70 ,60 ,70 ,100,130,125,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,110,125,100,70 ,60 ,70 ,100,125,110,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,100,120,90 ,80 ,80 ,80 ,90 ,120,100,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,110,90 ,110,130,110,90 ,110,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,110,90 ,110,130,110,90 ,110,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,100,90 ,100,130,100,90 ,100,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,100,90 ,90 ,110,90 ,90 ,100,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,100,80 ,80 ,70 ,80 ,80 ,100,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,80 ,90 ,80 ,70 ,65 ,70 ,80 ,90 ,80 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,80 ,90 ,80 ,70 ,60 ,70 ,80 ,90 ,80 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  
		},
		//pawn
		{
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,10 ,10 ,10 ,20 ,25 ,20 ,10 ,10 ,10 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,25 ,30 ,40 ,50 ,60 ,50 ,40 ,30 ,25 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,25 ,30 ,30 ,40 ,40 ,40 ,30 ,30 ,25 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,20 ,25 ,25 ,30 ,30 ,30 ,25 ,25 ,20 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,15 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,15 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,10 ,0  ,15 ,0  ,15 ,0  ,15 ,0  ,10 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,10 ,0  ,10 ,0  ,15 ,0  ,10 ,0  ,10 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  
		}
	},
	//for normal black
	{
		//king
		{
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,15 ,20 ,15 ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,10 ,10 ,10 ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,1  ,1  ,1  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  
		},
		//advisor
		{
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,30 ,0  ,30 ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,22 ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,30 ,0  ,30 ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  
		},
		//bishop
		{
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,30 ,0  ,0  ,0  ,30 ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,20 ,0  ,0  ,0  ,35 ,0  ,0  ,0  ,20 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,25 ,0  ,0  ,0  ,25 ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  
		},
		//knight
		{
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,60 ,70 ,75 ,70 ,60 ,70 ,75 ,70 ,60 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,70 ,75 ,75 ,70 ,50 ,70 ,75 ,75 ,70 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,80 ,80 ,90 ,90 ,80 ,90 ,90 ,80 ,80 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,80 ,90 ,100,100,90 ,100,100,90 ,80 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,100,100,110,100,110,100,100,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,110,110,120,100,120,110,110,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,100,120,130,110,130,120,100,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,100,120,125,120,125,120,100,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,80 ,110,125,90 ,70 ,90 ,125,110,80 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,70 ,80 ,90 ,80 ,70 ,80 ,90 ,80 ,70 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  
		},
		//rook
		{
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,150,160,150,160,150,160,150,160,150,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,160,170,160,160,150,160,160,170,160,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,170,180,170,170,160,170,170,180,170,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,170,190,200,220,240,220,200,190,170,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,180,220,210,240,250,240,210,220,180,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,180,220,210,240,250,240,210,220,180,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,180,220,210,240,250,240,210,220,180,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,170,190,200,220,240,220,200,190,170,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,170,180,170,190,250,190,170,180,170,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,160,170,160,150,150,150,160,170,160,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  
		},
		//cannon
		{
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,80 ,90 ,80 ,70 ,60 ,70 ,80 ,90 ,80 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,80 ,90 ,80 ,70 ,65 ,70 ,80 ,90 ,80 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,100,80 ,80 ,70 ,80 ,80 ,100,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,100,90 ,90 ,110,90 ,90 ,100,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,100,90 ,110,130,110,90 ,100,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,110,90 ,110,130,110,90 ,110,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,90 ,110,90 ,110,130,110,90 ,110,90 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,100,120,90 ,80 ,80 ,80 ,90 ,120,100,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,110,125,100,70 ,60 ,70 ,100,125,110,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,125,130,100,70 ,60 ,70 ,100,130,125,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  
		},
		//pawn
		{
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,10 ,0  ,10 ,0  ,15 ,0  ,10 ,0  ,10 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,10 ,0  ,15 ,0  ,15 ,0  ,10 ,0  ,10 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,15 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,15 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,20 ,25 ,25 ,30 ,30 ,30 ,25 ,25 ,20 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,25 ,30 ,30 ,40 ,40 ,40 ,30 ,30 ,25 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,25 ,30 ,40 ,50 ,60 ,50 ,40 ,30 ,25 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,10 ,10 ,10 ,20 ,25 ,20 ,10 ,10 ,10 ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
			0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  
		}
	}
};
void EvaluateSituation::initializeLegalPosition()
{
	for(int i=0;i<256;i++)
                legalPosition[0][i]=legalPosition[1][i]=0;

        int knight_cannon_rook=cannonLegalPosVal | knightLegalPosVal | rookLegalPosVal;
        int base=3*16+3;
        for(int i=3;i<=12;i++)
        {       
                for(int j=3;j<=11;j++)
                {       
                        legalPosition[0][base]=legalPosition[0][base] | knight_cannon_rook;
                        legalPosition[1][base]=legalPosition[1][base] | knight_cannon_rook;
                        ++base; 
                }       
                base+=7;
        }       
        base=10*16+6;
	for(int i=10;i<=12;i++)
        {       
                for(int j=6;j<=8;j++)
                {
                        legalPosition[0][base]=legalPosition[0][base] | kingLegalPosVal;
                        legalPosition[1][240-base+2*j]=legalPosition[1][240-base+2*j] | kingLegalPosVal;
                        ++base;
                }
                base+=13;
        }
        base=10*16+6;
        int advisorLegalPosOff[5]={0,2,15,15,2};
        for(int i=0;i<5;i++)
        {
                base=base+advisorLegalPosOff[i];
                legalPosition[0][base]=legalPosition[0][base] | advisorLegalPosVal;
                int j=base%16;
                legalPosition[1][240-base+2*j]=legalPosition[1][240-base+2*j] | advisorLegalPosVal;
        }
        base=8*16+5;
        int bishopLegalPosOff[7]={0,4,26,4,4,26,4};
        for(int i=0;i<7;i++)
        {
                base=base+bishopLegalPosOff[i];
                legalPosition[0][base]=legalPosition[0][base] | bishopLegalPosVal;
                int j=base%16;
                legalPosition[1][240-base+2*j]=legalPosition[1][240-base+2*j] | bishopLegalPosVal;
        }
        base=8*16+3;
        int somePawnLegalPosOff[10]={0,2,2,2,2,8,2,2,2,2};
        for(int i=0;i<10;i++)
        {
                base=base+somePawnLegalPosOff[i];
                legalPosition[0][base]=legalPosition[0][base] | pawnLegalPosVal;
                int j=base%16;
                legalPosition[1][240-base+2*j]=legalPosition[1][240-base+2*j] | pawnLegalPosVal;
	}
        base=3*16+3;
        for(int i=3;i<=7;i++)
        {
                for(int j=3;j<=11;j++)
                {
                        legalPosition[0][base]=legalPosition[0][base] | pawnLegalPosVal;
                        legalPosition[1][240-base+2*j]=legalPosition[1][240-base+2*j] | pawnLegalPosVal;
                        ++base;
                }
                base+=7;
        }
}
bool EvaluateSituation::isInLegalPosition(int source,int chessManPosVal,int position) const
{
	if(position<0 || position>255 || source<0 || source>255)
	{
		cout<<"ERROR:Out of  cChessArray index........"<<endl;
		return false;
	}
	if(cChess->cChessArray[source]==0)
	{
		cout<<"No Chess Man In "<<source<<endl;
		return false;
	}
	int index=0;
	bool isRedChessMan=false;
	if(cChess->cChessArray[source] < 32)
		isRedChessMan=true;
        if(cChess->cChessManPosition[0] > 8*16) 
        {       
                index=(isRedChessMan==true?0:1);
        }       
        else    
        {       
                index=(isRedChessMan==true?1:0);
        }       
        return (chessManPosVal & legalPosition[index][position]);
}
//判断局势是否为残局
bool EvaluateSituation::isEndGame() const
{
	int redNumberOfKnight_Rook_Cannon_Pawn=0;
	int blackNumberOfKnight_Rook_Cannon_Pawn=0;
	for(int i=5;i<=10;i++)
	{
		if(cChess->cChessManPosition[i]!=0)
			++redNumberOfKnight_Rook_Cannon_Pawn;
		if(cChess->cChessManPosition[i+16]!=0)
			++blackNumberOfKnight_Rook_Cannon_Pawn;
	}
	if(redNumberOfKnight_Rook_Cannon_Pawn<=3 || blackNumberOfKnight_Rook_Cannon_Pawn<=3)
		return true;
	else
		return false;
}
//前提：position位置有棋子
int EvaluateSituation::getChessManStaticPosVal(int position) const
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index ...."<<endl;
		return 0;
	}
	if(cChess->cChessArray[position]==0)
	{
		cout<<"ERROR:No ChessMan in "<<position<<endl;
		return 0;
	}
	bool isRedOnBigEnd=false,isRedChessMan=false;
	int index=0,chessManSub;
	if(cChess->cChessManPosition[0] > 8*16)
		isRedOnBigEnd=true;
	if(cChess->cChessArray[position] < 32)
		isRedChessMan=true;
	index=(isRedOnBigEnd==isRedChessMan?0:1);
	return chessManStaticPosVal[index][getChessManSub(cChess->cChessArray[position])][position];
}
//静态局面评估，分为开局中局、残局两类
double EvaluateSituation::chessManStaticPosEvaluate() const
{
	double redStaticEvaluate=0,blackStaticEvaluate=0;
	bool endGame=isEndGame();
	if(!endGame)
	{
		for(int i=0;i<16;i++)
		{
			if(cChess->cChessManPosition[i]!=0)
			{
				redStaticEvaluate+=getChessManStaticPosVal(cChess->cChessManPosition[i])*chessManValRateInOpenMiddle[i];
			}
		} 
		for(int i=16;i<32;i++)
		{
			if(cChess->cChessManPosition[i]!=0)
			{
				blackStaticEvaluate+=getChessManStaticPosVal(cChess->cChessManPosition[i])*chessManValRateInOpenMiddle[i-16];
			}
		} 
		redStaticEvaluate*=chessManValRateInOpenMiddle[16];
		blackStaticEvaluate*=chessManValRateInOpenMiddle[16];
	}
	else
	{
		for(int i=0;i<16;i++)
		{
			if(cChess->cChessManPosition[i]!=0)
			{
				redStaticEvaluate+=getChessManStaticPosVal(cChess->cChessManPosition[i])*chessManValRateInEnd[i];
			}
		} 
		for(int i=16;i<32;i++)
		{
			if(cChess->cChessManPosition[i]!=0)
			{
				blackStaticEvaluate+=getChessManStaticPosVal(cChess->cChessManPosition[i])*chessManValRateInEnd[i-16];
			}
		} 
		redStaticEvaluate*=chessManValRateInEnd[16];
		blackStaticEvaluate*=chessManValRateInEnd[16];
	}
	
	if(cChess->isRed)
		return redStaticEvaluate-blackStaticEvaluate;
	else
		return blackStaticEvaluate-redStaticEvaluate;
}
bool EvaluateSituation::isCrossRiver(int position) const
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index ...."<<endl;
		return 0;
	}
	if(cChess->cChessArray[position]==0)
	{
		cout<<"ERROR:No ChessMan in "<<position<<endl;
		return false;
	}
	bool isRedChessMan=false,isRedOnBigEnd=false;
	if(cChess->cChessManPosition[0] > 8*16)
		isRedOnBigEnd=true;
	if(cChess->cChessArray[position] < 32)
		isRedChessMan=true;
	if(isRedChessMan == isRedOnBigEnd)
		return position<=7*16+11;
	else
		return position>=8*16+3;
}
//评价威胁指数
//加入藐视因子，正数过于乐观，0正常,负数过于悲观
//敌人过河数、己方守卫数     攻击等级、守卫等级      威胁等级（结合藐视因子）
//对于藐视因子：
//		分为对于自己评价限制(5)、对于他们评价限制(10)
//		即：乐观度大于5才会改变自己局面的评价
//		    悲观度小于-5才会改变自己局面的评价
//		即：乐观度大于5才会改变他人局面的评价
//		    悲观度小于-5才会改变他人局面的评价
ThreatenLevel EvaluateSituation::getThreatenLevel(bool isRed,int subjective) const
{
	ThreatenLevel redThreatenLevel,blackThreatenLevel;
	int advisorNu,bishopNu,knightOnSelfSideNu,rookOnSelfSideNu,cannonOnSelfSideNu,pawnOnSelfSideNu;
	int knightEnemyCrossedNu,rookEnemyCrossedNu,cannonEnemyCrossedNu,pawnEnemyCrossedNu;
	int subjectiveForSelf,subjectiveForOther;
	//计算对于自己以及他人的乐观悲观度
	if(subjective>0)
	{
		if(subjective-subjectiveForSelfScope>0)
			subjectiveForSelf=1;
		else 
			subjectiveForSelf=0;
		if(subjective-subjectiveForOtherScope>0)
			subjectiveForOther=1;
		else
			subjectiveForOther=0;
	}
	else if(subjective==0)
	{
		subjectiveForSelf=0;
		subjectiveForOther=0;
	}
	else
	{
		if(subjective+subjectiveForSelfScope>=0)
			subjectiveForSelf=0;
		else 
			subjectiveForSelf=-1;
		if(subjective+subjectiveForOtherScope>=0)
			subjectiveForOther=0;
		else
			subjectiveForOther=-1;
	}
	//分析局面的守卫数、以及敌人数
	for(int side=0;side<=1;side++)
	{
		//守卫数
		advisorNu=bishopNu=knightOnSelfSideNu=rookOnSelfSideNu=cannonOnSelfSideNu=pawnOnSelfSideNu=0;
		//敌人数
		knightEnemyCrossedNu=rookEnemyCrossedNu=cannonEnemyCrossedNu=pawnEnemyCrossedNu=0;
		//遍历方便使用的偏移量
		int chessManPositionIndexOffForSelf=(side==0?0:16);
		int chessManPositionIndexOffForOther=(side==0?16:0);
		ThreatenLevel temp;
		//敌人的强弱分析
		int strongEnemyNu=0,weakEnemyNu=0,strongGuarderNu=0,mediumGuarderNu=0,weakGuarderNu=0;
		//总体的攻防能力分析
		bool strongAttack=false,mediumAttack=false,weakAttack=false;
		bool strongProtect=false,mediumProtect=false,weakProtect=false;
		for(int i=1+chessManPositionIndexOffForSelf;i<=2+chessManPositionIndexOffForSelf;i++)
			if(cChess->cChessManPosition[i])
				++advisorNu;
		for(int i=3+chessManPositionIndexOffForSelf;i<=4+chessManPositionIndexOffForSelf;i++)
			if(cChess->cChessManPosition[i])
				++bishopNu;
		for(int i=5+chessManPositionIndexOffForSelf;i<=6+chessManPositionIndexOffForSelf;i++)
			if(cChess->cChessManPosition[i] && !isCrossRiver(cChess->cChessManPosition[i]))
				++knightOnSelfSideNu;
		for(int i=7+chessManPositionIndexOffForSelf;i<=8+chessManPositionIndexOffForSelf;i++)
			if(cChess->cChessManPosition[i] && !isCrossRiver(cChess->cChessManPosition[i]))
				++rookOnSelfSideNu;
		for(int i=9+chessManPositionIndexOffForSelf;i<=10+chessManPositionIndexOffForSelf;i++)
			if(cChess->cChessManPosition[i] && !isCrossRiver(cChess->cChessManPosition[i]))
				++cannonOnSelfSideNu;
		for(int i=11+chessManPositionIndexOffForSelf;i<=15+chessManPositionIndexOffForSelf;i++)
			if(cChess->cChessManPosition[i] && !isCrossRiver(cChess->cChessManPosition[i]))
				++pawnOnSelfSideNu;
		for(int i=5+chessManPositionIndexOffForOther;i<=6+chessManPositionIndexOffForOther;i++)
			if(cChess->cChessManPosition[i] && isCrossRiver(cChess->cChessManPosition[i]))
				++knightEnemyCrossedNu;
		for(int i=7+chessManPositionIndexOffForOther;i<=8+chessManPositionIndexOffForOther;i++)
			if(cChess->cChessManPosition[i] && isCrossRiver(cChess->cChessManPosition[i]))
				++rookEnemyCrossedNu;
		for(int i=9+chessManPositionIndexOffForOther;i<=10+chessManPositionIndexOffForOther;i++)
			if(cChess->cChessManPosition[i] && isCrossRiver(cChess->cChessManPosition[i]))
				++cannonEnemyCrossedNu;
		for(int i=10+chessManPositionIndexOffForOther;i<=15+chessManPositionIndexOffForOther;i++)
			if(cChess->cChessManPosition[i] && isCrossRiver(cChess->cChessManPosition[i]))
				++pawnEnemyCrossedNu;
		strongEnemyNu=knightEnemyCrossedNu+rookEnemyCrossedNu+cannonEnemyCrossedNu; 
		weakEnemyNu=pawnEnemyCrossedNu;	
		strongGuarderNu=rookOnSelfSideNu+knightOnSelfSideNu+cannonOnSelfSideNu;
		mediumGuarderNu=advisorNu+bishopNu;
		weakGuarderNu=pawnOnSelfSideNu;
		strongAttack=(	 strongEnemyNu >= 4	||
				(strongEnemyNu >= 2 && strongEnemyNu <= 3 && weakEnemyNu >= 3)
				);
		mediumAttack=(	(strongEnemyNu >= 2 && strongEnemyNu <= 3 && weakEnemyNu <= 2)	||
				(strongEnemyNu <= 1 && weakEnemyNu >= 3)
				);
		weakAttack=(	strongEnemyNu <= 1 && weakEnemyNu <=2
				);
		strongProtect=(	strongGuarderNu >= 4	||
				(strongGuarderNu <=3 && strongGuarderNu >=2 && mediumGuarderNu >= 3)
				);
		mediumProtect=(	(strongGuarderNu <=3 && strongGuarderNu >=2 && mediumGuarderNu <=2 && weakGuarderNu >= 4)	||
				(strongGuarderNu <=1 && mediumGuarderNu == 4)
				);
		weakProtect=(	(strongGuarderNu <=3 && strongGuarderNu >=2 && mediumGuarderNu <=2 && weakGuarderNu <=3)	||
				(strongGuarderNu <=1 && mediumGuarderNu <=3)
				);
		if(weakAttack)
		{
			if(subjective >=0)
				temp=smallThreatenLevel;
			else
				temp=middleThreatenLevel;
		}
		else if(mediumAttack)
		{
			if(strongProtect && subjectiveForSelf>=0) 
				temp=smallThreatenLevel;
			else if(strongProtect && subjectiveForSelf<0)
					temp=middleThreatenLevel;
			else if(mediumProtect)
					temp=middleThreatenLevel;
			else if(weakProtect && subjectiveForSelf>=0)
					temp=middleThreatenLevel;
			else if(weakProtect && subjectiveForSelf<0)
					temp=bigThreatenLevel;
		}
		else if(strongAttack)
		{
			if(strongProtect && subjectiveForSelf==0)
				temp=middleThreatenLevel;
			else if(strongProtect && subjectiveForSelf>0)
				temp=smallThreatenLevel;
			else if(strongProtect && subjectiveForSelf<0)
				temp=bigThreatenLevel;
			else if(mediumProtect && subjectiveForSelf<=0)
				temp=bigThreatenLevel;
			else if(mediumProtect && subjectiveForSelf>0)
				temp=middleThreatenLevel;
			else if(weakProtect)
				temp=bigThreatenLevel;
		}
		if(side==0)
			redThreatenLevel=temp;
		else
			blackThreatenLevel=temp;
	}
	if(isRed)
	{
		switch(redThreatenLevel)
		{
			case smallThreatenLevel:
						if(subjectiveForOther<0 && blackThreatenLevel==smallThreatenLevel)
							redThreatenLevel=middleThreatenLevel;
						break;
			case middleThreatenLevel:
						switch(blackThreatenLevel)
						{
							case smallThreatenLevel:
										break;
							case middleThreatenLevel:
										if(subjectiveForOther>0)
											redThreatenLevel=smallThreatenLevel;
										else if(subjectiveForOther<0)
											redThreatenLevel=bigThreatenLevel;
										break;
							case bigThreatenLevel:
										if(subjectiveForOther>=0)
											redThreatenLevel=smallThreatenLevel;
										break;
						}
						break;
			case bigThreatenLevel:
						if(subjectiveForOther>0 && blackThreatenLevel==bigThreatenLevel)
							redThreatenLevel=middleThreatenLevel;
						break;
		}
		return redThreatenLevel;
	}
	else
	{
		switch(blackThreatenLevel)
		{
			case smallThreatenLevel:
						if(subjectiveForOther<0 && redThreatenLevel==smallThreatenLevel)
							blackThreatenLevel=middleThreatenLevel;
						break;
			case middleThreatenLevel:
						switch(redThreatenLevel)
						{
							case smallThreatenLevel:
										break;
							case middleThreatenLevel:
										if(subjectiveForOther>0)
											blackThreatenLevel=smallThreatenLevel;
										else if(subjectiveForOther<0)
											blackThreatenLevel=bigThreatenLevel;
										break;
							case bigThreatenLevel:
										if(subjectiveForOther>=0)
											blackThreatenLevel=smallThreatenLevel;
										break;
						}
						break;
			case bigThreatenLevel:
						if(subjectiveForOther>0 && redThreatenLevel==bigThreatenLevel)
							blackThreatenLevel=middleThreatenLevel;
						break;
		}
		return blackThreatenLevel;
	}
}
bool EvaluateSituation::isSelfSideChessMan(int source,int position) const
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index ...."<<endl;
		return 0;
	}
	if(cChess->cChessArray[position] ==0)
	{
		cout<<"ERROR:No ChessMan in "<<position<<endl;
		return false;
	}
	bool isPositionRedChessMan=false;
	bool isSourceRedChessMan=false;
	if(cChess->cChessArray[position] <32)
		isPositionRedChessMan=true;
	if(cChess->cChessArray[source]<32)
		isSourceRedChessMan=true;
	return (isSourceRedChessMan == isPositionRedChessMan);
	
}
int EvaluateSituation::getChessManSub(int chessManValue) const
{
	int sub=0;
	switch(chessManValue)
	{
		case redKing:
		case blackKing:
				sub=0;break;
		case redAdvisorOne:
		case redAdvisorTwo:
		case blackAdvisorOne:
		case blackAdvisorTwo:
				sub=1;break;
		case redBishopOne:
		case redBishopTwo:
		case blackBishopOne:
		case blackBishopTwo:
				sub=2;break;
		case redKnightOne:
		case redKnightTwo:
		case blackKnightOne:
		case blackKnightTwo:
				sub=3;break;
		case redRookOne:
		case redRookTwo:
		case blackRookOne:
		case blackRookTwo:
				sub=4;break;
		case redCannonOne:
		case redCannonTwo:
		case blackCannonOne:
		case blackCannonTwo:
				sub=5;break;
		case redPawnOne:
		case redPawnTwo:
		case redPawnThree:
		case redPawnFour:
		case redPawnFive:
		case blackPawnOne:
		case blackPawnTwo:
		case blackPawnThree:
		case blackPawnFour:
		case blackPawnFive:
				sub=6;break;
		default:
				sub=0;break;
	}
	return sub;
}
//-1 0 1
//参数：棋子的值，即枚举值
int EvaluateSituation::compareChessManCheckAndBalance(int firstChessManValue,int secondChessManValue) const
{
	if(firstChessManValue<16 || firstChessManValue>47 || secondChessManValue<16 || secondChessManValue>47)
		return 0;
	return chessManCheckAndBalanceValue[getChessManSub(firstChessManValue)][getChessManSub(secondChessManValue)];
}
void EvaluateSituation::knightDynamicPosEvaluate(int position,int* flexibility,int* killScore,int* checkAndBalance) const
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index..."<<endl;
		return;
	}
	if(cChess->cChessArray[position] != redKnightOne &&
	   cChess->cChessArray[position] != redKnightTwo &&
	   cChess->cChessArray[position] != blackKnightOne &&
	   cChess->cChessArray[position] != blackKnightTwo)
	{
		cout<<"ERROR:Not Is Knight in "<<position<<endl;
		return;
	}
	if(!isInLegalPosition(position,knightLegalPosVal,position))
		return;
	int indexForThreaten=0;
	int dest,leg;
	int selfValue,destValue,destValueIndex;
	if(evaluatedThreatenLevel == middleThreatenLevel)
		indexForThreaten=1;
	else if(evaluatedThreatenLevel == bigThreatenLevel)
			indexForThreaten=2;
	for(int i=0;i<8;i++)
	{
		dest=position+knightMoveOff[i];
		leg=position+knightLeg[i];
		if(!isInLegalPosition(position,knightLegalPosVal,dest))
			continue;
		if(cChess->cChessArray[leg] !=0)
			continue;
		if(cChess->cChessArray[dest]!=0)
		{
			selfValue=cChess->cChessArray[position];
			destValue=cChess->cChessArray[dest];
			destValueIndex=(destValue<32?destValue-16:destValue-32);
			if(!isSelfSideChessMan(position,dest))
			{
				*killScore=*killScore+chessManImportanceValue[destValueIndex];
				*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][5];
			}
			else
			{
				*checkAndBalance=*checkAndBalance+compareChessManCheckAndBalance(selfValue,destValue);
			}
		}
		else
			*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][5];
	}
}
void EvaluateSituation::bishopDynamicPosEvaluate(int position,int* flexibility,int* killScore,int* checkAndBalance) const
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index..."<<endl;
		return;
	}
	if(cChess->cChessArray[position] != redBishopOne &&
	   cChess->cChessArray[position] != redBishopTwo &&
	   cChess->cChessArray[position] != blackBishopOne &&
	   cChess->cChessArray[position] != blackBishopTwo)
	{
		cout<<"ERROR:Not Is Bishop in "<<position<<endl;
		return; 
	}
	if(!isInLegalPosition(position,bishopLegalPosVal,position))
		return;
	int indexForThreaten=0;
	int dest,eye;
	int selfValue,destValue,destValueIndex;
	if(evaluatedThreatenLevel == middleThreatenLevel)
		indexForThreaten=1;
	else if(evaluatedThreatenLevel == bigThreatenLevel)
			indexForThreaten=2;
	for(int i=0;i<4;i++)
	{
		dest=position+bishopMoveOff[i];
		eye=position+bishopEye[i];
		if(!isInLegalPosition(position,bishopLegalPosVal,dest))
			continue;
		if(cChess->cChessArray[eye] != 0)
			continue;
		if(cChess->cChessArray[dest]!=0)
		{
			selfValue=cChess->cChessArray[position];
			destValue=cChess->cChessArray[dest];
			destValueIndex=(destValue<32?destValue-16:destValue-32);
			if(!isSelfSideChessMan(position,dest))
			{
				*killScore=*killScore+chessManImportanceValue[destValueIndex];
				*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][3];
			}
			else
			{
				*checkAndBalance=*checkAndBalance+compareChessManCheckAndBalance(selfValue,destValue);
			}
		}
		else
			*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][3];
	}
}
void EvaluateSituation::advisorDynamicPosEvaluate(int position,int* flexibility,int* killScore,int* checkAndBalance) const
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index..."<<endl;
		return;
	}
	if(cChess->cChessArray[position] != redAdvisorOne &&
	   cChess->cChessArray[position] != redAdvisorTwo &&
	   cChess->cChessArray[position] != blackAdvisorOne &&
	   cChess->cChessArray[position] != blackAdvisorTwo)
	{
		cout<<"ERROR:Not Is Advisor in"<<position<<endl;
		return; 
	}
	if(!isInLegalPosition(position,advisorLegalPosVal,position))
		return;
	int indexForThreaten=0;
	int dest;
	int selfValue,destValue,destValueIndex;
	if(evaluatedThreatenLevel == middleThreatenLevel)
		indexForThreaten=1;
	else if(evaluatedThreatenLevel == bigThreatenLevel)
			indexForThreaten=2;
	for(int i=0;i<4;i++)
	{
		dest=position+advisorMoveOff[i];
		if(!isInLegalPosition(position,advisorLegalPosVal,dest))
			continue;
		if(cChess->cChessArray[dest] != 0)
		{
			selfValue=cChess->cChessArray[position];
			destValue=cChess->cChessArray[dest];
			destValueIndex=(destValue<32?destValue-16:destValue-32);
			if(!isSelfSideChessMan(position,dest))
			{
				*killScore=*killScore+chessManImportanceValue[destValueIndex];
				*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][1];
			}
			else
			{
				*checkAndBalance=*checkAndBalance+compareChessManCheckAndBalance(selfValue,destValue);
			}
		}
		else
			*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][1];
	}
}
void EvaluateSituation::kingDynamicPosEvaluate(int position,int* flexibility,int* killScore,int* checkAndBalance) const
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index..."<<endl;
		return;
	}
	if(cChess->cChessArray[position] != redKing &&
	   cChess->cChessArray[position] != blackKing)
	{
		cout<<"ERROR:Not Is King in"<<position<<endl;
		return;
	}
	if(!isInLegalPosition(position,kingLegalPosVal,position))
		return;
	int indexForThreaten=0;
	int dest;
	int selfValue,destValue,destValueIndex;
	if(evaluatedThreatenLevel == middleThreatenLevel)
		indexForThreaten=1;
	else if(evaluatedThreatenLevel == bigThreatenLevel)
			indexForThreaten=2;
	for(int i=0;i<4;i++)
	{
		dest=position+kingMoveOff[i];
		if(!isInLegalPosition(position,kingLegalPosVal,dest))
			continue;
		if(cChess->cChessArray[dest]!=0)
		{
			selfValue=cChess->cChessArray[position];
			destValue=cChess->cChessArray[dest];
			destValueIndex=(destValue<32?destValue-16:destValue-32);
			if(!isSelfSideChessMan(position,dest))
			{
				*killScore=*killScore+chessManImportanceValue[destValueIndex];
				*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][0];
			}
			else
			{
				*checkAndBalance=*checkAndBalance+compareChessManCheckAndBalance(selfValue,destValue);
			}
		}
		else
			*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][0];
	}
}
void EvaluateSituation::rookDynamicPosEvaluate(int position,int* flexibility,int* killScore,int* checkAndBalance) const
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index..."<<endl;
		return;
	}
	if(cChess->cChessArray[position] != redRookOne &&
	   cChess->cChessArray[position] != redRookTwo &&
	   cChess->cChessArray[position] != blackRookOne &&
	   cChess->cChessArray[position] != blackRookTwo)
	{
		cout<<"ERROR:Not Is Rook in"<<position<<endl;
		return; 
	}
	if(!isInLegalPosition(position,rookLegalPosVal,position))
		return;
	int indexForThreaten=0;
	int dest;
	int selfValue,destValue,destValueIndex;
	if(evaluatedThreatenLevel == middleThreatenLevel)
		indexForThreaten=1;
	else if(evaluatedThreatenLevel == bigThreatenLevel)
			indexForThreaten=2;
	for(int i=0;i<4;i++)
	{
		dest=position+rookMoveOff[i];
		while(isInLegalPosition(position,rookLegalPosVal,dest))
		{
			if(cChess->cChessArray[dest]!=0)
			{
				selfValue=cChess->cChessArray[position];
				destValue=cChess->cChessArray[dest];
				destValueIndex=(destValue<32?destValue-16:destValue-32);
				if(!isSelfSideChessMan(position,dest))
				{
					*killScore=*killScore+chessManImportanceValue[destValueIndex];
					*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][7];
				}
				else
				{
					*checkAndBalance=*checkAndBalance+compareChessManCheckAndBalance(selfValue,destValue);
				}
				break;
			}
			else
				*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][7];
			dest+=rookMoveOff[i];
		}
	}
}
//对跑架子进行评估
void EvaluateSituation::cannonDynamicPosEvaluate(int position,int* flexibility,int* killScore,int* checkAndBalance) const
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index..."<<endl;
		return;
	}
	if(cChess->cChessArray[position] != redCannonOne &&
	   cChess->cChessArray[position] != redCannonTwo &&
	   cChess->cChessArray[position] != blackCannonOne &&
	   cChess->cChessArray[position] != blackCannonTwo)
	{
		cout<<"ERROR:Not Is Cannon in"<<position<<endl;
		return; 
	}
	if(!isInLegalPosition(position,cannonLegalPosVal,position))
		return;
	int indexForThreaten=0;
	int dest;
	int selfValue,destValue,destValueIndex;
	if(evaluatedThreatenLevel == middleThreatenLevel)
		indexForThreaten=1;
	else if(evaluatedThreatenLevel == bigThreatenLevel)
			indexForThreaten=2;
	for(int i=0;i<4;i++)
	{
		dest=position+cannonMoveOff[i];
		while(isInLegalPosition(position,cannonLegalPosVal,dest))
		{
			if(cChess->cChessArray[dest] !=0 )
			{
				int middleChessPosition=dest;
				int middleChessValue=(isSelfSideChessMan(position,middleChessPosition)?2:-2);
				dest+=cannonMoveOff[i];
				while(isInLegalPosition(position,cannonLegalPosVal,dest))
				{
					if(cChess->cChessArray[dest]!=0)
					{
						selfValue=cChess->cChessArray[position];
						destValue=cChess->cChessArray[dest];
						destValueIndex=(destValue<32?destValue-16:destValue-32);
						if(!isSelfSideChessMan(position,dest))
						{
							*killScore=*killScore+chessManImportanceValue[destValueIndex]+middleChessValue;
							*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][9];
						}
						else
						{
							*checkAndBalance=*checkAndBalance+
								compareChessManCheckAndBalance(selfValue,destValue)+middleChessValue;
						}
						break;
					}
					dest+=cannonMoveOff[i];
				}
				break;
			}
			else
				*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][9];
			dest+=cannonMoveOff[i];
		}
	}
}
void EvaluateSituation::pawnDynamicPosEvaluate(int position,int* flexibility,int* killScore,int* checkAndBalance) const
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index..."<<endl;
		return;
	}
	if(cChess->cChessArray[position] < redPawnOne ||
	   cChess->cChessArray[position] > redPawnFive &&
	   cChess->cChessArray[position] < blackPawnOne ||
	   cChess->cChessArray[position] > blackPawnFive)
	{
		cout<<"ERROR:Not Is Pawn in"<<position<<endl;
		return;
	}
	if(!isInLegalPosition(position,pawnLegalPosVal,position))
		return;
	int indexForThreaten=0,indexForMove;
	bool isRedChessMan=false,isRedOnBigEnd=false;
	int dest;
	int selfValue,destValue,destValueIndex;
	if(evaluatedThreatenLevel == middleThreatenLevel)
		indexForThreaten=1;
	else if(evaluatedThreatenLevel == bigThreatenLevel)
			indexForThreaten=2;
	if(cChess->cChessArray[position] < 32)
		isRedChessMan=true;
	if(cChess->cChessManPosition[0] > 8*16)
		isRedOnBigEnd=true;
	indexForMove=(isRedChessMan==isRedOnBigEnd?0:1);
	if(isCrossRiver(position))
	{
		for(int i=0;i<3;i++)
		{
			dest=position+pawnMoveOff[indexForMove][i];
			if(!isInLegalPosition(position,pawnLegalPosVal,dest))
				continue;
			if(cChess->cChessArray[dest]!=0)
			{
				selfValue=cChess->cChessArray[position];
				destValue=cChess->cChessArray[dest];
				destValueIndex=(destValue<32?destValue-16:destValue-32);
				if(!isSelfSideChessMan(position,dest))
				{
					*killScore=*killScore+chessManImportanceValue[destValueIndex];
					*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][11];
				}
				else
				{
					*checkAndBalance=*checkAndBalance+compareChessManCheckAndBalance(selfValue,destValue);
				}
			}
			else
			{
				*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][11];
			}
		}
	}
	else
	{
		if(indexForMove==0)
			dest=position-16;
		else
			dest=position+16;
		if(!isInLegalPosition(position,pawnLegalPosVal,dest))
			return;
		if(cChess->cChessArray[dest]==0)
			*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][11];
		else
		{
				selfValue=cChess->cChessArray[position];
				destValue=cChess->cChessArray[dest];
				destValueIndex=(destValue<32?destValue-16:destValue-32);
				if(!isSelfSideChessMan(position,dest))
				{
					*killScore=*killScore+chessManImportanceValue[destValueIndex];
					*flexibility=*flexibility+chessManFlexibilityValue[indexForThreaten][11];
				}
				else
				{
					*checkAndBalance=*checkAndBalance+compareChessManCheckAndBalance(selfValue,destValue);
				}
		}
		
	}
}
//评估棋子的灵活性、吃子效率、牵制
//走子灵活加分
//吃子对比分值加分,这里给吃将特别的加分
//牵制更具分值加分
double EvaluateSituation::chessManDynamicPosEvaluate() 
{
	int redFlexibility=0,blackFlexibility=0;
	int redKillScore=0,blackKillScore=0;
	int redCheckAndBalance=0,blackCheckAndBalance=0;
	int redEvaluateScore=0,blackEvaluateScore=0;
	int position=0;
	chessManEvaluateFunPtr funPtrArray[16]={
					&EvaluateSituation::kingDynamicPosEvaluate,&EvaluateSituation::advisorDynamicPosEvaluate,
					&EvaluateSituation::advisorDynamicPosEvaluate,&EvaluateSituation::bishopDynamicPosEvaluate,
					&EvaluateSituation::bishopDynamicPosEvaluate,&EvaluateSituation::knightDynamicPosEvaluate,
					&EvaluateSituation::knightDynamicPosEvaluate,&EvaluateSituation::rookDynamicPosEvaluate,
					&EvaluateSituation::rookDynamicPosEvaluate,&EvaluateSituation::cannonDynamicPosEvaluate,
					&EvaluateSituation::cannonDynamicPosEvaluate,&EvaluateSituation::pawnDynamicPosEvaluate,
					&EvaluateSituation::pawnDynamicPosEvaluate,&EvaluateSituation::pawnDynamicPosEvaluate,
					&EvaluateSituation::pawnDynamicPosEvaluate,&EvaluateSituation::pawnDynamicPosEvaluate
						};
	for(int i=0;i<16;i++)
	{
		this->evaluatedThreatenLevel=getThreatenLevel(true,subjectiveOfPlayer);
		position=cChess->cChessManPosition[i];
		if(position!=0)
			(this->*funPtrArray[i])(cChess->cChessManPosition[i],&redFlexibility,&redKillScore,&redCheckAndBalance);
	}
	for(int i=16;i<32;i++)
	{
		this->evaluatedThreatenLevel=getThreatenLevel(false,subjectiveOfPlayer);
		position=cChess->cChessManPosition[i];
		if(position!=0)
			(this->*funPtrArray[i-16])(cChess->cChessManPosition[i],&blackFlexibility,&blackKillScore,&blackCheckAndBalance);
	}
	//这里如果考虑各自的威胁级别，就会造成红黑方选择的不一样的选项
	//这对于参数的设置要求很严格，所以此处就顺加入
	if(this->knowledgeLevel >= dynamicFirstLevel)
	{
		redEvaluateScore+=redFlexibility;
		blackEvaluateScore+=blackFlexibility;
	}
	if(this->knowledgeLevel >= dynamicSecondLevel)
	{
		redEvaluateScore+=redKillScore;
		blackEvaluateScore+=blackKillScore;
	}
	if(this->knowledgeLevel >= dynamicThirdLevel)
	{
		redEvaluateScore+=redCheckAndBalance;
		blackEvaluateScore+=blackCheckAndBalance;
	}
	if(cChess->isRed)
		return (double)(redEvaluateScore-blackEvaluateScore);
	else
		return (double)(blackEvaluateScore-redEvaluateScore);
}
int EvaluateSituation::abs(int value) const
{
	if(value<0)
		value=-value;
	return value;
}
//在调用函数时已经调用了动态、静态评估函数
//关键是已经对局面进行了评估威胁等级
double EvaluateSituation::particularFormEvaluate() const
{
	double redParticularFormScore=0,blackParticularFormScore=0;
	//其中一些简单的配合，如兵联手、连环马、担子炮、霸王车已经在牵制评估中加以考虑，这里就可以忽略了
	//一些车炮的配合
	//1、车炮将调车抽子
	//	条件：在一条直线上且之间无任何棋子且对方将帅无法避免
	int rookPosition=0,cannonPosition=0,enemyKingPosition=0;
	enemyKingPosition=cChess->cChessManPosition[16];
	for(int i=7;i<=8;i++)
	{
		rookPosition=cChess->cChessManPosition[i];
		for(int j=9;j<=10;j++)
		{
			cannonPosition=cChess->cChessManPosition[j];
			//车在中间
			if((rookPosition-cannonPosition)*(rookPosition-enemyKingPosition)>=0)
				continue;
			//同一行
			if(enemyKingPosition/16==rookPosition/16 && enemyKingPosition/16==cannonPosition/16)
			{
				//检测中间是否有其他棋子
				int c2koff=(enemyKingPosition>cannonPosition?1:-1);
				bool hasOtherChessMan=false;
				for(int mid=cannonPosition+c2koff;mid!=rookPosition;mid+=c2koff)
				{
					if(cChess->cChessArray[mid] !=0)
					{
						hasOtherChessMan=true;
						break;
					}
				}
				if(hasOtherChessMan)
					continue;
				for(int mid=rookPosition+c2koff;mid!=enemyKingPosition;mid+=c2koff)
				{
					if(cChess->cChessArray[mid] !=0)
					{
						hasOtherChessMan=true;
						break;
					}
				}
				if(hasOtherChessMan)
					continue;
				//判断将垂直方向的逃路
				int kingEscape=enemyKingPosition+16;
				if(isInLegalPosition(enemyKingPosition,kingLegalPosVal,kingEscape) && 
				   !(cChess->cChessArray[kingEscape]!=0 && isSelfSideChessMan(enemyKingPosition,kingEscape)))	
					continue;
				kingEscape=enemyKingPosition-16;
				if(isInLegalPosition(enemyKingPosition,kingLegalPosVal,kingEscape) && 
				   !(cChess->cChessArray[kingEscape]!=0 && isSelfSideChessMan(enemyKingPosition,kingEscape)))	
					continue;
				//无法逃脱，则为红方加分
				redParticularFormScore+=30;
			}
			if(enemyKingPosition%16==rookPosition%16 && enemyKingPosition%16==cannonPosition%16) 
			{
				//检测中间是否有其他棋子
				int c2koff=(enemyKingPosition>cannonPosition?16:-16);
				bool hasOtherChessMan=false;
				for(int mid=cannonPosition+c2koff;mid!=rookPosition;mid+=c2koff)
				{
					if(cChess->cChessArray[mid] !=0)
					{
						hasOtherChessMan=true;
						break;
					}
				}
				if(hasOtherChessMan)
					continue;
				for(int mid=rookPosition+c2koff;mid!=enemyKingPosition;mid+=c2koff)
				{
					if(cChess->cChessArray[mid] !=0)
					{
						hasOtherChessMan=true;
						break;
					}
				}
				if(hasOtherChessMan)
					continue;
				//判断将水平方向的逃路
				int kingEscape=enemyKingPosition+1;
				if(isInLegalPosition(enemyKingPosition,kingLegalPosVal,kingEscape) && 
				   !(cChess->cChessArray[kingEscape]!=0 && isSelfSideChessMan(enemyKingPosition,kingEscape)))	
					continue;
				kingEscape=enemyKingPosition-1;
				if(isInLegalPosition(enemyKingPosition,kingLegalPosVal,kingEscape) && 
				   !(cChess->cChessArray[kingEscape]!=0 && isSelfSideChessMan(enemyKingPosition,kingEscape)))	
					continue;
				//无法逃脱，则为红方加分
				redParticularFormScore+=30;
			}
		}
	}
	enemyKingPosition=cChess->cChessManPosition[0];
	for(int i=23;i<=24;i++)
	{
		rookPosition=cChess->cChessManPosition[i];
		for(int j=25;j<=26;j++)
		{
			cannonPosition=cChess->cChessManPosition[j];
			//车在中间
			if((rookPosition-cannonPosition)*(rookPosition-enemyKingPosition)>=0)
				continue;
			//同一行
			if(enemyKingPosition/16==rookPosition/16 && enemyKingPosition/16==cannonPosition/16)
			{
				//检测中间是否有其他棋子
				int c2koff=(enemyKingPosition>cannonPosition?1:-1);
				bool hasOtherChessMan=false;
				for(int mid=cannonPosition+c2koff;mid!=rookPosition;mid+=c2koff)
				{
					if(cChess->cChessArray[mid] !=0)
					{
						hasOtherChessMan=true;
						break;
					}
				}
				if(hasOtherChessMan)
					continue;
				for(int mid=rookPosition+c2koff;mid!=enemyKingPosition;mid+=c2koff)
				{
					if(cChess->cChessArray[mid] !=0)
					{
						hasOtherChessMan=true;
						break;
					}
				}
				if(hasOtherChessMan)
					continue;
				//判断帅垂直方向的逃路
				int kingEscape=enemyKingPosition+16;
				if(isInLegalPosition(enemyKingPosition,kingLegalPosVal,kingEscape) && 
				   !(cChess->cChessArray[kingEscape]!=0 && isSelfSideChessMan(enemyKingPosition,kingEscape)))	
					continue;
				kingEscape=enemyKingPosition-16;
				if(isInLegalPosition(enemyKingPosition,kingLegalPosVal,kingEscape) && 
				   !(cChess->cChessArray[kingEscape]!=0 && isSelfSideChessMan(enemyKingPosition,kingEscape)))	
					continue;
				//无法逃脱，则为黑方加分
				blackParticularFormScore+=30;
			}
			if(enemyKingPosition%16==rookPosition%16 && enemyKingPosition%16==cannonPosition%16) 
			{
				//检测中间是否有其他棋子
				int c2koff=(enemyKingPosition>cannonPosition?16:-16);
				bool hasOtherChessMan=false;
				for(int mid=cannonPosition+c2koff;mid!=rookPosition;mid+=c2koff)
				{
					if(cChess->cChessArray[mid] !=0)
					{
						hasOtherChessMan=true;
						break;
					}
				}
				if(hasOtherChessMan)
					continue;
				for(int mid=rookPosition+c2koff;mid!=enemyKingPosition;mid+=c2koff)
				{
					if(cChess->cChessArray[mid] !=0)
					{
						hasOtherChessMan=true;
						break;
					}
				}
				if(hasOtherChessMan)
					continue;
				//判断帅水平方向的逃路
				int kingEscape=enemyKingPosition+1;
				if(isInLegalPosition(enemyKingPosition,kingLegalPosVal,kingEscape) && 
				   !(cChess->cChessArray[kingEscape]!=0 && isSelfSideChessMan(enemyKingPosition,kingEscape)))	
					continue;
				kingEscape=enemyKingPosition-1;
				if(isInLegalPosition(enemyKingPosition,kingLegalPosVal,kingEscape) && 
				   !(cChess->cChessArray[kingEscape]!=0 && isSelfSideChessMan(enemyKingPosition,kingEscape)))	
					continue;
				//无法逃脱，则为黑方加分
				blackParticularFormScore+=30;
			}
		}
	}
	//2、车沉底炮碾丹砂
	//	条件：将士原位，车炮底线，且车炮不被攻击
	int redBottomLine=0,blackBottomLine=0;
	int advisorOnePosition=0,advisorTwoPosition=0;
	int factor;
	bool isRedOnBigEnd=false;
	if(cChess->cChessManPosition[0] > 8*16)
		isRedOnBigEnd=true;
	if(isRedOnBigEnd)
	{
		redBottomLine=12;
		blackBottomLine=3;
		factor=1;
	}
	else
	{
		redBottomLine=3;
		blackBottomLine=12;
		factor=-1;
	}
	enemyKingPosition=cChess->cChessManPosition[16];
	//对方将在原始位置
	if(enemyKingPosition==blackBottomLine*16+7)
	{
		advisorOnePosition=cChess->cChessManPosition[17];
		advisorTwoPosition=cChess->cChessManPosition[18];
		if(advisorOnePosition!=0 && advisorTwoPosition!=0 && 
		   abs(advisorOnePosition-enemyKingPosition)==1 && 
		   abs(advisorTwoPosition-enemyKingPosition)==1 )
		{
			for(int i=7;i<=8;i++)
			{
				rookPosition=cChess->cChessManPosition[i];
				if(rookPosition/16 != blackBottomLine)
					continue;
				if(abs(rookPosition-enemyKingPosition)==4)
					continue;
				for(int j=9;j<=10;j++)
				{
					cannonPosition=cChess->cChessManPosition[j];
					if(cannonPosition/16 != blackBottomLine)
						continue;
					if((rookPosition-enemyKingPosition)*(rookPosition-cannonPosition)>=0)
						continue;
					//检测之间是否有棋子
					int off=(rookPosition>enemyKingPosition?1:-1);
					bool hasOtherChessMan=false;
					for(int mid=enemyKingPosition+off;mid!=rookPosition;mid+=off)
					{
						if(cChess->cChessArray[mid]!=0)
						{
							hasOtherChessMan=true;
							break;
						}
					}
					if(hasOtherChessMan)
						continue;
					for(int mid=rookPosition+off;mid!=cannonPosition;mid+=off)
					{
						if(cChess->cChessArray[mid]!=0)
						{
							hasOtherChessMan=true;
							break;
						}
					}
					if(hasOtherChessMan)
						continue;
					//检测当前的形式是否稳定，并进行一定的评估
					//马的威胁
					if(cChess->cChessArray[rookPosition+14*factor]==blackKnightOne || 
				   	   cChess->cChessArray[rookPosition+14*factor]==blackKnightTwo ||
				   	   cChess->cChessArray[rookPosition+31*factor]==blackKnightOne ||
				   	   cChess->cChessArray[rookPosition+31*factor]==blackKnightTwo ||
				   	   cChess->cChessArray[rookPosition+33*factor]==blackKnightOne ||
				  	   cChess->cChessArray[rookPosition+33*factor]==blackKnightTwo ||
				   	   cChess->cChessArray[rookPosition+18*factor]==blackKnightOne ||
				   	   cChess->cChessArray[rookPosition+18*factor]==blackKnightTwo)
						continue;
					if(cChess->cChessArray[cannonPosition+14*factor]==blackKnightOne || 
				   	   cChess->cChessArray[cannonPosition+14*factor]==blackKnightTwo ||
				   	   cChess->cChessArray[cannonPosition+31*factor]==blackKnightOne ||
				   	   cChess->cChessArray[cannonPosition+31*factor]==blackKnightTwo ||
				   	   cChess->cChessArray[cannonPosition+33*factor]==blackKnightOne ||
				  	   cChess->cChessArray[cannonPosition+33*factor]==blackKnightTwo ||
				   	   cChess->cChessArray[cannonPosition+18*factor]==blackKnightOne ||
				   	   cChess->cChessArray[cannonPosition+18*factor]==blackKnightTwo)
						continue;
					//象的威胁
					if(cChess->cChessArray[rookPosition+30*factor]==blackBishopOne ||
				   	   cChess->cChessArray[rookPosition+30*factor]==blackBishopTwo ||
				   	   cChess->cChessArray[rookPosition+34*factor]==blackBishopOne ||
				   	   cChess->cChessArray[rookPosition+34*factor]==blackBishopTwo)
						continue;
					//车的威胁
					int blackRook=cChess->cChessManPosition[23];
					hasOtherChessMan=false;
					if(blackRook%16 == rookPosition%16)
					{
						for(int i=blackRook-16*factor;i!=rookPosition;i-=16*factor)
						{
							if(cChess->cChessArray[i]!=0)
							{
								hasOtherChessMan=true;
								break;
							}
						}
						if(!hasOtherChessMan)
						{
							//此处对车，加分
							redParticularFormScore+=20;
							break;
						}
					}
					blackRook=cChess->cChessManPosition[24];
					hasOtherChessMan=false;
					if(blackRook%16 == rookPosition%16)
					{
						for(int i=blackRook-16*factor;i!=rookPosition;i-=16*factor)
						{
							if(cChess->cChessArray[i]!=0)
							{
								hasOtherChessMan=true;
								break;
							}
						}
						if(!hasOtherChessMan)
						{
							//此处对车，加分
							redParticularFormScore+=20;
							break;
						}
					}
					for(int i=23;i<=24;i++)
					{
						blackRook=cChess->cChessManPosition[24];
						hasOtherChessMan=false;
						if(blackRook%16 == cannonPosition%16)
						{
							for(int i=blackRook-16*factor;i!=cannonPosition;i-=16*factor)
							{
								if(cChess->cChessArray[i]!=0)
								{
									hasOtherChessMan=true;
									break;
								}
							}
							if(!hasOtherChessMan)
							{
								if(abs(rookPosition-enemyKingPosition)==3)
									continue;
							}
						}
					}
					//炮的威胁
					int blackCannon=cChess->cChessManPosition[25];
					hasOtherChessMan=false;
					if(blackCannon%16 == rookPosition%16)
					{
						for(int i=blackCannon-16*factor;i!=rookPosition;i-=16*factor)
						{
							if(cChess->cChessArray[i]!=0)
							{
								hasOtherChessMan=true;
								break;
							}
						}
						if(!hasOtherChessMan)
						{
							//此处献炮，加分
							redParticularFormScore+=10;
							break;
						}
						else
							//有炮架，形式被毁
							break;
					}
					blackCannon=cChess->cChessManPosition[26];
					hasOtherChessMan=false;
					if(blackCannon%16 == rookPosition%16)
					{
						for(int i=blackCannon-16*factor;i!=rookPosition;i-=16*factor)
						{
							if(cChess->cChessArray[i]!=0)
							{
								hasOtherChessMan=true;
								break;
							}
						}
						if(!hasOtherChessMan)
						{
							//此处献炮，加分
							redParticularFormScore+=10;
							break;
						}
						else
							//有炮架，形式被毁
							break;
					}
					for(int i=25;i<=26;i++)
					{
						blackCannon=cChess->cChessManPosition[i];
						hasOtherChessMan=false;
						if(blackCannon%16 == cannonPosition%16)
						{
							for(int i=blackCannon-16*factor;i!=cannonPosition;i-=16*factor)
							{
								if(cChess->cChessArray[i]!=0)
								{
									hasOtherChessMan=true;
									break;
								}
							}
							if(hasOtherChessMan)
								break;
						}
					}
					//确认可以加分
					redParticularFormScore+=35;
				}
			}
		}
	} 
	enemyKingPosition=cChess->cChessManPosition[0];
	if(enemyKingPosition==redBottomLine*16+7)
	{
		advisorOnePosition=cChess->cChessManPosition[1];
		advisorTwoPosition=cChess->cChessManPosition[2];
		if(advisorOnePosition!=0 && advisorTwoPosition!=0 && 
		   abs(advisorOnePosition-enemyKingPosition)==1 && 
		   abs(advisorTwoPosition-enemyKingPosition)==1)
		{
			for(int i=23;i<=24;i++)
			{
				rookPosition=cChess->cChessManPosition[i];
				if(rookPosition/16 != redBottomLine)
					continue;
				for(int j=25;j<=26;j++)
				{
					cannonPosition=cChess->cChessManPosition[j];
					if(cannonPosition/16 != redBottomLine)
						continue;
					if((rookPosition-enemyKingPosition)*(rookPosition-cannonPosition)>=0)
						continue;
					//检测之间是否有棋子
					int off=(rookPosition>enemyKingPosition?1:-1);
					bool hasOtherChessMan=false;
					for(int mid=enemyKingPosition+off;mid!=rookPosition;mid+=off)
					{
						if(cChess->cChessArray[mid]!=0)
						{
							hasOtherChessMan=true;
							break;
						}
					}
					if(hasOtherChessMan)
						continue;
					for(int mid=rookPosition+off;mid!=cannonPosition;mid+=off)
					{
						if(cChess->cChessArray[mid]!=0)
						{
							hasOtherChessMan=true;
							break;
						}
					}
					if(hasOtherChessMan)
						continue;
					//马的威胁
					if(cChess->cChessArray[rookPosition-14*factor]==redKnightOne || 
				   	   cChess->cChessArray[rookPosition-14*factor]==redKnightTwo ||
				   	   cChess->cChessArray[rookPosition-31*factor]==redKnightOne ||
				   	   cChess->cChessArray[rookPosition-31*factor]==redKnightTwo ||
				   	   cChess->cChessArray[rookPosition-33*factor]==redKnightOne ||
				  	   cChess->cChessArray[rookPosition-33*factor]==redKnightTwo ||
				   	   cChess->cChessArray[rookPosition-18*factor]==redKnightOne ||
				   	   cChess->cChessArray[rookPosition-18*factor]==redKnightTwo)
						continue;
					if(cChess->cChessArray[cannonPosition-14*factor]==redKnightOne || 
				   	   cChess->cChessArray[cannonPosition-14*factor]==redKnightTwo ||
				   	   cChess->cChessArray[cannonPosition-31*factor]==redKnightOne ||
				   	   cChess->cChessArray[cannonPosition-31*factor]==redKnightTwo ||
				   	   cChess->cChessArray[cannonPosition-33*factor]==redKnightOne ||
				  	   cChess->cChessArray[cannonPosition-33*factor]==redKnightTwo ||
				   	   cChess->cChessArray[cannonPosition-18*factor]==redKnightOne ||
				   	   cChess->cChessArray[cannonPosition-18*factor]==redKnightTwo)
						continue;
					//象的威胁
					if(cChess->cChessArray[rookPosition-30*factor]==redBishopOne ||
				   	   cChess->cChessArray[rookPosition-30*factor]==redBishopTwo ||
				   	   cChess->cChessArray[rookPosition-34*factor]==redBishopOne ||
				   	   cChess->cChessArray[rookPosition-34*factor]==redBishopTwo)
						continue;
					//车的威胁
					int redRook=cChess->cChessManPosition[7];
					hasOtherChessMan=false;
					if(redRook%16 == rookPosition%16)
					{
						for(int i=redRook+16*factor;i!=rookPosition;i+=16*factor)
						{
							if(cChess->cChessArray[i]!=0)
							{
								hasOtherChessMan=true;
								break;
							}
						}
						if(!hasOtherChessMan)
						{
							//此处对车，加分
							blackParticularFormScore+=20;
							break;
						}
					}
					redRook=cChess->cChessManPosition[8];
					hasOtherChessMan=false;
					if(redRook%16 == rookPosition%16)
					{
						for(int i=redRook+16*factor;i!=rookPosition;i+=16*factor)
						{
							if(cChess->cChessArray[i]!=0)
							{
								hasOtherChessMan=true;
								break;
							}
						}
						if(!hasOtherChessMan)
						{
							//此处对车，加分
							blackParticularFormScore+=20;
							break;
						}
					}
					for(int i=7;i<=8;i++)
					{
						redRook=cChess->cChessManPosition[8];
						hasOtherChessMan=false;
						if(redRook%16 == cannonPosition%16)
						{
							for(int i=redRook+16*factor;i!=cannonPosition;i+=16*factor)
							{
								if(cChess->cChessArray[i]!=0)
								{
									hasOtherChessMan=true;
									break;
								}
							}
							if(!hasOtherChessMan)
							{
								if(abs(rookPosition-enemyKingPosition)==3)
									break;
							}
						}
					}
					//炮的威胁
					int redCannon=cChess->cChessManPosition[9];
					hasOtherChessMan=false;
					if(redCannon%16 == rookPosition%16)
					{
						for(int i=redCannon+16*factor;i!=rookPosition;i+=16*factor)
						{
							if(cChess->cChessArray[i]!=0)
							{
								hasOtherChessMan=true;
								break;
							}
						}
						if(!hasOtherChessMan)
						{
							//此处献炮，加分
							blackParticularFormScore+=10;
							break;
						}
						else
							//有炮架，形式被毁
							break;
					}
					redCannon=cChess->cChessManPosition[10];
					hasOtherChessMan=false;
					if(redCannon%16 == rookPosition%16)
					{
						for(int i=redCannon+16*factor;i!=rookPosition;i+=16*factor)
						{
							if(cChess->cChessArray[i]!=0)
							{
								hasOtherChessMan=true;
								break;
							}
						}
						if(!hasOtherChessMan)
						{
							//此处献炮，加分
							blackParticularFormScore+=10;
							break;
						}
						else
							//有炮架，形式被毁
							break;
					}
					for(int i=9;i<=10;i++)
					{
						redCannon=cChess->cChessManPosition[10];
						hasOtherChessMan=false;
						if(redCannon%16 == cannonPosition%16)
						{
							for(int i=redCannon+16*factor;i!=cannonPosition;i+=16*factor)
							{
								if(cChess->cChessArray[i]!=0)
								{
									hasOtherChessMan=true;
									break;
								}
							}
							if(hasOtherChessMan)
							{
								break;
							}
						}
					}
					//加分
					blackParticularFormScore+=35;
				}
			}
		}
	}
	//3、帅仕形状引发的空头炮、沉底炮、窝心马、车封边路
	//考虑到这些形式均是由比较灵活的炮、车造成的，可以忽略马、像、士等对形式的制约作用
	enemyKingPosition=cChess->cChessManPosition[16];
	advisorOnePosition=cChess->cChessManPosition[17];
	advisorTwoPosition=cChess->cChessManPosition[18];
	if(enemyKingPosition == blackBottomLine*16+7)
	{
		//将中双士两边
		if(abs(advisorOnePosition-enemyKingPosition)==1 && abs(advisorTwoPosition-enemyKingPosition)==1)
		{
			//空头炮、炮镇窝心马
			for(int i=9;i<=10;i++)
			{
				int redCannon=cChess->cChessManPosition[i];
				if(redCannon%16 == enemyKingPosition%16)
				{
					int otherChessManNu=0;
					bool hasHeartKnight=false;
					for(int mid=redCannon-16*factor;mid!=enemyKingPosition;mid-=16*factor)
					{
						if(mid==enemyKingPosition+16*factor && (cChess->cChessArray[mid]==blackKnightOne 
											|| cChess->cChessArray[mid]==blackKnightTwo)
						  )
						{
							hasHeartKnight=true;
						}
						else if(cChess->cChessArray[mid]!=0)
							++otherChessManNu;
					}
					//窝心马
					if(hasHeartKnight && otherChessManNu==1)
					{
						redParticularFormScore+=15;
						break;
					}
					//空头炮
					if(otherChessManNu==0 && !hasHeartKnight)
					{
						redParticularFormScore+=18;
						break;
					}
				}
			}
		}
		//将中双士左包围将
		if((advisorOnePosition-enemyKingPosition==16*factor && advisorTwoPosition-enemyKingPosition==1*factor)||
		   (advisorTwoPosition-enemyKingPosition==16*factor && advisorTwoPosition-enemyKingPosition==1*factor))
		{
			//沉底炮
			for(int i=9;i<=10;i++)
			{
				cannonPosition=cChess->cChessManPosition[i];
				if(cannonPosition/16 == blackBottomLine && (cannonPosition-enemyKingPosition)*factor>0)
				{
					bool hasOtherChessMan=false;
					for(int mid=enemyKingPosition+2*factor;mid!=cannonPosition;mid+=1*factor)
					{	
						if(cChess->cChessArray[mid]!=0)
						{
							hasOtherChessMan=true;
							break;
						}
					}
					if(!hasOtherChessMan)
					{
						redParticularFormScore+=25;
						break;
					}
				}
			}
			//幼封车
			for(int i=7;i<=8;i++)
			{
				rookPosition=cChess->cChessManPosition[i];
				if(rookPosition/16 == blackBottomLine && (rookPosition-enemyKingPosition)*factor<0)
				{
					bool hasOtherChessMan=false;
					for(int mid=enemyKingPosition-1*factor;mid!=rookPosition;mid-=1*factor)
					{	
						if(cChess->cChessArray[mid]!=0)
						{
							hasOtherChessMan=true;
							break;
						}
					}
					if(!hasOtherChessMan)
					{
						redParticularFormScore+=28;
						break;
					}
				}
			}
		}
		//将中双士右包围将
		if((advisorOnePosition-enemyKingPosition==16*factor && advisorTwoPosition-enemyKingPosition==-1*factor )||
		   (advisorTwoPosition-enemyKingPosition==16*factor  && advisorTwoPosition-enemyKingPosition==-1*factor ))
		{
			//沉底炮
			for(int i=9;i<=10;i++)
			{
				cannonPosition=cChess->cChessManPosition[i];
				if(cannonPosition/16 == blackBottomLine && (cannonPosition-enemyKingPosition)*factor<0)
				{
					bool hasOtherChessMan=false;
					for(int mid=enemyKingPosition-2*factor;mid!=cannonPosition;mid-=1*factor)
					{	
						if(cChess->cChessArray[mid]!=0)
						{
							hasOtherChessMan=true;
							break;
						}
					}
					if(!hasOtherChessMan)
					{
						redParticularFormScore+=25;
						break;
					}
				}
			}
			//左封车
			for(int i=7;i<=8;i++)
			{
				rookPosition=cChess->cChessManPosition[i];
				if(rookPosition/16 == blackBottomLine && (rookPosition-enemyKingPosition)*factor>0)
				{
					bool hasOtherChessMan=false;
					for(int mid=enemyKingPosition+1*factor;mid!=rookPosition;mid+=1*factor)
					{	
						if(cChess->cChessArray[mid]!=0)
						{
							hasOtherChessMan=true;
							break;
						}
					}
					if(!hasOtherChessMan)
					{
						redParticularFormScore+=28;
						break;
					}
				}
			}
		}
	}
	enemyKingPosition=cChess->cChessManPosition[0];
	advisorOnePosition=cChess->cChessManPosition[1];
	advisorTwoPosition=cChess->cChessManPosition[2];
	if(enemyKingPosition == redBottomLine*16+7)
	{
		//将中双士两边
		if(abs(advisorOnePosition-enemyKingPosition)==1 && abs(advisorTwoPosition-enemyKingPosition)==1)
		{
			//空头炮、炮镇窝心马
			for(int i=25;i<=26;i++)
			{
				int blackCannon=cChess->cChessManPosition[i];
				if(blackCannon%16 == enemyKingPosition%16)
				{
					int otherChessManNu=0;
					bool hasHeartKnight=false;
					for(int mid=blackCannon+16*factor;mid!=enemyKingPosition;mid+=16*factor)
					{
						if(mid==enemyKingPosition-16*factor && (cChess->cChessArray[mid]==redKnightOne 
											|| cChess->cChessArray[mid]==redKnightTwo)
						  )
						{
							hasHeartKnight=true;
						}
						else if(cChess->cChessArray[mid]!=0)
							++otherChessManNu;
					}
					//若只有一个棋子，则将军，此处不考虑
					//窝心马
					if(hasHeartKnight && otherChessManNu==1)
					{
						blackParticularFormScore+=15;
						break;
					}
					//空头炮
					if(otherChessManNu==0 && !hasHeartKnight)
					{
						blackParticularFormScore+=18;
						break;
					}
				}
			}
		}
		//将中双士左包围将
		if((advisorOnePosition-enemyKingPosition==-16*factor && advisorTwoPosition-enemyKingPosition==-1*factor)||
		   (advisorTwoPosition-enemyKingPosition==-16*factor && advisorTwoPosition-enemyKingPosition==-1*factor))
		{
			//沉底炮
			for(int i=25;i<=26;i++)
			{
				cannonPosition=cChess->cChessManPosition[i];
				if(cannonPosition/16 == redBottomLine && (cannonPosition-enemyKingPosition)*factor<0)
				{
					bool hasOtherChessMan=false;
					for(int mid=enemyKingPosition-2*factor;mid!=cannonPosition;mid-=1*factor)
					{	
						if(cChess->cChessArray[mid]!=0)
						{
							hasOtherChessMan=true;
							break;
						}
					}
					if(!hasOtherChessMan)
					{
						blackParticularFormScore+=25;
						break;
					}
				}
			}
			//左封车
			for(int i=23;i<=24;i++)
			{
				rookPosition=cChess->cChessManPosition[i];
				if(rookPosition/16 == blackBottomLine && (rookPosition-enemyKingPosition)*factor>0)
				{
					bool hasOtherChessMan=false;
					for(int mid=enemyKingPosition+1*factor;mid!=rookPosition;mid+=1*factor)
					{	
						if(cChess->cChessArray[mid]!=0)
						{
							hasOtherChessMan=true;
							break;
						}
					}
					if(!hasOtherChessMan)
					{
						blackParticularFormScore+=28;
						break;
					}
				}
			}
		}
		//将中双士右包围将
		if((advisorOnePosition-enemyKingPosition==-16*factor && advisorTwoPosition-enemyKingPosition==1*factor)||
		   (advisorTwoPosition-enemyKingPosition==-16*factor && advisorTwoPosition-enemyKingPosition==1*factor))
		{
			//沉底炮
			for(int i=25;i<=26;i++)
			{
				cannonPosition=cChess->cChessManPosition[i];
				if(cannonPosition/16 == redBottomLine && (cannonPosition-enemyKingPosition)*factor>0)
				{
					bool hasOtherChessMan=false;
					for(int mid=enemyKingPosition+2*factor;mid!=cannonPosition;mid+=1*factor)
					{	
						if(cChess->cChessArray[mid]!=0)
						{
							hasOtherChessMan=true;
							break;
						}
					}
					if(!hasOtherChessMan)
					{
						blackParticularFormScore+=25;
						break;
					}
				}
			}
			//幼封车
			for(int i=23;i<=24;i++)
			{
				rookPosition=cChess->cChessManPosition[i];
				if(rookPosition/16 == redBottomLine && (rookPosition-enemyKingPosition)*factor<0)
				{
					bool hasOtherChessMan=false;
					for(int mid=enemyKingPosition-1*factor;mid!=rookPosition;mid-=1*factor)
					{	
						if(cChess->cChessArray[mid]!=0)
						{
							hasOtherChessMan=true;
							break;
						}
					}
					if(!hasOtherChessMan)
					{
						blackParticularFormScore+=28;
						break;
					}
				}
			}
		}
	}
	if(this->knowledgeLevel >= formSecondLevel)
	{
	//一些形式的录入（网上）
	//
	//
	//
	}
	if(this->knowledgeLevel >= formThirdLevel)
	{
	//一些杀法的录入（参考古籍）
	//
	//
	//
	//
	//
	//
	//
	//
	//
	}
	if(cChess->isRed)
		return redParticularFormScore-blackParticularFormScore;
	else
		return blackParticularFormScore-redParticularFormScore;
}
//待扩展的评价
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
double EvaluateSituation::moreEvaluate() const
{
	return 0;
}
EvaluateSituation::EvaluateSituation(): subjectiveForSelfScope(5),subjectiveForOtherScope(10),
					kingLegalPosVal(1),knightLegalPosVal(2),cannonLegalPosVal(2),rookLegalPosVal(2),
					advisorLegalPosVal(4),bishopLegalPosVal(8),pawnLegalPosVal(16),cChess(NULL)
{
	CChessStruct* temp=new CChessStruct;
	this->cChess=makeSmartPtr(temp); 
	this->evaluatedThreatenLevel=smallThreatenLevel;
	this->subjectiveOfPlayer=0;
	this->knowledgeLevel=staticLevel;
	initializeLegalPosition();
}
EvaluateSituation::EvaluateSituation(SmartPtr<CChessStruct> _cChess):subjectiveForSelfScope(5),subjectiveForOtherScope(10),
							       	     kingLegalPosVal(1),knightLegalPosVal(2),cannonLegalPosVal(2),
								     rookLegalPosVal(2), advisorLegalPosVal(4),bishopLegalPosVal(8),
								     pawnLegalPosVal(16),cChess(_cChess)
{
	if(_cChess.getPtr() == NULL)
	{
		CChessStruct* temp=new CChessStruct;
		this->cChess=makeSmartPtr(temp); 
	}
	this->evaluatedThreatenLevel=smallThreatenLevel;
	this->subjectiveOfPlayer=0;
	this->knowledgeLevel=staticLevel;
	initializeLegalPosition();
}
void EvaluateSituation::bindStruct(SmartPtr<CChessStruct> _cChess)
{
	if(_cChess.getPtr() != NULL)
	{
		this->cChess=_cChess;
	}
}
void EvaluateSituation::setSubjective(SubjectiveSituation _subjectiveOfPlayer)
{
	this->subjectiveOfPlayer=_subjectiveOfPlayer;
}
SubjectiveSituation EvaluateSituation::getSubjective() const
{
	return (SubjectiveSituation)(this->subjectiveOfPlayer);
}
void EvaluateSituation::setKnowledgeLevel(KnowledgeLevel _knowledgeLevel)
{
	this->knowledgeLevel=_knowledgeLevel;
}
KnowledgeLevel EvaluateSituation::getKnowledgeLevel() const
{
	return this->knowledgeLevel;
}
//目前只加入动态、静态（无形式评估）
double EvaluateSituation::evaluate()
{
	double firstItem=0;
	double secondItem=0;
	double thirdItem=0;
	double fourItem=0;
	double resultEvaluate=0;
	if(this->knowledgeLevel >= staticLevel)
		firstItem=chessManStaticPosEvaluate();
	if(this->knowledgeLevel >= dynamicFirstLevel)
		secondItem=chessManDynamicPosEvaluate();
	if(this->knowledgeLevel >= formFirstLevel)
		thirdItem=particularFormEvaluate();
	if(this->knowledgeLevel >= moreKnowledgeLevel)
		fourItem=moreEvaluate();
	resultEvaluate=firstItem+secondItem+thirdItem+fourItem;

	cout<<"静态估值:"<<firstItem<<endl;
	cout<<"动态估值:"<<secondItem<<endl;
	cout<<"形式估值:"<<thirdItem<<endl;
	return resultEvaluate;
}
