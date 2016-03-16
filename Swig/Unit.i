/* File : Unit.i */
%module Unit
%{
#include "../WowInteraction/Unit.h"
%}
%include <std_string.i>
%include <std_wstring.i>
%include "WowObject.i"

/* Let's just grab the original header file here */
%include "../WowInteraction/Unit.h"

