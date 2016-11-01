import wx
from network_communication import NetworkCommunicator
from frames.quest.quest_editor import QuestFrame
from frames.base_frame import BaseFrame

from threading import Thread

class Builder(BaseFrame):
    def __init__(self):
        BaseFrame.__init__(self, None,name= 'Activity Builder', pos=(300, 100), size=(400, 400))
        self.open_quest_button = wx.Button(self.panel, id=-1, label='QuestEditor', pos=(8, 8), size=(100, 40))
        self.open_quest_button.Bind(wx.EVT_BUTTON, self.open_quest_button_click)
        self.com = NetworkCommunicator(("127.0.0.1", 8001))
        self.com.start()
        self.player_position = None
        self.start_bg_communication()

    def open_quest_button_click(self, event):
        quest_editor = QuestFrame(self)
        quest_editor.Show()
        self.open_quest_button.Disable()

    def background_communication(self):
        pass
