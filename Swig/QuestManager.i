/* File : QuestManager.i */
%module QuestManager
%{

#include "../WowInteraction/QuestManager.h"
%}
%include "pyabc.i"
%include "std_vector.i"
%include "WowInteractionUtils.i"
%include "Quest.i"
/* Let's just grab the original header file here */
%include "../WowInteraction/QuestManager.h"
%template (UnsignedVector) std::vector<unsigned>;

