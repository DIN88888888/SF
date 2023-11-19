#include "TMonitor.h"

TMonitor::TMonitor()
{
}
TMonitor::TMonitor(string name, float weight, float price, float Diag, bool s3d, string Mount, string info) :TItem(name, weight, price,info),TDisplay( Diag), _3dsupport(s3d), _MountType(Mount) {};
TMonitor::~TMonitor() {}

string TMonitor::GetDescription()
{
	return TDisplay::GetDescription() + TMonitor::GetSelfDescription();
}
string TMonitor::GetSelfDescription()
{
	return "Mount type  : " + _MountType + '\n' +
		"Supports 3d : " + ((_3dsupport) ? "True":"False") + '\n';
}
string TMonitor::GetShortDescription()
{
	return "Mon " + getName();
}