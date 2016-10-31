import wx
from network_communication import NetworkCommunicator
from packets import *
from quest_editor import QuestFrame
from threading import Thread, Event
import time


class Builder(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None, wx.ID_ANY, 'Activity Builder', pos=(300, 100), size=(400, 400))
        panel = wx.Panel(self)
        self.Bind(wx.EVT_CLOSE ,self.onClose)
        self.Bind(wx.EVT_WINDOW_DESTROY ,self.onDestroy)
        self.open_quest_button = wx.Button(panel, id=-1, label='QuestEditor', pos=(8, 8), size=(100, 40))
        self.open_quest_button.Bind(wx.EVT_BUTTON, self.open_quest_button_click)
        self.com = NetworkCommunicator(("127.0.0.1", 8001))
        self.com.start()
        self.quest_editor = QuestFrame(self)
        self.thread = Thread(target=self.background_communication)
        self.stop_thread=Event()
        self.thread_stopped=Event()
        self.player_position=None
        self.thread.start()

    def open_quest_button_click(self, event):
        self.quest_editor.Show()
        self.open_quest_button.Disable()

    def onClose(self,event):
        self.stop_thread.set()
        self.thread_stopped.wait()
        self.Destroy()

    def onDestroy(self,event):
        self.quest_editor.Destroy()

    def background_communication(self):
        while not self.stop_thread.is_set():

            packet = RequestPacket(1)
            self.com.send(packet)
            self.player_position = PlayerPositionReply(self.com.recieve())
            time.sleep(0.05)
            if self.quest_editor.IsShown() and not self.stop_thread.is_set():
                self.quest_editor.player_position_label.SetLabelText("Player position:\n{}".format(self.player_position.fields["position"]))
                self.quest_editor.player_rotation_label.SetLabelText("Player rotation: {}".format(self.player_position.fields["rotation"].z))

        self.thread_stopped.set()
