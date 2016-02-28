/* File : Unit.i */
%module Unit
%{
#include "../WowInteraction/Unit.h"
%}
%include "WowObject.i"

/* Let's just grab the original header file here */
%include "../WowInteraction/Unit.h"

