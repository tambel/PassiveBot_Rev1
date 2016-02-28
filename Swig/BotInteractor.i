/* File : BotInteractor.i */
%module BotInteractor
%{
#include "../BotInteractor/BotInteractor.h"
%}

%include <std_string.i>
%include <std_wstring.i>
%include "Player.i"
%include "NavArea.i"
%include "WowInteractionUtils.i"
%include "WowWorldsDataUtils.i"
%include "ObjectManager.i"
/* Let's just grab the original header file here */
%include "../BotInteractor/BotInteractor.h"


