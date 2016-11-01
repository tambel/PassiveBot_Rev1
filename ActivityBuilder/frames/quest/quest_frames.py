import wx
from frames.base_frame import BaseFrame, BaseDialog
from packets import TargerObjevtInfoRequest,RequestPacket


class SelectQuestGiverFrame(BaseDialog):
    def __init__(self,parent_frame):
        """

        :param parent_frame:
        :type parent_frame: QuestFrame
        :return:
        """
        BaseDialog.__init__(self, parent_frame, name="Select Quest Giver",pos=(500, 150), size=(400, 300))
        self.okButton = wx.Button(self.panel, wx.ID_OK, "OK", pos=(140, 200),size=(100,40))
        self.okButton.Bind(wx.EVT_BUTTON,self.select_current_target_button_click)
        self.com=self.parent.com
        self.start_bg_communication()
        #self.select_current_target_button = wx.Button(self.panel, wx.ID_OK, id=-1, label='GetPos',pos=(300, 300), size=(20, 28))
        #self.select_current_target_button.Bind(wx.EVT_BUTTON, self.select_current_target_button)
        self.target_label=wx.StaticText(self.panel,label=u"Name: {}\nGUID: {}\nType: {}\nPosition: {}".format("None", 0,0,0),pos=(10, 0), size=(100, 50))

    def background_communication(self):
        packet =RequestPacket(2)
        self.com.send(packet)
        self.current_target = TargerObjevtInfoRequest(self.com.recieve())
        self.target_label.SetLabelText(u"Name: {}\nGUID: {}\nType: {}\nPosition: {}".format(*(self.current_target.fields[f] for f in "name,guid,type,position".split(','))))

    def select_current_target_button_click(self,event):
        if self.current_target is not None:
            self.EndModal(wx.ID_OK)
        else:
            self.EndModal(wx.ID_CANCEL)






