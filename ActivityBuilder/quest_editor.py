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
        self.get_target_info_button = wx.Button(panel, id=-1, label='GetPos',pos=(8, 8), size=(20, 28))
        self.get_target_info_button.Bind(wx.EVT_BUTTON, self.get_target_info_button_click)
        self.Bind(wx.EVT_CLOSE ,self.OnClose)
        self.parent_frame=parent_frame
        self.com=self.parent_frame.com
        self.player_position_label = wx.StaticText(panel, pos=(30,10))
        self.player_rotation_label = wx.StaticText(panel,pos=(30,30))
        self.target_label = wx.StaticText(panel, pos=(40,10))
        #self.player_rotation_label = wx.StaticText(panel,pos=(30,30))

    def get_target_info_button_click(self,event):
        packet =RequestPacket(2)
        self.com.send(packet)
        target=TargerObjevtInfoRequest(self.com.recieve())
        self.target_label.SetLabelText(u"".join([u"{}\n".format(v) for v in target.fields.values()]))


    def OnClose(self,event):
        self.parent_frame.open_quest_button.Enable()
        self.Hide()






