/* File : WowWorldsDataUtils.i */
%module WowWorldsDataUtils
%{
#include "../WowWorldsData/Utils.h"
%}
%include "WowInteractionUtils.i"
/* Let's just grab the original header file here */
%include "../WowWorldsData/Utils.h"

