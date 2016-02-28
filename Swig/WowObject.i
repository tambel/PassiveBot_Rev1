/* File : WowObject.i */
%module WowObject
%{
#include "../WowInteraction/WowObject.h"
%}
%include "WowInteractionUtils.i"
/* Let's just grab the original header file here */
%include "../WowInteraction/WowObject.h"

