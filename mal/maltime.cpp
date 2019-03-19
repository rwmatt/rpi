#include "maltime.h"

bool isBst(int y, int m, int d, int hr)
{
	int base_year = 2019;
	char begins[] = { 31, 29, 28, 27 };
	char ends[]   = { 27, 25, 31, 30 };
	int idx = y - base_year;
	if(idx>sizeof(ends)) idx = 0; // cope with index out-of-bounds
	if(m<3 || m > 10) return false;
	if(m>3 && m < 10) return true;
	if(m==3 && d < begins[idx]) return false;
	if(m==3 && d > begins[idx]) return true;
	if(m==3 && d == begins[idx] && hr >= 1) return true;
	if(m==3 && d == begins[idx] && hr < 1) return false;

	if(m == 10 && d < ends[idx]) return true;
	if(m == 10 && d > ends[idx]) return false;
	if(m == 10 && d == ends[idx] && hr < 2) return true;
	if(m == 10 && d == ends[idx] && hr >= 2) return false;

	return true;
}

// TODO adjust for month rollover
void adjBST(int y, int &m, int &d, int &hr)
{
	if(!isBst(y, m, d, hr)) return;
	if(hr < 24) { hr++; return; }
	hr = 0; 
	d++;
}
