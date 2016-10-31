import wx
from main_frame import *


class QuestFrame(wx.Frame):
    def __init__(self,parent_frame):
        """

        :param parent_frame:
        :type parent_frame: Builder
        :return:
        """
        wx.Frame.__init__(self, None, wx.ID_ANY, 'Quest Editor',pos=(500, 200), size=(600, 400))
        panel=wx.Panel(self)
        self.get_target_info_button = wx.Button(panel, id=-1, label='GetPos',pos=(300, 300), size=(20, 28))
        self.get_target_info_button.Bind(wx.EVT_BUTTON, self.get_target_info_button_click)
        self.player_static_box=wx.StaticBox(panel,pos=(0,0),size=(200,120),label="Player Info")

        self.Bind(wx.EVT_CLOSE ,self.OnClose)
        self.parent_frame=parent_frame
        self.com=self.parent_frame.com
        self.player_position_label = wx.StaticText(self.player_static_box, pos=(10,20))
        self.player_rotation_label = wx.StaticText(self.player_static_box,pos=(10,90))
        self.target_label = wx.StaticText(panel, pos=(40,60))

    def get_target_info_button_click(self,event):
        packet =RequestPacket(2)
        self.com.send(packet)
        target=TargerObjevtInfoRequest(self.com.recieve())
        self.target_label.SetLabelText(u"Name: {}\nGUID: {}\nType: {}\nPosition: {}".format(*(target.fields[f] for f in "name,guid,type,position".split(','))))


    def OnClose(self,event):
        self.parent_frame.open_quest_button.Enable()
        self.Hide()






