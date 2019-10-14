#include"delay.h"

void Delay_Us(int i)
{
	while(--i);
}

void Delay_Ms(int m)
{
 	while(m--)
	{
	 	Delay_Us(125);
	}
}