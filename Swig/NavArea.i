/* File : NavArea.i */
%module NavArea
%{
#include "../WowWorldsData/NavArea.h"
%}
%include "Area.i"
%include "WowInteractionUtils.i"
%include "WowWorldsDataUtils.i"
/* Let's just grab the original header file here */
%include "../WowWorldsData/NavArea.h"

