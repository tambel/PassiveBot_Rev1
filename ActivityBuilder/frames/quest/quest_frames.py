import wx
from frames.base_frame import BaseFrame
from packets import TargerObjevtInfoRequest,RequestPacket


class SelectQuestGiverFrame(BaseFrame):
    def __init__(self,parent_frame):
        """

        :param parent_frame:
        :type parent_frame: QuestFrame
        :return:
        """
        wx.BaseFrame.__init__(self, parent_frame, wx.ID_ANY, name="Select Quest Giver",pos=(500, 200), size=(600, 400))

        self.con=self.parent.con
        self.select_current_target_button = wx.Button(self.panel, id=-1, label='GetPos',pos=(300, 300), size=(20, 28))
        self.select_current_target_button.Bind(wx.EVT_BUTTON, self.select_current_target_button)
        self.target_label=wx.StaticText(self.parent,label=u"Name: {}\nGUID: {}\nType: {}\nPosition: {}".format("None", 0,0,0),pos=(0, 0), size=(100, 100))
        self.parent.Disable()

    def background_communication(self):
        pass
        '''
        packet =RequestPacket(2)
        self.com.send(packet)
        self.current_target = TargerObjevtInfoRequest(self.com.recieve())
        self.target_label.SetLabelText(u"Name: {}\nGUID: {}\nType: {}\nPosition: {}".format(*(target.fields[f] for f in "name,guid,type,position".split(','))))
        '''

    def select_current_target_button_click(self,event):
        self.parent.selected_target=self.current_target
        self.parent.Enable()
        self.Close()




