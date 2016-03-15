#!/usr/bin/env python
# -*- coding: utf-8 -*-
from PassiveBot.PassiveBot import PassiveBot
from PassiveBot import Data

bot = PassiveBot()

bot.start()
unit =bot.ObjectManager.FindUnitByName("Grull Hawkwind")
unit.DumpPosition()
'''
while 1:
    for unit in bot.ObjectManager.GetUnitsList():
        unit.DumpPosition()
'''
